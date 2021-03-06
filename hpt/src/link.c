/*****************************************************************************
* Link for HPT (FTN NetMail/EchoMail Tosser)
*****************************************************************************
* Copyright (C) 1998
*
* Kolya Nesterov
*
* Fido:     2:463/7208.53
* Kiev, Ukraine
*
* This file is part of HPT.
*
* HPT is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2, or (at your option) any
* later version.
*
* HPT is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with HPT; see the file COPYING.  If not, write to the Free
* Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*****************************************************************************
* $Id: link.c,v 1.82 2015/05/25 15:35:49 dukelsky Exp $
*/
/* Revision log:
19.12.98 - first version
28.11.99 - write modified chains only (by L. Lisowsky 2:5020/454.2)
*/

/*
For now reply linking is performed using the msgid/reply kludges
TODO:
subject linking
FIXME:	do better when finding if msg links need to be updated
The problem with original patch by Leonid was that if msg had 
some reply links written in replies or replyto fields but
no replies were found during linkage reply&replyto fields won't
be cleared. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <huskylib/compiler.h>

#ifdef __OS2__
#define INCL_DOSFILEMSG /* for DosSetMaxFH() */
#include <os2.h>
#endif

#include <smapi/msgapi.h>
#include <smapi/api_jam.h>

#include <fidoconf/fidoconf.h>
#include <fidoconf/common.h>
#include <huskylib/typesize.h>
#include <huskylib/crc.h>
#include <huskylib/log.h>

#include "global.h"
#include "fcommon.h"
#include "hptafix.h"

#define MAX_INCORE	10000UL	/*  if more messages, do not link incore */

dword Jam_Crc32(unsigned char* buff, dword len);
char *Jam_GetKludge(HAREA jm, dword msgnum, word what);
JAMHDR *Jam_GetHdr(HAREA jm, dword msgnum);
void Jam_WriteHdr(HAREA jm, JAMHDR *jamhdr, dword msgnum);

/* internal structure holding msg's related to link information */
/* used by linkArea */
struct msginfo {
    char *msgId;
    char *replyId;
    HMSG msgh;
    union {
        XMSG *xmsg;
        JAMHDR *jamhdr;
    } hdr;
    UMSGID msgPos; 
    
    short freeReply;
    char relinked;
    UMSGID replyto, replynext;
    UMSGID replies[MAX_REPLY];
};

struct hashinfo {
    dword crc;
    int idx;
};

typedef struct msginfo s_msginfo;

int linkJamByCRC = 0;

static s_msginfo *findMsgId(s_msginfo *entries, struct hashinfo *hash, dword hashSize, char *msgId, int add, dword crc32)
{
    unsigned long h, d = 1;
    dword crc;
    if (crc32 == 0)
        h = crc = strcrc32(strUpper(msgId), 0xFFFFFFFFL);
    else if (crc32 == 0xFFFFFFFFL)
        h = crc = Jam_Crc32((UCHAR*)strUpper(msgId), strlen(msgId));
    else
        h = crc = crc32;
    while (d < hashSize) {
        h %= hashSize;
        if (hash[h].idx == 0) {
            /* Found free entry */
            if (!add) return NULL;
            hash[h].idx = add;
            hash[h].crc = crc;
            return &(entries[add-1]);
        }
        if (hash[h].crc == crc && !stricmp(entries[hash[h].idx-1].msgId, msgId)) {
            /* Found it ! */
            return &(entries[hash[h].idx-1]);
        } else {
            /* Collision, resolve it */
            h++; 
            d++;
        };
    };
    return NULL;
}

static char *stripDomain(char *msgid)
{
    char *p, *p1;
    hs_addr addr = {0};

    if (msgid == NULL) return msgid;
    for (p=msgid; *p && (isdigit(*p) || *p==':' || *p=='/' || *p=='.'); p++);
    if (*p != '@') return msgid;
    if (parseFtnAddrZ(msgid, &addr, FTNADDR_GOOD, (const char**)(&p1)) & FTNADDR_ERROR) return msgid;
     /*  '(const char**)(&p1)' is needs for prevent warning "passing arg 4 of `parseFtnAddrZ' from incompatible pointer type" */

    for (; *p1 && !isspace(*p1); p1++);
    strcpy(p, p1);
    return msgid;
}

static char *GetKludgeText(byte *ctl, char *kludge)
{
    char *pKludge, *pToken;
    
    pToken = (char *) GetCtrlToken(ctl, (byte *)kludge);
    if (pToken) {
        pKludge = safe_strdup(pToken+1+strlen(kludge));
        nfree(pToken);
        return stripDomain(pKludge);
    } else
        return NULL;
}

