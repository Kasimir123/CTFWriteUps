# UMDCTF Painting Windows Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

I am trying to paint these windows; however, there seems to be some sort of password blocking my way to them. Can you help me figure out what the password is?

## Write up:

I started by decompiling the main function:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int result; 
  int v4; 
  __int64 v5;
  __int64 v6; 
  __int64 v7;
  int v8; 
  char *v9;
  char String[512];

  if ( IsDebuggerPresent() )
  {
    sub_140001020("That is not allowed!\n");
    result = 1;
  }
  else
  {
    sub_140001020("What is the password?\n");
    sub_140001080("%s");
    v4 = strnlen(String, 0x100ui64);
    v5 = 0i64;
    v6 = v4;
    if ( v4 <= 0 )
    {
      v8 = 0;
    }
    else
    {
      v7 = 0i64;
      do
      {
        String[v7 + 256] = 2 * (String[v7] ^ 0xF);
        ++v7;
      }
      while ( v7 < v6 );
      v8 = 0;
      do
      {
        if ( String[v5 + 256] != byte_1400022D0[v5] )
          v8 = 1;
        ++v5;
      }
      while ( v5 < v6 );
    }
    v9 = "Failed to unlock the Windows\n";
    if ( !v8 )
      v9 = "Successfully unlocked the Windows!\n";
    sub_140001020(v9);
    result = 0;
  }
  return result;
}
```

I extracted byte_1400022D0 from the executable:

```python
flag = [0xB4, 0x84, 0x96, 0x98, 0xB6, 0x92, 0x44, 0xE8, 0xAC, 0x7E, 0xB4, 0xA0, 0xB8, 0xF6, 0xDC, 0xFA, 0xF6, 0x78, 0x96, 0xA0, 0xEC, 0x80, 0xF4, 0xBA, 0xA0, 0xB0, 0x7C, 0xC2, 0xD6, 0x7E, 0xF0, 0xB8, 0xA0, 0x8A, 0x7E, 0xB4, 0xBA, 0x82, 0xD4, 0xAC, 0xE4, 0x00, 0x00, 0x00]
```

Then I made a script to go through and xor everything by 0xF:

```python
# flag
flag = [0xB4, 0x84, 0x96, 0x98, 0xB6, 0x92, 0x44, 0xE8, 0xAC, 0x7E, 0xB4, 0xA0, 0xB8, 0xF6, 0xDC, 0xFA, 0xF6, 0x78, 0x96, 0xA0, 0xEC, 0x80, 0xF4, 0xBA, 0xA0, 0xB0, 0x7C, 0xC2, 0xD6, 0x7E, 0xF0, 0xB8, 0xA0, 0x8A, 0x7E, 0xB4, 0xBA, 0x82, 0xD4, 0xAC, 0xE4, 0x00, 0x00, 0x00]

s = ""

# decrypt
for i in flag:
	s += chr(int((i/2))^0xF)
print(s)
```

When run I got:

```
UMDCTF-{Y0U_Start3D_yOuR_W1nd0wS_J0URNeY}
```