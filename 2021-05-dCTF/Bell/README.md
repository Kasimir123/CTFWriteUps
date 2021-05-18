# dCTF Bell Write Up

## Details:

Jeopardy style CTF

Points: 100

Category: Reverse Engineering

Comments:

Blaise's friends like triangles too!
nc dctf-chall-bell.westeurope.azurecontainer.io 5311

## Write up:

Decompiling the program I got that the main function was:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  unsigned int v3; 
  unsigned int v5;

  v3 = time(0LL);
  srand(v3);
  v5 = rand() % 5 + 8;
  printf("%d\n", v5);
  process(v5);
  return 0;
}
```

I saw that what I needed was inside of process so I decompiled that too:

```c
__int64 __fastcall process(int a1)
{
  int v2;
  int i; 
  __int64 v4;
  __int64 v5; 
  unsigned __int64 v6;

  v6 = __readfsqword(0x28u);
  v2 = 1;
  for ( i = 1; i <= a1; ++i )
  {
    v5 = triangle((unsigned int)a1, (unsigned int)i);
    __isoc99_scanf(&unk_AA4, &v4);
    if ( v5 != v4 )
      v2 = 0;
  }
  if ( v2 == 1 )
    system("cat flag.txt");
  else
    puts("Better luck next time.");
  return 0LL;
}
```

From the looks of the code I looped through whatever random number of times and then had to enter the number that triangle would spit out. I then decompiled triangle:

```c
__int64 __fastcall triangle(unsigned int a1, int a2)
{
  __int64 v3; 

  if ( a2 > (int)a1 )
    return 0LL;
  if ( a1 == 1 && a2 == 1 )
    return 1LL;
  if ( a2 == 1 )
    return triangle(a1 - 1, a1 - 1);
  v3 = triangle(a1, (unsigned int)(a2 - 1));
  return v3 + triangle(a1 - 1, (unsigned int)(a2 - 1));
}
```

This was just a function that did some math so I decided to make a python script to do the same:

```py
# import pwn 
from pwn import *

# open remote connection
r = remote("dctf-chall-bell.westeurope.azurecontainer.io", 5311)

# get the first line
num = r.recvline()

# turn the line into a number
num = int(num.decode().replace("\n", ""))

# triangle function
def triangle(a1, a2):
	if a2 > a1:
		return 0
	if a1 == 1 and a2 == 1:
		return 1
	if a2 == 1:
		return triangle(a1-1, a1-1)
	v3 = triangle(a1, a2 -1)
	return v3 + triangle(a1-1, a2-1)

# loop through X amount of times
for i in range(1, num + 1):

    # send the calculated value
	r.sendline(str(triangle(num, i)))

# print the flag
print(r.recvline())
```

When run I got:

```
[+] Opening connection to dctf-chall-bell.westeurope.azurecontainer.io on port 5311: Done
b'dctf{f1rst_step_t0wards_b3ll_l4bs}\n'
```