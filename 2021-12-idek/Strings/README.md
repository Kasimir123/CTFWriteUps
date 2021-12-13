# idek Strings Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments: Strings might be helpful but that's not all...

## Write up:

```
00001135  main:
00001135  55                 push    rbp {__saved_rbp}
00001136  4889e5             mov     rbp, rsp {__saved_rbp}
00001139  4883ec30           sub     rsp, 0x30
0000113d  48c745f800000000   mov     qword [rbp-0x8 {var_10}], 0x0
00001145  c745f469646500     mov     dword [rbp-0xc {var_14}], 0x656469
0000114c  c745f06b7b7300     mov     dword [rbp-0x10 {var_18}], 0x737b6b
00001153  c745ec74523100     mov     dword [rbp-0x14 {var_1c}], 0x315274
0000115a  c745e86e347300     mov     dword [rbp-0x18 {var_20}], 0x73346e
00001161  c745e45f375400     mov     dword [rbp-0x1c {var_24}], 0x54375f
00001168  c745e057212100     mov     dword [rbp-0x20 {var_28}], 0x212157
0000116f  c745dc7d000000     mov     dword [rbp-0x24 {var_2c}], 0x7d
00001176  488d3d870e0000     lea     rdi, [rel data_2004]
0000117d  b800000000         mov     eax, 0x0
00001182  e8a9feffff         call    printf
00001187  90                 nop     
00001188  c9                 leave    {__saved_rbp}
00001189  c3                 retn     {__return_addr}
```

Opening up the main function in a disassembler I saw the above instructions. I saw that the program loads some hex values in the ascii range into a value so I checked to see what they equaled:

```
6964656b7b737452326e34735f37545721217d
```

Converting these into ascii I got:

```
idek{stR2n4s_7TW!!}
```