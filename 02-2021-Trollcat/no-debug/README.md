# Trollcat no debug Write Up

## Details:
Points: 500

Jeopardy style CTF

Category: Reversing

## Write up:

This challenge also gave me a 64 bit executable:

```
file crackme

crackme: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=f4599ab6673e44ee040c43849f7af66cd81a3d45, for GNU/Linux 3.2.0, stripped
```

After throwing it into IDA I found the main function:

``` c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  int v3; // eax
  char buf[264]; // [rsp+0h] [rbp-110h] BYREF
  ssize_t v6; // [rsp+108h] [rbp-8h]

  printf("Enter key: ");
  v6 = read(0, buf, 0xFFuLL);
  if ( v6 )
    buf[v6 - 1] = 0;
  if ( (unsigned int)sub_1738(buf, v6 - 1) )
  {
    printf("Congrats here is your flag: ");
    v3 = open("/flag", 0);
    sendfile(1, v3, 0LL, 0x100uLL);
  }
  else
  {
    puts("Invalid key");
  }
  return 0LL;
}
```

However, running the program in a debugger I realized that the program was not getting to this function. Running the program normally I got Invalid key when I input the wrong function however when I was in debug mode I would get Wrong password. I did a string search and found the following function:

``` c
int sub_55DD312E91E5()
{
  int v0; // eax
  char buf[32]; // [rsp+0h] [rbp-130h] BYREF
  __int64 s1[33]; // [rsp+20h] [rbp-110h] BYREF
  int v4; // [rsp+128h] [rbp-8h]
  int fd; // [rsp+12Ch] [rbp-4h]

  s1[0] = 0LL;
  ...
  s1[31] = 0LL;
  fd = open("/dev/urandom", 0);
  read(fd, buf, 0x20uLL);
  close(fd);
  v4 = 0;
  printf("Enter key: ");
  v4 = read(0, s1, 0xFFuLL);
  if ( v4 > 0 )
    *((_BYTE *)s1 + v4 - 1) = 0;
  if ( memcmp(s1, buf, 0x1FuLL) )
    return puts("Wrong password");
  puts("Congrats you are a super eleet hacker, here is your flag: ");
  v0 = open((const char *)s1, 0);
  return sendfile(1, v0, 0LL, 0x100uLL);
}
```

I shortened the string assignment with ... but it sinply iterates from 0-31 assigning 0LL to everything.

Following the xrefs I found a function that called this function:

``` c 
__int64 sub_55DD312E941A()
{
  __int64 result; // rax

  setvbuf(stdin, 0LL, 2, 0LL);
  setvbuf(stdout, 0LL, 2, 0LL);
  setvbuf(stderr, 0LL, 2, 0LL);
  alarm(0x30u);
  result = ptrace(PTRACE_TRACEME, 0LL, 0LL, 0LL);
  if ( result < 0 )
  {
    sub_55DD312E91E5();
    exit(0);
  }
  return result;
}
```

After rebasing the program to 0 and checking out the hex you see:

```
.text:000000000000148C BE 00 00 00 00    mov     esi, 0
.text:0000000000001491 BF 00 00 00 00    mov     edi, 0          ; request
.text:0000000000001496 B8 00 00 00 00    mov     eax, 0
.text:000000000000149B E8 00 FC FF FF    call    _ptrace
.text:00000000000014A0 48 85 C0          test    rax, rax
.text:00000000000014A3 79 14             jns     short loc_14B9
.text:00000000000014A5 B8 00 00 00 00    mov     eax, 0
.text:00000000000014AA E8 36 FD FF FF    call    sub_11E5
.text:00000000000014AF BF 00 00 00 00    mov     edi, 0          ; status
.text:00000000000014B4 E8 27 FC FF FF    call   
```

From here you just needed to NOP the instruction you didn't want, then I put the patched program into IDA and ran through the debugger again, this time it went where I wanted it to.

The function called in main was:

``` c
__int64 __fastcall sub_559D5B597738(__int64 a1, unsigned __int64 a2)
{
  int i; // [rsp+1Ch] [rbp-4h]

  sub_559D5B5974BC((__int64)aWarasg47Npgis9, 0x20uLL);
  sub_559D5B5975F8((__int64)byte_559D5B59A0D0, a2);
  for ( i = 0; a2 > i; ++i )
  {
    if ( *(_BYTE *)(i + a1) != byte_559D5B59A0D0[i] )
      return 0LL;
  }
  return 1LL;
}
```

Normally I would go into those functions to see what was needed but I noticed that the first function didn't depend on user input at all so I just looked a at the second function:


``` c
unsigned __int64 __fastcall sub_561B729415F8(__int64 a1, unsigned __int64 a2)
{
  __int64 v2; // kr00_8
  __int64 v3; // kr08_8
  unsigned __int64 result; // rax
  char v5; // [rsp+1Bh] [rbp-15h]
  unsigned __int64 i; // [rsp+20h] [rbp-10h]
  int v7; // [rsp+28h] [rbp-8h]
  int v8; // [rsp+2Ch] [rbp-4h]

  v8 = 0;
  v7 = 0;
  for ( i = 0LL; ; ++i )
  {
    result = i;
    if ( i >= a2 )
      break;
    v8 = (v8 + 1) % 256;
    v2 = *(char *)(qword_561B72944130 + v8) + v7;
    v7 = (unsigned __int8)(HIBYTE(v2) + *(_BYTE *)(qword_561B72944130 + v8) + v7) - HIBYTE(HIDWORD(v2));
    v5 = *(_BYTE *)(qword_561B72944130 + v8);
    *(_BYTE *)(qword_561B72944130 + v8) = *(_BYTE *)(qword_561B72944130 + v7);
    *(_BYTE *)(v7 + qword_561B72944130) = v5;
    v3 = *(char *)(qword_561B72944130 + v8) + *(char *)(qword_561B72944130 + v7);
    *(_BYTE *)(a1 + i) ^= *(_BYTE *)(qword_561B72944130
                                   + (unsigned __int8)(HIBYTE(v3)
                                                     + *(_BYTE *)(qword_561B72944130 + v8)
                                                     + *(_BYTE *)(qword_561B72944130 + v7))
                                   - HIBYTE(HIDWORD(v3)));
  }
  return result;
}
```

This used the length of the input to iterate through, so I just put in 32 a's and then I simply stepped to the for loop and looked at what byte_559D5B59A0D0 contained since that was being compared with the string.

Using lazy ida I got that the string was:

``` python
[0x64, 0x75, 0x6D, 0x62, 0x68, 0x61, 0x63, 0x6B, 0x65, 0x72, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 0x00]
```

I then made a small python script to print that out:

``` python
key = [0x64, 0x75, 0x6D, 0x62, 0x68, 0x61, 0x63, 0x6B, 0x65, 0x72, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 0x00]

s = ""

for i in key:  
    s += chr(i)

print(s)
```

This printed out:

```
dumbhackerpassword
```

I then used this key:

```
nc 157.230.33.195 3333

Enter key: dumbhackerpassword
Congrats here is your flag: Trollcat{y0u_ptr4c3d_m3}
```