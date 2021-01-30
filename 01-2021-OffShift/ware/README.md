# Offshift ware Write Up

## Details:
Points: 500

Jeopardy style CTF

Category: Reversing

Comment: My plaintext has been encrypted by an innocent friend of mine while playing around cryptographic libraries, can you help me to recover the plaintext , remembers it's just numbers and there's a space between some numbers which you need to remove the space and submit the recovered plain text as a flag.
## Write up:

As I do with most reversing challenges I started by running strings on the file. This resulted in a result that contained:

```
5U/3Q
mU64 
X(%MPWA
/usr/
/go-1.6/
-gdb
UPX!
UPX!
(x8#
```

Seeing the UPX let me realize that I would need to unpack using UPX before starting to reverse.

```
UPX -d skidw4re
```

After unpacking I ran file and found that it was a 32 bit program.

```
$ file skidw4re  

skidw4re: ELF 32-bit LSB executable, Intel 80386, version 1 (GNU/Linux), statically linked, no section header
```

I then ran the program to see what the output was:

```
$ ./skidw4re  

This is the only message--------> ae385c6f1dd72132b2afcd4c25b9d35e0000000000000000
32 The message has been encrypted and written
```

I then loaded the program into IDA.

``` c
void __cdecl main_main()
{
  os_File *v0; // eax
  uintptr v1; // eax
  interface_{} *v2; // ebx
  uintptr v3; // eax
  interface_{} *v4; // ebx
  uintptr v5; // eax
  interface_{} *v6; // ebx
  uintptr v7; // eax
  interface_{} *v8; // ebx
  int v9; // eax
  _BYTE name[24]; // [esp+0h] [ebp-5Ch] BYREF
  int elem; // [esp+18h] [ebp-44h] BYREF
  os_File *f; // [esp+1Ch] [ebp-40h]
  error_0 err; // [esp+20h] [ebp-3Ch]
  int v14; // [esp+28h] [ebp-34h]
  uintptr v15; // [esp+2Ch] [ebp-30h]
  int v16; // [esp+30h] [ebp-2Ch] BYREF
  int v17; // [esp+34h] [ebp-28h]
  int v18[2]; // [esp+38h] [ebp-24h] BYREF
  __interface_{} a; // [esp+40h] [ebp-1Ch]
  int v20[4]; // [esp+4Ch] [ebp-10h] BYREF
  void *retaddr; // [esp+5Ch] [ebp+0h] BYREF

  while ( (unsigned int)&retaddr <= *(_DWORD *)(*(_DWORD *)(__readgsdword(0) - 4) + 8) )
    runtime_morestack_noctxt();
  main_Encryptfinal();
  *(_DWORD *)name = "encryptedmessage.txt";
  *(_DWORD *)&name[4] = 20;
  os_Create(*(string *)name, *(os_File **)&name[8], *(error_0 *)&name[12]);
  v0 = *(os_File **)&name[8];
  err = *(error_0 *)&name[12];
  if ( *(_DWORD *)&name[12] )
  {
    v16 = 0;
    v17 = 0;
    if ( name == (_BYTE *)-48 )
      v16 = *(_DWORD *)&name[8];
    a.len = 1;
    a.cap = 1;
...
```

Looking over the top of the main function I noticed the main_Encryptfinal method and decided to click on it.

``` c
void main_Encryptfinal()
{
  int v0; // eax
  uintptr v1; // eax
  interface_{} *v2; // ebx
  _BYTE buf[28]; // [esp+0h] [ebp-84h] BYREF
  uint8 v4[32]; // [esp+1Ch] [ebp-68h] BYREF
  string pt; // [esp+3Ch] [ebp-48h]
  string key; // [esp+44h] [ebp-40h]
  string c; // [esp+4Ch] [ebp-38h]
  int v8; // [esp+54h] [ebp-30h]
  uintptr v9; // [esp+58h] [ebp-2Ch]
  int v10[2]; // [esp+5Ch] [ebp-28h] BYREF
  int elem[2]; // [esp+64h] [ebp-20h] BYREF
  __interface_{} a; // [esp+6Ch] [ebp-18h]
  int v13; // [esp+78h] [ebp-Ch]
  int v14; // [esp+7Ch] [ebp-8h]
  int v15; // [esp+80h] [ebp-4h] BYREF

  while ( (unsigned int)&v15 <= *(_DWORD *)(*(_DWORD *)(__readgsdword(0) - 4) + 8) )
    runtime_morestack_noctxt();
  pt.str = (uint8 *)"321174068998067 98980909";
  pt.len = 24;
  key.str = (uint8 *)"thisis32bitlongpassphraseimusing";
  *(_DWORD *)&buf[4] = "thisis32bitlongpassphraseimusing";
  key.len = 32;
  *(_DWORD *)&buf[8] = 32;
  runtime_stringtoslicebyte((uint8 (*)[32])v4, *(string *)&buf[4], *(__uint8 *)&buf[12]);
  v13 = *(_DWORD *)&buf[12];
  *(_DWORD *)buf = *(_DWORD *)&buf[12];
  v14 = *(_DWORD *)&buf[16];
  *(_DWORD *)&buf[4] = *(_DWORD *)&buf[16];
  v15 = *(_DWORD *)&buf[20];
  *(_DWORD *)&buf[8] = *(_DWORD *)&buf[20];
  main_EncryptAES(*(__uint8 *)buf, pt, *(string *)&buf[20]);
  *(_DWORD *)&buf[8] = 34;
  c = *(string *)&buf[20];
  *(_DWORD *)&buf[12] = *(_DWORD *)&buf[20];
  *(_DWORD *)&buf[16] = *(_DWORD *)&buf[24];
  runtime_concatstring2(0, (string *)"This is the only message--------> ", *(string *)&buf[8]);
  elem[0] = *(_DWORD *)&buf[20];
  elem[1] = *(_DWORD *)&buf[24];
  v10[0] = 0;
  v10[1] = 0;
  if ( buf == (_BYTE *)-92 )
    v10[0] = v0;
  a.len = 1;
  a.cap = 1;
  a.array = (interface_{} *)v10;
  runtime_convT2E((runtime__type_0 *)&stru_80F1F00, elem, 0, *(runtime_eface_0 *)&buf[12]);
  v1 = *(_DWORD *)&buf[16];
  v2 = a.array;
  v8 = *(_DWORD *)&buf[12];
  a.array->_type = *(runtime__type_0 **)&buf[12];
  v9 = v1;
  if ( runtime_writeBarrier.enabled )
    runtime_writebarrierptr((uintptr *)&v2->data, v1);
  else
    v2->data = (void *)v1;
  fmt_Println(a, *(__int32 *)&buf[12], *(error_0 *)&buf[16]);
}
```

Here I noticed pt.str, since this was an encryption function I assumed pt stood for plain text, and the string matched the format the comment specified.

The flag ended up being:

```
flag{32117406899806798980909}
```