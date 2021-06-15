# Circle City Con Little Mountain Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

I noticed that the binary was packed with UPX so I used upx to unpack it.

The main function is:

```c
int __cdecl __noreturn main(int argc, const char **argv, const char **envp)
{
  int v3;
  int v4;
  int v5;
  int v6; 
  char v7;
  int v8; 

  setabuf(argc, argv, envp);
  while ( 1 )
  {
    puts("Option 0: Guess the number");
    puts("Option 1: Change the number");
    puts("Option 2: Exit");
    _isoc99_scanf((unsigned int)"%d", (unsigned int)&v8, v3, v4, v5, v6, v7);
    ((void (*)(void))funcs[v8])();
  }
}
```

I followed the code into setabuf and saw:

```c
void (__fastcall __noreturn *setabuf())()
{
  void (__fastcall __noreturn *result)();

  srandom(1337LL);
  magic = random();
  funcs[0] = (__int64)a;
  qword_4CC348 = (__int64)b;
  qword_4CC350 = (__int64)c;
  result = d;
  qword_4CC358 = (__int64)d;
  return result;
}
```

I followed the last function pointer to:

```c
void __noreturn d()
{
  char v0; 
  int v1; 
  int v2;
  const char *v3;
  _BYTE *v4; 
  int v5;
  int i; 

  v4 = &unk_49E022;
  v3 = "little_mountain";
  v2 = j_strlen_ifunc(&unk_49E022);
  v1 = j_strlen_ifunc("little_mountain");
  v5 = 0;
  if ( modded == 20 )
  {
    for ( i = 0; i < v2; ++i )
    {
      if ( v5 == v1 )
        v5 = 0;
      v0 = v4[i] ^ v3[v5++];
      write(1LL, &v0, 1LL);
    }
    puts("\n");
  }
  exit(0LL);
}
```

This seemed like the function that prints out the flag so I wrote a little script to print it out:

```python
# initialize variables
v5 = 0
v3 = "little_mountain"
v4 = [0x0A, 0x05, 0x15, 0x13, 0x17, 0x07, 0x6B, 0x0F, 0x16, 0x06, 0x59, 0x47, 0x11, 0x5C, 0x1B, 0x1C, 0x1D, 0x47, 0x1C, 0x01, 0x55, 0x2A, 0x03, 0x58, 0x41, 0x5F, 0x1A, 0x1C]
v1 = len(v3)
v2 = len(v4)

# string to store flag
s = ""

# loop through v2
for i in range(0, v2):
    # reset counter if it reaches the end
	if v5 == v1:
		v5 = 0

    # do the xor
	v0 = v4[i] ^ ord(v3[v5])
	v5 += 1

    # add the character
	s += chr(v0)

# print flag
print(s)
```

When run we get:

```
flag{b4bys73p5upt3hm0un741n}
```