# picoCTF Static ain't always noise Write Up

## Details:
Points: 20

Jeopardy style CTF

Category: General Skills

Comments:

Can you look at the data in this binary: static? This BASH script might help!

## Write up:

Yet again we can simply use strings and grep to find the flag this time:

```
strings static | grep pico

picoCTF{d15a5m_t34s3r_1e6a7731}
```