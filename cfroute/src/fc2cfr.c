#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fidoconf/fidoconf.h"
#include "fidoconf/common.h"

static int writeArea(FILE *f, s_area *area, char netMail) {
   switch (area->msgbType) {

      case (MSGTYPE_SQUISH): fprintf(f, "NETSQUISH ");
                             break;

      case (MSGTYPE_SDM):    fprintf(f, "NETMAIL ");
                             break;

      case (MSGTYPE_JAM):    fprintf(f, "NETJAM ");
                             break;

      default:               return -1;
   }

   if (netMail != 1) return -1;

   fprintf(f, "%s\n", area->fileName);

   return 0;
}

static void fc_print_address(FILE *f, hs_addr *paddr)
{
    fprintf (f, "%u:%u/%u.%u", paddr->zone, paddr->net,
             paddr->node, paddr->point);
    if (paddr->domain != NULL && *(paddr->domain))
    {
        fprintf (f, "@%s", paddr->domain);
    }
}

#define ALL id_route
#define RFILE id_routeFile
#define MAIL id_routeMail

#define ROUTE 0
#define DIRECT 3
#define BOSS  6
#define NOPACK 9

const char *commands[] = {
    "ROUTE", "ROUTE-FILE", "ROUTE-MAIL",
    "DIRECT-TO", "DIRECT-MAIL", "DIRECT-FILES",
    "ROUTE-BOSS", "ROUTE-BOSS-MAIL", "ROUTE-BOSS-FILES",
    "NOPACK", "NOPACK-MAIL", "NOPACK-FILES"
};

static void fc_convert_route(FILE *f, s_route *route, int what)
{
    int mode = ROUTE;
    static s_route *lastroute = NULL;

    if (lastroute == NULL || route->flavour != lastroute->flavour ||
        (route->target == NULL && lastroute->target != NULL) ||
        (route->target != NULL && lastroute->target == NULL) ||
        (route->target != NULL && lastroute->target != NULL &&
         addrComp((route->target->hisAka), (lastroute->target->hisAka))) ||
        (route->target == NULL && lastroute->target == NULL &&
         route->routeVia != lastroute->routeVia))
    {
        fprintf(f, "\n");
        if (route->target == NULL)
        {
            switch (route->routeVia)
            {
            case route_zero:
            case nopack:
                mode = NOPACK;
                break;

            case noroute:
                mode = DIRECT;
                break;

            case boss:
                mode = BOSS;
                break;

            case host:
            case hub:
            case route_extern:
                fprintf (stderr, "Host, Hub and External routing is not supported "
                         "by cfroute.\n");
                abort();
            }
        }

        fprintf(f, "%s ", commands[mode + what]);

        if (route->flavour != normal)
        {
            switch (route->flavour)
            {
            case hold:      fprintf(f, "HOLD "); break;
            case crash:     fprintf(f, "CRASH "); break;
            case direct:    fprintf(f, "DIRECT "); break;
            case immediate: fprintf(f, "IMMEDIATE "); break;
            default:        fprintf(stderr, "unknwon flavour\n"); abort();
            }
        }

        if (route->target != NULL)
        {
            fc_print_address(f, &(route->target->hisAka));
            fprintf(f, " ");
        }
    }

    fprintf(f, "%s ", route->pattern);

    lastroute = route;
}

#define CVT_ROUTES 1
#define CVT_PASSWORDS 2
#define CVT_SETTINGS 4

