{ $O+,F+,I-,S-,R-,V-}
{$I-}
Unit MKFFile; {Buffered File Object Unit}

{$IfDef FPC}
 {$PackRecords 1}
{$EndIf}

Interface

Type FBufType = Array[0..$fff0] of Byte;

Type FFileObj = Object
  BufFile: File;             {File to be buffered}
  Buf: ^FBufType;            {Pointer to the buffer-actual size given by init}
  BufStart: LongInt;         {File position of buffer start}
  BufSize: LongInt;          {Size of the buffer}
  {$IFDEF VirtualPascal}
  BufChars: LongInt;
  {$ELSE}
  BufChars: Word;            {Number of valid characters in the buffer}
  {$ENDIF}
  CurrSize: LongInt;         {Current file size}
  NeedWritten: Boolean;      {Buffer dirty/needs written flag}
  IsOpen: Boolean;           {File is currently open flag}
  CurrPos: LongInt;          {Current position in file/buffer}
  Constructor Init(BSize: Word);
    {Initialize object and set buffer size/allocate memory}
  Destructor Done; Virtual;  {Done}
  Function  OpenFile(FName: String; FMode: Word): Boolean;  Virtual;
                             {Open a file FNAME in the filemode FMode}
  Function  CloseFile: Boolean; Virtual; {Close the currently open file}
  Function  BlkRead(Var V; Num: Word; Var NumRead: Word): Boolean; Virtual;
    {Equivalent to BlockRead but makes use of buffer to reduce real reads}
  Function  BlkWrite(Var V; Num: Word): Boolean; Virtual;
    {Equivalent to BlockWrite but uses buffer to reduce real writes}
  Function  SeekFile(FP: LongInt): Boolean; Virtual;
    {Equivalent to seek but uses buffer to reduce real seeks}
  Function  WriteBuffer: Boolean; Virtual;
    {Internal use normally - flushes buffer if needed}
  Function  ReadBuffer: Boolean; Virtual;
    {Internal use normally - refills buffer}
  Function  RawSize: LongInt; Virtual;
    {Pass through to filesize function}
  Function  FilePos: LongInt; Virtual;
  End;


Implementation

Uses MKFile
{$IfDef WINDOWS}
 , WinDOS
{$Else}
 , DOS
 {$IfDef OPRO}
 , OpCRT
 {$Else}
  {$IfNDef FPC}
 , CRT
  {$EndIf}
 {$EndIf}
{$EndIf}
;

Constructor FFileObj.Init(BSize: Word);
  Begin
  Buf := Nil;
  BufSize := BSize;
  BufStart := 0;           {Invalidate buffer}
  BufChars := 0;
  IsOpen := False;           {Initialize values}
  NeedWritten := False;
  CurrPos := 0;
  GetMem(Buf, BufSize);      {Allocate memory for buffer}
  If Buf = Nil Then
    Fail;
  End;


Destructor FFileObj.Done;
Begin
  If IsOpen Then CloseFile;
  If Buf <> Nil Then FreeMem(Buf, BufSize);
End;



Function FFileObj.OpenFile(FName: String; FMode: Word): Boolean;
  Var
    DoneOk: Boolean;

  Begin
  If IoResult <> 0 Then;     {protect against unchecked errors in calling proc}
  DoneOk := True;
  If IsOpen Then CloseFile;
  If DoneOk Then Begin                    {Create file if needed}
    If Not FileExist(FName) Then
      DoneOk := SaveFile(FName, DoneOk, 0) = 0;
    End;
  If DoneOk Then
    Begin                    {open file}
    Assign(BufFile, FName);
    FileMode := FMode;
    If DoneOk Then
      DoneOk := shReset(BufFile, 1);
    IsOpen := DoneOk;
    CurrPos := 0;            {Initialize file position}
    BufStart := 0;           {Invalidate buffer}
    BufChars := 0;
    NeedWritten := False;
    CurrSize := RawSize;
    End;
  OpenFile := DoneOk;
  End;


Function FFileObj.CloseFile: Boolean;
  Var
    DoneOk: Boolean;

Begin
  If not IsOpen then Exit;
  If IoResult <> 0 Then;
  DoneOk := True;
  If NeedWritten Then DoneOk := WriteBuffer;
  If DoneOk Then Begin
    Close(BufFile);         {Close file}
    DoneOk := (IoResult = 0);
  End;
  If DoneOk Then IsOpen := False;
  CloseFile := DoneOk;
End;


