# picoCTF wave a flag Write Up

## Details:
Points: 10

Jeopardy style CTF

Category: General Skills

Comments:

Can you invoke help flags for a tool or binary? This program has extraordinarily helpful information...

## Write up:

Since I am a hacker I didn't feel like following the rules, rather than trying to follow the guidance I simply tried using strings and grep to search for the flag:

```
strings warm | grep pico

Oh, help? I actually don't do much, but I do have this flag here: picoCTF{b1scu1ts_4nd_gr4vy_755f3544}
```