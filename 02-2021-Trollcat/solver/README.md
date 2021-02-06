# Trollcat solver Write Up

## Details:
Points: 500

Jeopardy style CTF

Category: Reversing

## Write up:

This challenge gave me a 64 bit executable file:

```
file crackme\ \(2\) 

crackme (2): ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=6c8f4137ce00dd9571d3b551dc81d8d4354e4d91, for GNU/Linux 3.2.0, stripped
```

I then put the file into IDA and found the main function:

``` c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  int v3; // eax
  char buf[264]; // [rsp+0h] [rbp-110h] BYREF
  ssize_t v6; // [rsp+108h] [rbp-8h]

  printf("Enter key: ");
  v6 = read(0, buf, 0xFFuLL);
  buf[v6 - 1] = 0;
  if ( sub_5585577D6200(buf, v6 - 1) )
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

The program read in a value and then checked it with sub_5585577D6200, this function looked like:

``` c
_BOOL8 __fastcall sub_5585577D6200(char *a1, unsigned __int64 a2)
{
  int i; // [rsp+1Ch] [rbp-4h]

  for ( i = 0; a2 > i; ++i )
  {
    if ( a1[i] <= 47 || a1[i] > 122 )
      return 0LL;
  }
  if ( *a1 + a1[3] != 100 )
    return 0LL;
  if ( a1[1] + a1[18] != 214 )
    return 0LL;
  if ( a1[2] + a1[4] != 178 )
    return 0LL;
  if ( ((unsigned __int8)a1[5] ^ (unsigned __int8)a1[6]) != 76 )
    return 0LL;
  if ( a1[8] - a1[7] != 17 )
    return 0LL;
  if ( a1[10] - a1[9] != 59 )
    return 0LL;
  if ( a1[12] + a1[11] - a1[13] != 69 )
    return 0LL;
  if ( a1[15] + a1[14] - a1[16] != 31 )
    return 0LL;
  if ( a1[16] + a1[17] - a1[18] == 88 )
    return ((unsigned __int8)(a1[20] ^ a1[19]) ^ (unsigned __int8)a1[21]) == 69;
  return 0LL;
}
```

From this I knew that I would need a string that was 22 characters long and matched the compares, I decided to use the z3 library to create a python script:

``` python
from z3 import *

# Creates solver
s = Solver()

# Creates an array of variables to solve for
flag = [BitVec(f"flag_{i}", 8) for i in range(0, 22)]

# checks that variables are in range
for i in range(0, 22):
    s.add(flag[i] >= 48)
    s.add(flag[i] <= 122)

# adds all the checks
s.add(flag[0] + flag[3] == 100)
s.add(flag[1] + flag[18] == 214)
s.add(flag[2] + flag[4] == 178)
s.add(flag[5] ^ flag[6] == 76)
s.add(flag[8] - flag[7] == 17)
s.add(flag[10] - flag[9] == 59)
s.add(flag[12] + flag[11] - flag[13] == 69)
s.add(flag[15] + flag[14] - flag[16] == 31)
s.add(flag[16] + flag[17] - flag[18] == 88)
s.add((flag[20] ^ flag[19]) ^ flag[21] == 69)

# print if we were able to solve or not
print(s.check())

# gets the variables
m = s.model()

# initializes an empty dictionary
t = {}

# parses the model to dictionary
for a in str(m)[1:-1].split(','):
    t[a.split('=')[0].strip()] = a.split('=')[1].strip()

# creates the string from the variables
s =""
for i in [BitVec(f"flag_{i}", 8) for i in range(0, 22)]:
    s += chr(int(t[str(i)]))

# prints the string
print(s)
```

Running the script I got:

```
sat
4wj0Hp<9J9ty=qI?iN_gXz
```

I then entered this key through netcat and got the flag:

```
nc 157.230.33.195 4444   

Enter key: 4wj0Hp<9J9ty=qI?iN_gXz
Congrats here is your flag: Trollcat{z3_b4by}
```