# RaRCTF RaRPG Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 200

Comments: 

Challenge instance ready at 193.57.159.27:40052. Click here to request a new instance.
I've been building a brand new Massively(?) Multiplayer(?) Online Role-Playing(?) Game(?) - try it out! Just don't try and visit the secret dev room...

If you do not have the linked libraries, download rarpg-libs.zip and extract the libraries into your working directory. Then use
LD_LIBRARY_PATH=$(pwd) ./client <ip> <port>

## Write up:

Opening up the game and walking around a bit I saw that there was a door behind some walls:

```
│                X                                                                                  │
│                                                                                                   │
│         ~~~~~~~~~~~~~~~      WWWWWW                                                               │
│X        ~~~~~~~~~~~~~~~   *  W   XW                                                               │
│         ~~~~~~~~~~~~~~~      WWWWWW                                                               │
│                                                                                                   │
│                X                                                                                            
```

From this I knew that I would most likely need to overwrite some sort of move check so I looked into the function names for something along the lines of move and found the following function:

```c
_BOOL8 __fastcall move_okay(int a1, int a2)
{
  bool v3; 
  int v4; 

  if ( wmove(stdscr__NCURSES6_TINFO_5_0_19991023, a1, a2) == -1 )
    v4 = -1;
  else
    v4 = winch(stdscr__NCURSES6_TINFO_5_0_19991023);
  v3 = 1; // set move to ok
  if ( v4 != 32 ) // if not an empty space we check if its a door or wall
    v3 = v4 == 88; // set move to not ok
  return v3;
}
```

While running a dynamic analysis I edited the registers so that it wouldn't set the check to false however that took too much time so the server locked me out. I then decided to edit the binary to remove the check for if we hit a wall. For this I took a look at the assembly:

```
.text:0000000000004970 55                                            push    rbp
.text:0000000000004971 48 89 E5                                      mov     rbp, rsp
.text:0000000000004974 48 83 EC 20                                   sub     rsp, 20h
.text:0000000000004978 89 7D FC                                      mov     [rbp+var_4], edi
.text:000000000000497B 89 75 F8                                      mov     [rbp+var_8], esi
.text:000000000000497E 48 8B 3C 25 A0 D4 41 00                       mov     rdi, 41D4A0h    ; WINDOW *
.text:0000000000004986 8B 75 FC                                      mov     esi, [rbp+var_4] ; int
.text:0000000000004989 8B 55 F8                                      mov     edx, [rbp+var_8] ; int
.text:000000000000498C E8 5F FA FF FF                                call    _wmove
.text:0000000000004991 83 F8 FF                                      cmp     eax, 0FFFFFFFFh
.text:0000000000004994 0F 85 0D 00 00 00                             jnz     loc_49A7
.text:000000000000499A B8 FF FF FF FF                                mov     eax, 0FFFFFFFFh
.text:000000000000499F 89 45 F0                                      mov     [rbp+var_10], eax
.text:00000000000049A2 E9 10 00 00 00                                jmp     loc_49B7
.text:00000000000049A7                               ; ---------------------------------------------------------------------------
.text:00000000000049A7
.text:00000000000049A7                               loc_49A7:                               ; CODE XREF: move_okay(int,int)+24↑j
.text:00000000000049A7 48 8B 3C 25 A0 D4 41 00                       mov     rdi, 41D4A0h    ; WINDOW *
.text:00000000000049AF E8 3C F7 FF FF                                call    _winch
.text:00000000000049B4 89 45 F0                                      mov     [rbp+var_10], eax
.text:00000000000049B7
.text:00000000000049B7                               loc_49B7:                               ; CODE XREF: move_okay(int,int)+32↑j
.text:00000000000049B7 8B 45 F0                                      mov     eax, [rbp+var_10]
.text:00000000000049BA 89 45 F4                                      mov     [rbp+var_C], eax
.text:00000000000049BD 83 7D F4 20                                   cmp     [rbp+var_C], 20h ; ' '
.text:00000000000049C1 B1 01                                         mov     cl, 1
.text:00000000000049C3 88 4D EF                                      mov     [rbp+var_11], cl
.text:00000000000049C6 0F 84 0A 00 00 00                             jz      loc_49D6
.text:00000000000049CC 83 7D F4 58                                   cmp     [rbp+var_C], 58h ; 'X'
.text:00000000000049D0 0F 94 C0                                      setz    al
.text:00000000000049D3 88 45 EF                                      mov     [rbp+var_11], al
.text:00000000000049D6
.text:00000000000049D6                               loc_49D6:                               ; CODE XREF: move_okay(int,int)+56↑j
.text:00000000000049D6 8A 45 EF                                      mov     al, [rbp+var_11]
.text:00000000000049D9 24 01                                         and     al, 1
.text:00000000000049DB 0F B6 C0                                      movzx   eax, al
.text:00000000000049DE 48 83 C4 20                                   add     rsp, 20h
.text:00000000000049E2 5D                                            pop     rbp
.text:00000000000049E3 C3                                            retn
```

I then used a hex editor and changed the follow bytes to all be 0x90:

```
.text:00000000000049CC 83 7D F4 58                                   cmp     [rbp+var_C], 58h ; 'X'
.text:00000000000049D0 0F 94 C0                                      setz    al
.text:00000000000049D3 88 45 EF                                      mov     [rbp+var_11], al
```

When I ran the game I was able to go through the wall and go to the flag which was:

```
rarctf{g4m3_h4ck1ng_f0r_n00b5!-75f8b0}
```