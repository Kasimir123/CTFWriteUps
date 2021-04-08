# Angstrom Jailbreak Write Up

## Details:
Points: 75

Jeopardy style CTF

Category: Reverse Engineering

Comments:

Clam was arguing with kmh about whether including 20 pyjails in a ctf is really a good idea, and kmh got fed up and locked clam in a jail with a python! Can you help clam escape?

Find it on the shell server at /problems/2021/jailbreak over over netcat at nc shell.actf.co 21701

## Write up:

My first step was to take the function that generated text and comment each of the returns for the proper text. I then started working backwards from the flag printing.

```c
  if ( v5 != 1337 || (v25 = (char *)sub_15A0(23LL), v26 = strcmp(v37, v25), free(v25), v26) )
  {
    v15 = (char *)sub_15A0(19LL);
    v16 = strcmp(v37, v15);
    free(v15);
    if ( v16 )
    {
      v17 = (char *)sub_15A0(20LL);
      v18 = strcmp(v37, v17);
      free(v17);
      v8 = 4LL;
      if ( !v18 )
      {
        v5 = 2 * v5 + 1;
        v8 = 22LL;
      }
    }
    else
    {
      v5 *= 2;
      v8 = 21LL;
    }
    goto LABEL_5;
  }
```

The last check before the flag was printed was the above. v5 needed to go from 1 to 1337 by either typing press the red button or press the green button. I did some math and found the pattern to do this was:

R, G, R, R, G, G, G, R, R, G

Looking through the code some more I noticed that a compare had to be correct to get to this code. To get to that one we had to do another string first, which we could only get to if we do two before that as well. I ended up writing the following script:

```python
# import pwntools
from pwn import *

# connect to remote
r = remote('shell.actf.co', 21701)

# button presses
b = "press the red button\n"
a = "press the green button\n"
x = [b,a,b,b,a,a,a,b,b,a]

# run the script
print(r.recvline())
print(r.recvline())
r.send("pick the snake up\n")
print(r.recvline())
print(r.recvline())
r.send("throw the snake at kmh\n")
print(r.recvline())
print(r.recvline())
r.send("pry the bars open\n")
print(r.recvline())
print(r.recvline())
r.send("look around\n")
print(r.recvline())
print(r.recvline())
for i in x:
	print(i)
	r.send(i)
	print(r.recvline())
	print(r.recvline())

r.send("bananarama\n")
print(r.recvline())
print(r.recvline())
```

Running the script we get:

```
[+] Opening connection to shell.actf.co on port 21701: Done
b"Welcome to clam's daring jailbreak! Please keep your hands and feet inside the jail at all times.\n"
b'What would you like to do?\n'
b'You pick the snake up.\n'
b'What would you like to do?\n'
b'You throw the snake at kmh and watch as he runs in fear.\n'
b'What would you like to do?\n'
b'You start prying the prison bars open. A wide gap opens and you slip through.\n'
b'What would you like to do?\n'
b'You look around and see that kmh has already made the jail contrived! There\'s a red button and a green button with a sign that says "pres butons 2 get fleg".\n'
b'What would you like to do?\n'
press the red button

b'You pressed the red button. Nothing changed.\n'
b'What would you like to do?\n'
press the green button

b'You pressed the green button. Nothing changed.\n'
b'What would you like to do?\n'
press the red button

b'You pressed the red button. Nothing changed.\n'
b'What would you like to do?\n'
press the red button

b'You pressed the red button. Nothing changed.\n'
b'What would you like to do?\n'
press the green button

b'You pressed the green button. Nothing changed.\n'
b'What would you like to do?\n'
press the green button

b'You pressed the green button. Nothing changed.\n'
b'What would you like to do?\n'
press the green button

b'You pressed the green button. Nothing changed.\n'
b'What would you like to do?\n'
press the red button

b'You pressed the red button. Nothing changed.\n'
b'What would you like to do?\n'
press the red button

b'You pressed the red button. Nothing changed.\n'
b'What would you like to do?\n'
press the green button

b'You pressed the green button. Nothing changed.\n'
b'What would you like to do?\n'
b'For some reason, a flag popped out of the wall, and you walk closer to read it.\n'
b'actf{guess_kmh_still_has_unintended_solutions}\n'
```