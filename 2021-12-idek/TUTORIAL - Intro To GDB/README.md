# idek TUTORIAL - Intro To GDB Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments: Crash course into dynamic analysis of linux binaries with GDB For experienced reverse engineers, just skip the walkthrough since this challenge should be no problem for you, otherwise, please read the walkthrough if you get stuck.

## Write up:

Loading the program into the decompiler I saw the below main function:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  puts("Hey, check out my new password checker! :D");
  __isoc99_scanf("%s", v5);
  strcpy(v4, "idek{m0m_g3t_th3_c4m3rA!}");
  v4[26] = 0;
  for ( i = 0; i <= 25; ++i )
  {
    if ( (unsigned __int8)v5[i] - (unsigned __int8)v4[i] )
    {
      printf("Nope, that's not right...");
      exit(0);
    }
  }
  return printf("GGs, you got it!");
}
```

The flag ended up just being:

```
idek{m0m_g3t_th3_c4m3rA!}
```