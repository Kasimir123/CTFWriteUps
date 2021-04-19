# HackPack Function Pointer Fun Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:



Somehow, by means unbeknowst to us, you have access to Melon Eusk's terminal. But can you crack his password? Rumor is he wrote his own verification algorithm...

nc ctf2021.hackpack.club 10998


## Write up:

I started by decompiling the function, the main function looked like:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int result; 
  bool changed; 
  int i;
  int (*fp)(void);
  char seed[5];
  unsigned __int64 v8;

  v8 = __readfsqword(0x28u);
  setvbuf(_bss_start, 0LL, 2, 0LL);
  *(_DWORD *)seed = 0;
  seed[4] = 0;
  printf("Hello, Mr. Eusk. \nPassword > ");
  __isoc99_scanf("%4s", seed);
  changed = 0;
  for ( i = 0; i <= 3; ++i )
  {
    if ( seed[i] )
      changed = 1;
  }
  if ( !changed )
  {
    puts("You gotta give an input!");
    result = 1;
  }
  else
  {
    fp = pickFunction(seed);
    ((void (__fastcall *)(char *))fp)(seed);
    result = 0;
  }
  return result;
}
```

I noticed the pickFunction function and decided to decompile that as well:

```c
int (*__cdecl pickFunction(char *seed))(void)
{
  char res; 

  res = (seed[2] | seed[3]) & (*seed | seed[1]);
  if ( res == 73 )
    return funTwo;
  if ( res > 0 && res <= 31 )
    return funOne;
  if ( res > 31 && res <= 63 )
    return funThree;
  if ( res <= 63 || res > 95 )
    return funFive;
  return funFour;
}
```

This function took in a char array contianing 4 chars and then used those to pick a function. I then needed to check which function had what I needed. I found out that funcTwo had what I needed:

```c
int __cdecl funTwo()
{
  FILE *fp; 
  char flag[25]; 
  unsigned __int64 v3; 

  v3 = __readfsqword(0x28u);
  fp = fopen("flag", "r");
  fgets(flag, 25, fp);
  puts(flag);
  return 1;
}
```

I now needed to write a script to figure out what 4 characters I should send, I decided to use Z3:

```python
# import z3
from z3 import *

# instantiate solver
s = Solver()

# create the 4 values
a = BitVec(f'a', 8)
b = BitVec(f'b', 8)
c = BitVec(f'c', 8)
d = BitVec(f'd', 8)

# add constraints
s.add(a < 127)
s.add(b < 127)
s.add(c < 127)
s.add(d < 127)
s.add(a > 32)
s.add(b > 32)
s.add(c > 32)
s.add(d > 32)
s.add(((c|d)&(a|b))==73)

# check the solve
print(s.check())
print(s.assertions())
m = s.model()

# print the model
print(m)
```

When run this output:

```
sat
[a < 127,
 b < 127,
 c < 127,
 d < 127,
 a > 32,
 b > 32,
 c > 32,
 d > 32,
 (c | d) & (a | b) == 73]
[b = 48, a = 105, c = 64, d = 73]
```

From this I was able to figure out the four characters to send were:

```
i0@I
```

I then connected to the server and tried my solve:

```
nc ctf2021.hackpack.club 10998

Hello, Mr. Eusk. 
Password > i0@I
flag{c1RcU1t5_R_fUn!2!}
```