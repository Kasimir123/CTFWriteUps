# ShadowCTF unchallenging Write Up

## Details:
Points: 100

Jeopardy style CTF

Category: Reversing

## Write up:

This challenge presented with a 64 Bit ELF LSB shared object. Knowing this CTF I once again just did strings and got the following:

```
$ strings unchallenging 

/lib64/ld-linux-x86-64.so.2
G@W(A
gets
puts
__cxa_finalize
strcmp
__libc_start_main
libc.so.6
GLIBC_2.2.5
_ITM_deregisterTMCloneTable
__gmon_start__
_ITM_registerTMCloneTable
u/UH
[]A\A]A^A_
What is the password?
op3n_se5ame
{Ar@b1an_night5}
...
```

{Ar@b1an_night5} ended up being the flag.