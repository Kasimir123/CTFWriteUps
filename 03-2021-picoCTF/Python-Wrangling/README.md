# picoCTF Python Wrangling Write Up

## Details:
Points: 10

Jeopardy style CTF

Category: General Skills

Comments:

Python scripts are invoked kind of like programs in the Terminal... Can you run this Python script using this password to get the flag?

## Write up:

Running the script by itself we get the following usage:

```
python3.8 ende.py  

Usage: ende.py (-e/-d) [file]
```

So then I tried running it with the file:

```
python3.8 ende.py -d flag.txt.en                                                           

Please enter the password:
```

I didn't feel like manually typing the password so I used cat and a pipe:

```
cat pw.txt | python3.8 ende.py -d flag.txt.en                                                                             

Please enter the password:picoCTF{4p0110_1n_7h3_h0us3_aa821c16}
```