int generateCfrouteConfig(s_fidoconfig *config, const char *fileName, int what)
{
    FILE *f;
    int  i;
    s_area *area;

    f = fopen(fileName, "w");
    if (f != NULL)
    {
        fprintf (f,";;CFROUTE configuration file automatically generated by fc2cfr");
        if (what & CVT_ROUTES) fprintf(f," -r");
        if (what & CVT_PASSWORDS) fprintf(f," -p");
        if (what & CVT_SETTINGS) fprintf(f," -s");
        fprintf(f,"\n");

        if (what & CVT_SETTINGS)
        {
            fprintf (f,"EOLENDSCOMMAND  ; end of line terminates any comments. This avoids ambiguities.\n");

            fprintf (f,";VIABOSSHOLD\n");
            fprintf (f,";VIABOSSDIRECT  ; we do NOT set VIABOSSDIRECT to mimic hpt's behaviour, but I\n");
            fprintf (f,"                ; strongly recommend that you DO set VIABOSSDIRECT. Please\n");
            fprintf (f,"                ; do read the cfroute documentation.\n");
            fprintf (f,"NODOMAINDIR     ; hpt and fidoconfig do not support domain outbound.\n");
            fprintf (f,";KILLINTRANSIT  ; not necessary; hpt sets the K/S flag on intransit mail\n");

            fprintf (f,"\n");

            if (config->logFileDir != NULL)
                fprintf (f,"LOG %scfroute.log\n", config->logFileDir);

            if (config->outbound != NULL)
                fprintf (f,"OUTBOUND %s\n", config->outbound);
            fprintf (f,"                ; cfroute must know about all your inbound\n");
            fprintf (f,"                ; directories in order to find file attaches.\n");
            if (config->inbound != NULL)
                fprintf(f, "INBOUND %s\n", config->inbound);
            if (config->protInbound != NULL &&
                config->protInbound != config->inbound)
                fprintf(f, "INBOUND %s\n", config->protInbound);
            if (config->listInbound != NULL &&
                config->listInbound != config->inbound)
                fprintf(f, "INBOUND %s\n", config->listInbound);
            if (config->tempInbound != NULL &&
                config->tempInbound != config->inbound)
                fprintf(f, "INBOUND %s\n", config->tempInbound);
            if (config->localInbound != NULL &&
                config->localInbound != config->inbound)
                fprintf(f, "INBOUND %s\n", config->localInbound);

            if (config->outtab != NULL)
              fprintf (f,"RECODE %s\n", config->outtab);
            if (config->lockfile != NULL)
              fprintf (f, "CHECKFILE %s\n", config->lockfile);

            fprintf (f, "\n");

            fprintf (f, "MAIN ");
            fc_print_address(f,config->addr);
            fprintf (f, "\n");

            for (i=1; i<config->addrCount; i++)
            {
                fprintf(f, "AKA  ");
                fc_print_address(f,config->addr + i);
                fprintf(f, "\n");
            }

            fprintf(f, "\n");

            for (i=0; i<config->netMailAreaCount; i++) {
                writeArea(f, &(config->netMailAreas[i]), 1);
            }

            fprintf (f, "\n");
        }

        if (what & CVT_PASSWORDS)
        {
            for (i=0; i<config->linkCount; i++)
            {
                if (config->links[i].pktPwd != NULL)
		{
		    if (config->links[i].pktPwd[0] != 0)
                    {
                        fprintf(f, "PASSWORD %s ", config->links[i].pktPwd);
                        fc_print_address(f,&(config->links[i].hisAka));
                        fprintf(f, "\n");
                    }
                }
                else if (config->links[i].defaultPwd != NULL)
                {
                    if (config->links[i].defaultPwd[0] != 0)
                    {
                        fprintf(f, "PASSWORD %s ", config->links[i].defaultPwd);
                        fc_print_address(f,&(config->links[i].hisAka));
                        fprintf(f, "\n");
                    }
                }
            }

            fprintf (f, "\n");
        }

        if (what & CVT_ROUTES)
        {
            fprintf (f,"TOPDOWN         ; the FIRST matching route is taken - this is the same logic\n");
            fprintf (f,"                ; which hpt is using\n");

/*          for (i = 0; i < config->routeFileCount; i++)
            {
                fc_convert_route(f, config->routeFile + i, RFILE);
            }
            for (i = 0; i < config->routeMailCount; i++)
            {
                fc_convert_route(f, config->routeMail + i, MAIL);
            } */
            for (i = 0; i < config->routeCount; i++)
            {
                fc_convert_route(f, config->route + i, config->route[i].id);
            }

            fprintf (f, "\n");
        }

        return 0;
    } else printf("Could not write %s\n", fileName);

    return 1;
}

int main (int argc, char *argv[])
{
    s_fidoconfig *config;
    int i;
    const char *fn = NULL;
    int usage = 0;
    int what = 0;

    printf("fconf2cfr\n");
    printf("---------\n");

    for (i = 1; (i < argc) && (!usage) ; i++)
    {
        if (argv[i][0]=='-')
        {
            switch(argv[i][1])
            {
              case 'r':
                what |= CVT_ROUTES;
                break;
              case 'p':
                what |= CVT_PASSWORDS;
                break;
              case 's':
                what |= CVT_SETTINGS;
                break;
              default:
                usage = 1;
                break;
            }
        }
        else
        {
            if (fn == NULL)
              fn = argv[i];
            else
              usage = 1;
        }
    }


    if (fn == NULL || usage)
    {
        printf("\nConverts a FIDOCONFIG file to a CFROUTE configuration file. Usage:\n");
        printf("   fc2cfr [<flag> ...] <cfrouteConfigFileName>\n\n");
        printf("The following values are allowable for <flag>:\n");
        printf("   -r   convert routing information\n");
        printf("   -p   convert packet passwords\n");
        printf("   -s   convert misc. other settings (like netmailarea etc.)\n");
        printf("You can combine one or more flags; if you specify no flags at all,\n");
        printf("everything will be converted.\n");
        printf("\nExample:\n");
        printf("   fc2cfr cfroute.cfg\n\n");
        printf("The location of the fidoconfig file is taken from the FIDOCONFIG\n");
        printf("environment variable.\n");
        return 1;
    }

    if (what == 0) what = 0xFF;

    printf("Generating Config-file %s\n", fn);

    config = readConfig(NULL);
    if (config!= NULL)
    {
        generateCfrouteConfig(config, fn, what);
        disposeConfig(config);
        return 0;
    }

    return 1;
}
