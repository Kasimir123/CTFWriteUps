# dvCTF rocca pia Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: reverse engineering

## Write up:

First I checked what type of file this was, it was a 64 bit elf file, so I went and opened it in a decompiler.

The main function was:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int result; // eax

  if ( argc == 2 )
  {
    if ( (unsigned int)transform(argv[1], argv, envp) )
      puts("Nice try");
    else
      puts("Nice flag");
    result = 0;
  }
  else
  {
    printf("Usage: %s <password>\n", *argv);
    result = 1;
  }
  return result;
}
```

It was fairly obvious that I would need to look into the transform function so thats where I started off.

```c
int __fastcall transform(__int64 a1)
{
  int i; // [rsp+14h] [rbp-1Ch]
  char *s2; // [rsp+18h] [rbp-18h]

  for ( i = 0; i < strlen(PASSWD); ++i )
  {
    if ( (i & 1) != 0 )
      s2[i] = *(_BYTE *)(i + a1) ^ 0x37;
    else
      s2[i] = *(_BYTE *)(i + a1) ^ 0x13;
  }
  return strncmp(PASSWD, s2, 0x16uLL);
}
```

This function took the input and then xor'ed it to get a new string, that string was then compared against PASSWD. Looking into the file data I saw that the passwd string was:

```python
[0x77, 0x41, 0x50, 0x63, 0x55, 0x4C, 0x5A, 0x68, 0x7F, 0x06, 0x78, 0x04, 0x4C, 0x44, 0x64, 0x06, 0x7E, 0x5A, 0x22, 0x59, 0x74, 0x4A]
```

I then wrote a python script to work backwards from the passwd string to get the output:

```python
# passwd string bytes
passw = [0x77, 0x41, 0x50, 0x63, 0x55, 0x4C, 0x5A, 0x68, 0x7F, 0x06, 0x78, 0x04, 0x4C, 0x44, 0x64, 0x06, 0x7E, 0x5A, 0x22, 0x59, 0x74, 0x4A]

# empty flag to write to
flag = ""

# loop through length of passwd
for x in range(0, len(passw)):
    # if the current int anded with 1 is not 0
	if (x&1)!=0:
        # xor with 0x37
		flag += chr(passw[x]^0x37)
	else:
        # else xor with 0x13
		flag += chr(passw[x]^0x13)

# print the flag
print(flag)
```

Once run the script returned:

```
dvCTF{I_l1k3_sw1mm1ng}
```