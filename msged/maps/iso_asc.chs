;
; This file is a charset conversion module in text form.
;
; This module converts ISO_8859-1 extended characters to 7 bit ASCII characters
;
; Format: ID, version, level,
;         from charset, to charset,
;         128 entries: first & second byte
;         "END"
; Lines beginning with a ";" or a ";" after the entries are comments
;
; Unkown characters are mapped to the "?" character.
;
; cedilla = ,   ; dieresis = ..       ; acute = '
; grave = `     ; circumflex = ^      ; ring = o
; tilde = ~     ; caron = v
; All of these are above the character, apart from the cedilla which is below.
;
; \ is the escape character: \0 means decimal zero,
; \dnnn where nnn is a decimal number, is the ordinal value of the character
; \xnn where nn is a hexadecimal number
; e.g.: \d32 is the ASCII space character
; Two \\ is the character "\" itself.
;
0               ; ID number
0               ; version number
;
2               ; level number
;
LATIN-1         ; from set
ASCII           ; to set
;
E U             ; (missing) Euro currency sign in Windows ANSI
\0 \d129        ; (missing) For transparency, these are not mapped.
\0 \d130        ; (missing)
\0 \d131        ; (missing) 4
\0 \d132        ; (missing)
\0 \d133        ; (missing)
\0 \d134        ; (missing)
\0 \d135        ; (missing) 8
\0 \d136        ; (missing)
\0 \d137        ; (missing)
\0 \d138        ; (missing)
\0 \d139        ; (missing) 12
\0 \d140        ; (missing)
\0 \d141        ; (missing)
\0 \d142        ; (missing)
\0 \d143        ; (missing) 16
\0 \d144        ; (missing)
\0 \d145        ; (missing)
\0 \d146        ; (missing)
\0 \d147        ; (missing) 20
\0 \d148        ; (missing)
\0 \d149        ; (missing)
\0 \d150        ; (missing)
\0 \d151        ; (missing) 24
\0 \d152        ; (missing)
\0 \d153        ; (missing)
\0 \d154        ; (missing)
\0 \d155        ; (missing) 28
\0 \d156        ; (missing)
\0 \d157        ; (missing)
\0 \d158        ; (missing)
\0 \d159        ; (missing) 32
\0 \x20         ; non-breaking space
\0 !            ; exclam downwards
\0 c            ; cent
\0 L            ; pound sterling
\0 \x0f         ; currency
\0 Y            ; Yen
\0 |            ; broken bar
\0 \x15         ; section
\x1 ?           ; dieresis
\0 c            ; copyright
\0 a            ; ord feminine
< <             ; guillemot left
\0 !            ; logical not
\0 -            ; soft hyphen (or em dash)
\0 R            ; registered trademark
\x1 ?           ; overbar (macron)
\x1 ?           ; ring / degree
+ -             ; plusminus
^ 2             ; superscript two (squared)
^ 3             ; superscript three (cubed)
\0 '            ; acute
m u             ; mu
\0 \x14         ; paragraph
\0 o            ; bullet
\0 ,            ; cedilla
^ 1             ; superscript one
\0 o            ; ord masculine
> >             ; guillemot right
\x1 ?           ; one quarter
. 5             ; half
\x1 ?           ; three quarters
\0 ?            ; question downwards
\0 A            ; A grave
\0 A            ; A acute
\0 A            ; A circumflex
\0 A            ; A tilde
A e             ; A dieresis
\0 A            ; A ring
A E             ; AE
\0 C            ; C cedilla
\0 E            ; E grave
\0 E            ; E acute
\0 E            ; E circumflex
E e             ; E dieresis
\0 I            ; I grave
\0 I            ; I acute
\0 I            ; I circumflex
I e             ; I dieresis
\0 D            ; Eth
\0 N            ; N tilde
\0 O            ; O grave
\0 O            ; O acute
\0 O            ; O circumflex
\0 O            ; O tilde
O e             ; O dieresis
\0 x            ; multiplication
\0 O            ; O slash
\0 U            ; U grave
\0 U            ; U acute
\0 U            ; U circumflex
U e             ; U dieresis
\0 Y            ; Y acute
\x1 ?           ; Thorn
s s             ; german double s / beta
\0 a            ; a grave
\0 a            ; a acute
\0 a            ; a circumflex
\0 a            ; a tilde
a e             ; a dieresis
\0 a            ; a ring
a e             ; ae
\0 c            ; c cedilla
\0 e            ; e grave
\0 e            ; e acute
\0 e            ; e circumflex
e e             ; e dieresis
\0 i            ; i grave
\0 i            ; i acute
\0 i            ; i circumflex
i e             ; i dieresis
\x1 ?           ; eth
\0 n            ; n tilde
\0 o            ; o grave
\0 o            ; o acute
\0 o            ; o circumflex
\0 o            ; o tilde
o e             ; o dieresis
\0 /            ; division
\0 o            ; o slash
\0 o            ; u grave
\0 o            ; u acute
\0 o            ; u circumflex
u e             ; u dieresis
y e             ; y acute
\x1 ?           ; thorn
y e             ; y dieresis
END
