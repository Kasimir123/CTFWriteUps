# picoCTF Hurry up! Wait! Write Up

## Details:
Points: 100

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

This challenge annoyed me a little since it used an older version of libgnat which I had to download and set the path to include. After having done that I started reversing the file and found the following function:

```c
__int64 sub_298A()
{
  ada__calendar__delays__delay_for(1000000000000000LL);
  sub_2616();
  sub_24AA();
  sub_2372();
  sub_25E2();
  sub_2852();
  sub_2886();
  sub_28BA();
  sub_2922();
  sub_23A6();
  sub_2136();
  sub_2206();
  sub_230A();
  sub_2206();
  sub_257A();
  sub_28EE();
  sub_240E();
  sub_26E6();
  sub_2782();
  sub_28EE();
  sub_23A6();
  sub_240E();
  sub_233E();
  sub_23A6();
  sub_2372();
  sub_2206();
  sub_23A6();
  return sub_2956();
}
```

I then looked at the opcodes and address for the time and function:

```
.text:000000000000298E 48 BF 00 80 C6 A4 7E 8D 03 00                 mov     rdi, 38D7EA4C68000h
.text:0000000000002998 E8 D3 F1 FF FF                                call    _ada__calendar__delays__delay_for
``` 

And set all of the values for the number, 00 - 00, to 00 and ran the new file:

```
picoCTF{d15a5m_ftw_dfbdc5d}
```