static char *crc2str(dword crc)
{
   char *ptr, *str;

   if (crc==0 || crc==0xFFFFFFFFul)
      return NULL;
   ptr=str=safe_malloc(11);
   /* backward figures order, it's not mistake */
   while (crc) {
      *ptr++=(char)('0'+crc%10);
      crc/=10;
   }
   *ptr='\0';
   return str;
}

/* linking for msgid/reply */
int linkArea(s_area *area, int netMail)
{
    
    HAREA harea;
    HMSG  hmsg = NULL;
    XMSG  xmsg;
    s_msginfo *msgs;   
    dword msgsNum, hashNums, i, ctlen, cctlen;
    struct hashinfo *hash;
    byte *ctl;
    char *msgId;
    int jam;
    
    s_msginfo *curr;
    
    if (area->msgbType == MSGTYPE_PASSTHROUGH) return 0;
    
    if (area->nolink) {
        w_log(LL_LINKING, "%s has nolink option, ignoring", area->areaName);
        return 0;
    }
    
    harea = MsgOpenArea((UCHAR *) area->fileName, MSGAREA_NORMAL,
        (word)(area->msgbType | (netMail ? 0 : MSGTYPE_ECHO)));
    if (harea) {
        w_log(LL_LINKING, "Linking area %s", area->areaName);
        msgsNum = MsgGetNumMsg(harea);
        if (msgsNum < 2) { /* Really nothing to link */
            MsgCloseArea(harea);
            return 0;
        };
#ifdef __OS2__
        if (area->msgbType & MSGTYPE_SDM)
#if defined(__WATCOMC__)
            _grow_handles(msgsNum+20);
#else /* EMX */
        DosSetMaxFH(msgsNum+20);
#endif
#endif
        
        hashNums = msgsNum + msgsNum / 10 + 10;
        hash = safe_malloc(hashNums * sizeof(*hash));
        memset(hash, '\0', hashNums * sizeof(*hash));
        msgs = safe_malloc(msgsNum * sizeof(s_msginfo));
        memset(msgs, '\0', msgsNum * sizeof(s_msginfo));
        ctl = (byte *) safe_malloc(cctlen = 1); /* Some libs doesn't accept relloc(NULL, ..
                                                * So let it be initalized
        */
        jam = !!(area->msgbType & MSGTYPE_JAM);
        /* Area linking is done in three passes */
        /* Pass 1st : read all message information in memory */
        
        for (i = 1; i <= msgsNum; i++) {
            if (jam) {
                if (linkJamByCRC)
                   msgId = crc2str(Jam_GetHdr(harea, i)->MsgIdCRC);
                else {
                   msgId = Jam_GetKludge(harea, i, JAMSFLD_MSGID);
                   stripDomain(msgId);
                }
            } else {
                hmsg  = MsgOpenMsg(harea, (word)((msgsNum >= MAX_INCORE) ? MOPEN_READ : (MOPEN_READ|MOPEN_WRITE)), i);
                if (hmsg == NULL) {
                    continue;
                }
                ctlen = MsgGetCtrlLen(hmsg);
                if (ctlen == 0 ) {
                    MsgCloseMsg(hmsg);
                    w_log(LL_WARN, "msg %ld has no control information: thrown from reply chain", i);
                    continue;
                }
                
                if (ctlen > cctlen) {
                    cctlen = ctlen;
                    ctl   = (byte *) safe_realloc(ctl, ctlen + 1);
                }
                
                MsgReadMsg(hmsg, &xmsg, 0, 0, NULL, ctlen, ctl);
                ctl[ctlen] = '\0';
                msgId   = GetKludgeText(ctl, "MSGID");
            }
            if (msgId == NULL) {
                w_log(LL_WARN, "msg %ld hasn't got any MSGID, replying is not possible", i);
                if (!jam)
                    MsgCloseMsg(hmsg);
                continue;
            }
            curr = findMsgId(msgs, hash, hashNums, msgId, i,
                (jam && linkJamByCRC) ? Jam_GetHdr(harea, i)->MsgIdCRC : 0);
            if (curr == NULL) {
                w_log(LL_ERR, "hash table overflow. Tell developers about it!"); 
                /*  try to free as much as possible */
                /*  FIXME : remove blocks themselves */
                nfree(msgId);
                if (!jam) {
                    nfree(ctl);
                    MsgCloseMsg(hmsg);
                    MsgCloseArea(harea);
                }
                return 0;
            };
            if (curr -> msgId != NULL) {
                w_log(LL_DEBUGB, "msg %ld has dupes in msgbase: thrown out from reply chain", i);
                if (!jam) {
                    MsgCloseMsg(hmsg);
                    nfree(msgId);
                }
                continue;
            }
            curr -> msgId = msgId;
            curr -> msgPos  = MsgMsgnToUid(harea, i);
            curr -> freeReply = 0;
            curr -> relinked = 0;
            curr -> replyto = curr -> replynext = curr->replies[0] = 0;
            curr -> msgh = NULL;
            if (jam) {
                curr -> hdr.jamhdr = Jam_GetHdr(harea, i);
                if (linkJamByCRC)
                   curr -> replyId = crc2str(curr->hdr.jamhdr->ReplyCRC);
                else {
                   curr -> replyId = Jam_GetKludge(harea, i, JAMSFLD_REPLYID);
                   stripDomain(curr -> replyId);
                }
            } else {
                curr -> replyId = GetKludgeText(ctl, "REPLY");
                curr -> hdr.xmsg = memdup(&xmsg, sizeof(XMSG));
                if (msgsNum >= MAX_INCORE)
                    MsgCloseMsg(hmsg), curr -> msgh = NULL;
                else
                    curr -> msgh = hmsg; 
            }
        }
        
        /* Pass 2nd : search for reply links and build relations */
        for (i = 0; i < msgsNum; i++) {
            if (msgs[i].msgId == NULL || msgs[i].replyId == NULL)
                continue;
            curr = findMsgId(msgs, hash, hashNums, msgs[i].replyId, 0,
                (jam && linkJamByCRC) ? Jam_GetHdr(harea, i+1)->ReplyCRC : 0);
            if (curr == NULL) continue;
            if (curr -> msgId == NULL) continue;
            if (curr -> freeReply >= MAX_REPLY-4 &&
                (area->msgbType & (MSGTYPE_JAM | MSGTYPE_SDM))) {
                curr -> freeReply--;
                curr -> replies[curr -> freeReply - 1] = curr -> replies[curr -> freeReply];
            }
            if (curr -> freeReply >= MAX_REPLY) {
                w_log(LL_WARN, "msg %ld: replies count for msg %ld exceeds %d, rest of the replies won't be linked", i+1, curr-msgs+1, MAX_REPLY);
                continue;
            }
            curr -> replies[curr -> freeReply] = i;
            if (!jam) {
                if (curr->hdr.xmsg->replies[curr->freeReply]!=msgs[i].msgPos) {
                    curr->hdr.xmsg->replies[curr->freeReply]=msgs[i].msgPos;
                    if ((area->msgbType & MSGTYPE_SQUISH) ||
                        curr->freeReply == 0)
                        curr -> relinked = 1;
                }
                (curr -> freeReply)++;
                msgs[i].replyto = curr -> msgPos;
                if (msgs[i].hdr.xmsg -> replyto != curr -> msgPos) {
                    msgs[i].hdr.xmsg -> replyto = curr -> msgPos;
                    msgs[i].relinked = 1;
                }
            } else {
                if (curr -> freeReply == 0) {
                    if (curr->hdr.jamhdr->Reply1st != msgs[i].msgPos) {
                        curr->hdr.jamhdr->Reply1st = msgs[i].msgPos;
                        curr -> relinked = 1;
                    }
                } else {
                    int replyprev = curr -> replies[curr -> freeReply - 1];
                    msgs[replyprev].replynext = msgs[i].msgPos;
                    if (msgs[replyprev].hdr.jamhdr -> ReplyNext != msgs[i].msgPos) {
                        msgs[replyprev].hdr.jamhdr -> ReplyNext = msgs[i].msgPos;
                        msgs[replyprev].relinked = 1;
                    }
                }
                (curr -> freeReply)++;
                msgs[i].replyto = curr -> msgPos;
                if (msgs[i].hdr.jamhdr -> ReplyTo != curr -> msgPos) {
                    msgs[i].hdr.jamhdr -> ReplyTo = curr -> msgPos;
                    msgs[i].relinked = 1;
                }
            }
        }
        
        /* Pass 3rd : write information back to msgbase */
        for (i = 0; i < msgsNum; i++) {
            int j;
            if (msgs[i].msgId == NULL)
                continue;
            if (jam) {
                if ((msgs[i].replyto   != msgs[i].hdr.jamhdr->ReplyTo) ||
                    (msgs[i].replynext != msgs[i].hdr.jamhdr->ReplyNext) ||
                    (msgs[i].hdr.jamhdr->Reply1st && !msgs[i].freeReply) ||
                    msgs[i].relinked) {
                    if (!msgs[i].freeReply) msgs[i].hdr.jamhdr->Reply1st = 0;
                    msgs[i].hdr.jamhdr->ReplyTo   = msgs[i].replyto;
                    msgs[i].hdr.jamhdr->ReplyNext = msgs[i].replynext;
                    Jam_WriteHdr(harea, msgs[i].hdr.jamhdr, i+1);
                }
                nfree(msgs[i].msgId);
                nfree(msgs[i].replyId);
                continue;
            }
            for (j=0; j<MAX_REPLY && msgs[i].hdr.xmsg->replies[j]; j++);
            if (msgs[i].relinked != 0 ||
                msgs[i].replyto != msgs[i].hdr.xmsg->replyto ||
                ((area->msgbType & MSGTYPE_SQUISH) && msgs[i].freeReply != j) ||
                (msgs[i].freeReply == 0 && j)) {
                if (msgs[i].freeReply<MAX_REPLY)
                    msgs[i].hdr.xmsg->replies[msgs[i].freeReply] = 0;
                msgs[i].hdr.xmsg->replyto = msgs[i].replyto;
                if (msgs[i].msgh == NULL)
                    msgs[i].msgh = MsgOpenMsg(harea, MOPEN_READ|MOPEN_WRITE, i+1);
                if (msgs[i].msgh)
                    if (0!=MsgWriteMsg(msgs[i].msgh, 0, msgs[i].hdr.xmsg, NULL, 0, 0, 0, NULL))
                        w_log(LL_ERR, "Could not update msg in area %s! Check the wholeness of messagebase, please.", area->areaName);
            }
            if (msgs[i].msgh)
                MsgCloseMsg(msgs[i].msgh);
            
            /* free this node */
            nfree(msgs[i].msgId);
            nfree(msgs[i].replyId);
            nfree(msgs[i].hdr.xmsg);
        }
        /* close everything, free all allocated memory */
        nfree(msgs);
        nfree(hash);
        nfree(ctl);
        MsgCloseArea(harea);
   } else {
       w_log(LL_ERR, "could not open area %s", area->areaName);
       return 0;
   }
   return 1;
}

