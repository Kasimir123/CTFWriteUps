# ShadowCTF thirsty cow Write Up

## Details:
Points: 300

Jeopardy style CTF

Category: Reversing

## Write up:

The challenge gave us an ELF 64 bit LSB shared object. I tried running strings and found nothing useful so I opened the file in IDA and got the following pseudocode.

``` c++
int __cdecl main(int argc, const char **argv, const char **envp)
{
  __int64 v3; // rax
  __int64 v4; // rax
  __int64 v5; // rax
  __int64 v6; // rax
  __int64 v7; // rax
  __int64 v8; // rax
  __int64 v9; // rax
  __int64 v10; // rax
  char v12[36]; // [rsp+0h] [rbp-170h] BYREF
  char v13[9]; // [rsp+24h] [rbp-14Ch] BYREF
  char v14[3]; // [rsp+2Dh] [rbp-143h] BYREF
  char v15[4]; // [rsp+30h] [rbp-140h] BYREF
  char v16[10]; // [rsp+34h] [rbp-13Ch] BYREF
  char v17[11]; // [rsp+3Eh] [rbp-132h] BYREF
  char v18[4]; // [rsp+49h] [rbp-127h] BYREF
  char v19[3]; // [rsp+4Dh] [rbp-123h] BYREF
  char v20[4]; // [rsp+50h] [rbp-120h] BYREF
  char v21[4]; // [rsp+54h] [rbp-11Ch] BYREF
  char v22[4]; // [rsp+58h] [rbp-118h] BYREF
  char v23[4]; // [rsp+5Ch] [rbp-114h] BYREF
  char v24[268]; // [rsp+60h] [rbp-110h] BYREF

  strcpy(v23, "_si");
  strcpy(v22, "sin");
  strcpy(v21, "_5i");
  strcpy(v20, "rty");
  strcpy(v19, "th");
  strcpy(v18, "x_r");
  strcpy(&v17[7], "_th");
  strcpy(&v17[5], "x");
  strcpy(v17, "irty");
  strcpy(v16, v19);
  strcat(v16, v17);
  v3 = std::operator<<<std::char_traits<char>>(
         &std::cout,
         " The crow is thirsty and he needs your help to gather stones to fill the pot");
  v4 = std::ostream::operator<<(v3, &std::endl<char,std::char_traits<char>>);
  std::operator<<<std::char_traits<char>>(v4, " > ");
  std::operator>><char,std::char_traits<char>>(&std::cin, v24);
  strcat(v16, v23);
  strcat(v16, &v17[5]);
  if ( strcmp(v24, v16) )
  {
    v10 = std::operator<<<std::char_traits<char>>(&std::cout, " Not enough stones :( ");
    std::ostream::operator<<(v10, &std::endl<char,std::char_traits<char>>);
  }
  else
  {
    strcpy(v15, "Thi");
    strcpy(v14, "e_");
    strcpy(&v13[6], "ck");
    strcpy(&v13[4], "0");
    strcpy(v13, "p0t");
    strcpy(v12, v15);
    strcat(v12, v20);
    strcat(v12, v21);
    strcat(v12, v18);
    strcat(v12, &v13[4]);
    strcat(v12, &v13[6]);
    strcat(v12, v22);
    strcat(v12, &v17[7]);
    strcat(v12, v14);
    strcat(v12, v13);
    v5 = std::ostream::operator<<(&std::cout, &std::endl<char,std::char_traits<char>>);
    v6 = std::operator<<<std::char_traits<char>>(v5, "shadowCTF{");
    v7 = std::operator<<<std::char_traits<char>>(v6, v12);
    v8 = std::operator<<<std::char_traits<char>>(v7, "} ");
    v9 = std::ostream::operator<<(v8, &std::endl<char,std::char_traits<char>>);
    std::ostream::operator<<(v9, &std::endl<char,std::char_traits<char>>);
  }
  return 0;
}
```

Seeing this I knew that I would simply be able to run the file in the debugger and change the value of eax to make the code go into the else statement.

I started my linux debugger and set a breakpoint on the if statement. When prompted for the keyphrase I put in a random string.

```
.text:00005572417C22BC mov     rsi, rdx                        ; src
.text:00005572417C22BF mov     rdi, rax                        ; dest
.text:00005572417C22C2 call    _strcat
.text:00005572417C22C7 lea     rdx, [rbp+dest+7]
.text:00005572417C22CE lea     rax, [rbp+s1]
.text:00005572417C22D5 mov     rsi, rdx                        ; s2
.text:00005572417C22D8 mov     rdi, rax                        ; s1
.text:00005572417C22DB call    _strcmp
.text:00005572417C22E0 mov     [rbp+var_4], eax
.text:00005572417C22E3 cmp     [rbp+var_4], 0
.text:00005572417C22E7 jnz     loc_5572417C2499
.text:00005572417C22ED mov     dword ptr [rbp+dest+3], 696854h
.text:00005572417C22F7 mov     word ptr [rbp+dest], 5F65h
.text:00005572417C2300 mov     [rbp+dest+2], 0
```
I noticed that after the strcmp call the value of eax was put into rbp+var_4 so I stepped to the mov instruction and changed the eax value to 0 and pressed continue. The flag was then printed.

```
 The crow is thirsty and he needs your help to gather stones to fill the pot
 > sdffsd

shadowCTF{Thirty_5ix_r0cksin_the_p0t} 
```