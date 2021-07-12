# RedPwn wstrings Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

Looking at the main function we see:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  wchar_t ws[82]; 
  unsigned __int64 v5; 

  v5 = __readfsqword(0x28u);
  wprintf(U"Welcome to flag checker 1.0.\nGive me a flag> ", argv, envp);
  fgetws(ws, 80, stdin);
  if ( !wcscmp(flag, ws) )
    fputws(U"Correct!", stdout);
  return 0;
}
```

Following to where flag is stored in the binary we see:

```
01 00 02 00 00 00 00 00  66 00 00 00 6C 00 00 00  ........f...l...
61 00 00 00 67 00 00 00  7B 00 00 00 6E 00 00 00  a...g...{...n...
30 00 00 00 74 00 00 00  5F 00 00 00 61 00 00 00  0...t..._...a...
6C 00 00 00 31 00 00 00  5F 00 00 00 73 00 00 00  l...1..._...s...
74 00 00 00 72 00 00 00  31 00 00 00 6E 00 00 00  t...r...1...n...
67 00 00 00 73 00 00 00  5F 00 00 00 61 00 00 00  g...s..._...a...
72 00 00 00 33 00 00 00  5F 00 00 00 73 00 00 00  r...3..._...s...
6B 00 00 00 31 00 00 00  6E 00 00 00 6E 00 00 00  k...1...n...n...
79 00 00 00 7D 00 00 00  00 00 00 00 00 00 00 00  y...}...........
```

And we can see that the flag is:

```
flag{n0t_al1_str1ngs_ar3_sk1nny}
```