void linkAreas(char *name)
{
    FILE *f;
    char *line;
    s_area *area;
    unsigned int i;
    
    /*  link only one area */
    if (name != NULL) {
        if ((area = getNetMailArea(config, name)) != NULL) {
            linkArea(area,1);
        } else {
            area = getArea(config, name);
            if (area->areaName != config->badArea.areaName) linkArea(area,0);
            else w_log(LL_ERR, "area %s not found for link",name);
        }
        return;
    }
    
    /*  open importlog file */
    if (config->LinkWithImportlog != lwiNo) {
        f = fopen(config->importlog, "r");
    } else {
        f = NULL;
    }
    
    if (f == NULL) {

        if(config->LinkWithImportlog != lwiNo && config->importlog)
        {
            w_log(LL_CRIT, "Nothing to link. Use \"hpt link *\" to perform linking all areas");
            return;
        }
        /*  if importlog does not exist link all areas */
        w_log(LL_LINKING, "Linking all Areas.");
        
        /* link all echomail areas */
        for (i = 0; i < config -> echoAreaCount; i++)
            linkArea(&(config -> echoAreas[i]), 0);
        /* link all local areas */
        for (i = 0; i < config -> localAreaCount; i++)
            linkArea(&(config -> localAreas[i]), 0);
        /* link NetMailAreas */
        for (i = 0; i < config -> netMailAreaCount; i++)
            linkArea(&(config -> netMailAreas[i]), 1);
        
    } else {
        w_log(LL_LINKING, "Using importlogfile -> linking only listed Areas");
        
        while (!feof(f)) {
            line = readLine(f);
            
            if (line != NULL) {
                
                if ((area = getNetMailArea(config, line)) != NULL) {
                    if(area->scn == 0)
                    {
                        linkArea(area,1);
                        area->scn = 1;
                    }
                    
                } else {
                    area = getArea(config, line);
                    if (area->areaName != config->badArea.areaName)
                        if(area->scn == 0)
                        {
                            linkArea(area,1);
                            area->scn = 1;
                        }
                        nfree(line);
                }
            }
        }
        fclose(f);
        if (config->LinkWithImportlog == lwiKill) 
            remove(config->importlog);
    }
}
