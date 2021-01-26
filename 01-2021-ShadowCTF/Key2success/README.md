# ShadowCTF key2success Write Up

## Details:
Points: 200

Jeopardy style CTF

Category: Reversing

## Write up:

Finally a challenge in this CTF that I actually ran to solve. Running strings got me a partial flag as well as a bunch of text:

```
$ strings key2sucess   

...
loneTable
u/UH
flag{NevH
er_stop_H
learningH
[]A\A]A^A_
Constant_learning_is_the_key
Hey.
I have a flag for you...
But i need a key in return
Can you give me the key
Great. Well here is your key:
Hmm. This not the key.
;*3$"
GCC: (Debian 9.3.0-18) 9.3.0
crtstuff.c
```

At this point I suspected that the flag was Never_stop_learning but still decided to run the program to confirm:

```
$ ./key2sucess   
Hey.
I have a flag for you...
But i need a key in return
Can you give me the key

> 
```

Looking at the strings I got earlier I tried submitting Constant_learning_is_the_key which ended up being the correct answer:

```
> Constant_learning_is_the_key
Great. Well here is your key:
flag{Never_stop_learning}
```