Function FFileObj.BlkRead(Var V; Num: Word; Var NumRead: Word): Boolean;
  Var
    Tmp: LongInt;                      {Number of chars to write}
    DoneOk: Boolean;

  Begin
  If IoResult <> 0 Then;
  DoneOk := IsOpen;
  NumRead := 0;                        {Initialize number read to zero}
  DoneOk := SeekFile(CurrPos);          {Make currpos valid}
  While ((NumRead < Num) and (DoneOk)) Do
    Begin
    If BufChars = 0 Then
      DoneOk := ReadBuffer;
    Tmp := Num - NumRead;
    If Tmp > (BufChars - (CurrPos - BufStart)) Then
      Tmp := (BufChars - (CurrPos - BufStart));
    Move(Buf^[CurrPos - BufStart], FBufType(V)[NumRead] , Tmp);
    Inc(NumRead, Tmp);
    DoneOk := SeekFile(CurrPos + Tmp);
    If CurrPos >= CurrSize Then
      Num := NumRead;
    End;
  BlkRead := DoneOk;
  End;


Function FFileObj.BlkWrite(Var V; Num: Word): Boolean;
  Var
    Tmp: LongInt;                      {Number of chars to write}
    NumWritten: LongInt;               {Number of chars written}
    DoneOk: Boolean;

  Begin
  NumWritten := 0;
  DoneOk := IsOpen;
  While ((NumWritten < Num) and (DoneOk)) Do
    Begin
    Tmp := Num - NumWritten;  {num left to write}
    If (CurrPos >= CurrSize) Then
      Begin
      If CurrPos - BufStart + Tmp > BufChars Then
        BufChars := CurrPos - BufStart + Tmp;
      If BufChars > BufSize Then
        BufChars := BufSize;
      End;
    If Tmp > (BufChars - (CurrPos - BufStart)) Then
      Tmp := (BufChars - (CurrPos - BufStart));
    If ((Tmp > 0) and (DoneOk)) Then
      Begin
      Move(FBufType(V)[NumWritten], Buf^[CurrPos - BufStart] , Tmp);
      Inc(NumWritten, Tmp);
      NeedWritten := True;
      End;
    DoneOk := SeekFile(CurrPos + Tmp);
    If DoneOk Then
      Begin
      If BufChars = 0 Then
        Begin
        If Num - NumWritten < BufSize Then
          DoneOk := ReadBuffer
        Else
          BufChars := BufSize;
        End;
      End;
    End;
  BlkWrite := DoneOk;
  End;


Function FFileObj. SeekFile(FP: LongInt): Boolean;
  Var
    DoneOk: Boolean;

  Begin
  DoneOk := IsOpen;
  If (FP < BufStart) or (FP > (BufStart + BufChars - 1)) Then
    Begin {not in buffer}
    If (FP >= BufStart) and (FP < (BufStart + BufSize - 1)) and
    (FP >= CurrSize) Then
      Begin {Out of orig buffer but beyond eof and within bufsize}
      CurrPos := FP;
      If (CurrPos - BufStart) > BufChars Then
        BufChars := CurrPos - BufStart;
      End
    Else
      Begin {write buffer if needed and reposition}
      If (NeedWritten and (BufChars > 0)) Then  {Write old buffer first if needed}
        DoneOk := WriteBuffer;
      BufStart := FP;
      CurrPos := FP;
      BufChars := 0;
      End;
    End
  Else
    Begin  {was within buffer}
    CurrPos := FP;
    End;
  SeekFile := DoneOk;
  End;



Function FFileObj.WriteBuffer: Boolean;
  Var
    DoneOK: Boolean;

  Begin
  If IoResult <> 0 Then;
  DoneOk := shSeekFile(BufFile, BufStart);
  If DoneOk Then
    DoneOk := shWrite(BufFile, Buf^, BufChars); {Write buffer}
  If (BufStart + BufChars - 1) > CurrSize Then
    CurrSize := BufStart + BufChars - 1;
  If DoneOk Then
    NeedWritten := False;              {Turn off needs-written flag}
  WriteBuffer := DoneOk;               {Return result}
  End;


Function FFileObj.ReadBuffer: Boolean;
  Var
    DoneOK: Boolean;

  Begin
  If IoResult <> 0 Then;
  If NeedWritten Then
    DoneOk := WriteBuffer;
  Seek(BufFile, BufStart);
  DoneOk := (ioResult = 0);            {Seek to buffer start first}
  If DoneOk Then
    Begin
    If BufStart >= RawSize Then
      BufChars := 0
    Else
      DoneOk := shRead(BufFile, Buf^, BufSize, BufChars); {Read buffer}
    End;
  ReadBuffer := DoneOk;               {Return result}
  End;


Function FFileObj.RawSize: LongInt;
  Begin
  If IoResult <> 0 Then;
  RawSize := FileSize(BufFile);
  If IoResult <> 0 Then;
  End;


Function FFileObj.FilePos: LongInt;
  Begin
  FilePos := CurrPos;
  End;


End.
