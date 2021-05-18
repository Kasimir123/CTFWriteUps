# dCTF Just In Time Write Up

## Details:

Jeopardy style CTF

Points: 300

Category: Reverse Engineering

Comments: Don't fall in (rabbit) holes

## Write up:

I started by decompiling the main function:

```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  const char *v3;
  const char *v4; 
  char src[8]; 
  char *v7;
  char *v8; 
  char *dest; 

  dest = (char *)malloc(8uLL);
  v3 = (const char *)sub_126A(*a2);
  strncpy(dest, v3, 8uLL);
  strcpy(src, "\x1B&8 yegHr($g1bKu{\"f5`N}t#331Nv/%`11F#1");
  v8 = (char *)malloc(0x27uLL);
  strncpy(v8, src, 0x27uLL);
  sub_1372(v8);
  puts("Decryption finished.");
  v7 = (char *)malloc(0x27uLL);
  v4 = (const char *)sub_11C5(src, dest);
  strncpy(v7, v4, 0x27uLL);
  v7 = (char *)sub_11C5(v7, dest);
  sub_1460(v7);
  free(v8);
  free(v7);
  free(dest);
  return 0LL;
}
```

Rather than trying to do their decryption I decided to see if I cuold get the flag with dynamic analysis which worked out. I set a breakpoint on the strncpy line and saw that the flag was:

```
dctf{df77dbe0c407dd4a188e12013ccb009f}
```