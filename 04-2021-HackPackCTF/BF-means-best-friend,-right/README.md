# HackPack BF means best friend, right? Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

We only got part of Melon Eusk's program that prints out their password. Looks like it creates the right string in memory but does not print, see if you can figure out what it is (wrap the string in flag{}).

## Write up:

Looking at the file I could tell that the code was BrainFuck. I then looked up an online BF runner, ran the code, and looked at the memory dump:

```
00000:  098  114  097  105  110  045  098  108  097  115  116  000  brain-blast.
```

The flag was:

```
flag{brain-blast}
```