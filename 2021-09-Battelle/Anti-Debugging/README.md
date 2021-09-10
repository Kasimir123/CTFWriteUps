# Battelle Anti-Debugging Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 200

Comments:

```
Heard you liked debugging... Try this ;)

The flag will be revealed when you pass in the correct input
```

## Write up:

Opening the main function in a decompiler I see:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{

  v3 = alloca(16);
  __main();
  patchAttach();
  if ( findAndDetach() != 5 || debugged() == 1 || (unsigned __int8)checkForOlly() )
  {
    printf("Debugger Detected\n");
  }
  else
  {
    lpAddress[0] = (LPVOID)getRetAddress();
    VirtualProtect(lpAddress[0], 0x10u, 4u, &flOldProtect);
    *(_DWORD *)(getRetAddress() + 391) = 36859568;
  }
  v4 = GlobalAlloc(0x42u, 0xA000u);
  v16 = GlobalLock(v4);
  VirtualProtect(v16, 0xA000u, 4u, (PDWORD)lpAddress);
  v5 = (void *)findPointer(v16);
  qmemcpy(v5, (const void *)(getRetAddress() + 25), 0xA000u);
  v6 = (void (__stdcall *)(int))findPointer(v16);
  v6(v11);
  printf("Serial: ");
  fgets(Buffer, 100, (FILE *)__iob[0]._ptr);
  if ( debugged() != 1 && getRetAddress() < (unsigned int)v16 + 0x4000 )
  {
    printf("Checking serial...\n");
    *(_DWORD *)(findPointer(checkPass) + 83) = 42468652;
    v14 = (unsigned __int8 (__cdecl *)(char *))findPointer(checkPass);
    if ( v14(Buffer) )
    {
      qmemcpy(Str, &unk_40303C, 0x2Cu);
      for ( i = 0; ; ++i )
      {
        v7 = strlen(Str);
        if ( i >= v7 )
          break;
        v8 = &Str[i];
        v9 = Str[i] ^ 0x80;
        *v8 = v9;
        *v8 = v9;
      }
      printf("%s\n", Str);
    }
    else
    {
      printf("Serial Incorrect.\n");
    }
    printf("press a key to quit.");
    getchar();
  }
  return 0;
}
```

I wanted to debug this, especially since it's obvious they put lots of antidebugger checks in so I rebased the program and then started nopping out instructions that checked for debugging:

```
0000132A E8 16 03 00 00                                call    _findAndDetach

00001334 E8 1E 02 00 00                                call    _debugged

0000133E E8 C7 02 00 00                                call    _checkForOlly

00001407 FF D0                                         call    eax

0000143C E8 16 01 00 00                                call    _debugged
```

After nopping out these instructions I was able to debug, and by changing the z flag as I went I was able to go into the following:

```c
 if ( v12(Buffer) )
```

This resolved to the following function:

```c
BOOL __cdecl checkPass(char *Str2)
{

  *(_DWORD *)Str = dword_403113;
  v5 = dword_403117;
  v6 = word_40311B;
  v7 = byte_40311D;
  for ( i = 0; i < strlen(Str); ++i )
  {
    Str[i] -= 5;
    Str[i] ^= 0x6Eu;
  }
  v1 = strlen(Str);
  return strncmp(Str, Str2, v1) == 0;
}
```

Str ended up resolving to: youFoundMe.

When I used that in the program I got:

```
Correct. user = part6 pass = unpackedSerial
```

The flag ended up being:

```
unpackedSerial
```