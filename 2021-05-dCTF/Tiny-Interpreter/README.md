# dCTF Tiny Interpreter Write Up

## Details:

Jeopardy style CTF

Points: 400

Category: Reverse Engineering

Comments:

We wrote an interpreter just so we can run our own binaries. Can you figure it out?


## Write up:

After this challenge we kind of gave up on this CTF because it showed the lack of attention and effort put into these challenges. For this challenge we were given the interpreter and then a file named bin.

Running the interpreter by itself you got:

```
./interpreter

Usage:
./interpreter <program you want to run>
```

So then I ran the program with the bin file and got:

```
./interpreter bin

I
n
t
e
r
p
r
e
t
e
r
_
w
r
i
t
t
e
n
_
i
n
_
C
_
i
s
_
a
_
g
r
e
a
t
_
i
d
e
a
```

And the flag was:

```
dctf{Interpreter_written_in_C_is_a_great_idea}
```