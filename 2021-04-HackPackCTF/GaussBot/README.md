# HackPack GaussBot Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

We have trapped one of Melon Eusk's GaussBots™, given the right key we might get some Dogecoins out of them! But they are being evasive...

## Write up:

This program seemed to load the code in at runtime so I took a memory dump at runtime and found the following function:

```c
void __noreturn sub_565D503C()
{

  v0 = sys_read(0, v12, 0x1Fu);
  strcpy(v11, "flag{pr0gram-inside-4-pr0gram}");
  v2 = v1;
  v3 = v11;
  v4 = 30;
  do
  {
    if ( !v4 )
      break;
    v5 = *v2++ == (unsigned __int8)*v3++;
    --v4;
  }
  while ( v5 );
  if ( !v4 )
  {
    strcpy(v10, "Wait, that is the password for the dogecoin vault... How?\n");
    v6 = sys_write(1, v10, 0x3Au);
    v7 = sys_exit(0);
  }
  strcpy(&v10[28], "I'll never tell you my secrets\n");
  v8 = sys_write(1, &v10[28], 0x1Fu);
  v9 = sys_exit(0);
}
```

The flag was:

```
flag{pr0gram-inside-4-pr0gram}
```