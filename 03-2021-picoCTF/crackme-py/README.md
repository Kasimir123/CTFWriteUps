# picoCTF crackme-py Write Up

## Details:
Points: 30

Jeopardy style CTF

Category: Reverse Engineering


## Write up:

At the top of the python file you can see:

```python
# Hiding this really important number in an obscure piece of code is brilliant!
# AND it's encrypted!
# We want our biggest client to know his information is safe with us.
bezos_cc_secret = "A:4@r%uL`M-^M0c0AbcM-MFE055a4ce`eN"

# Reference alphabet
alphabet = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"+ \
            "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"



def decode_secret(secret):
    """ROT47 decode

    NOTE: encode and decode are the same operation in the ROT cipher family.
    """
```

And then the file calls another function, to solve this I simply changed what function the program calls:

```python
decode_secret(bezos_cc_secret)
```

And this then produced:

```
picoCTF{1|\/|_4_p34|\|ut_dd2c4616}
```