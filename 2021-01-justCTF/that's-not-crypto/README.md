# justCTF that's not crypto Write Up

## Details:
Points: 500

Jeopardy style CTF

Category: Reversing

## Write up:

This challenge gave me a .pyc file so I knew that I would need to use uncompyle:

``` python
uncompyle6 checker.pyc 
# uncompyle6 version 3.7.4
# Python bytecode 3.6 (3379)
# Decompiled from: Python 3.8.6 (default, Sep 25 2020, 09:36:53) 
# [GCC 10.2.0]
# Embedded file name: checker.py
# Compiled at: 2021-01-30 10:41:40
# Size of source mod 2**32: 50109 bytes
from random import randint

def make_correct_array(s):
    from itertools import accumulate
    s = map(ord, s)
    s = accumulate(s)
    return [x * 69684751861829721459380039 for x in s]


def validate(a, xs):

    def poly(a, x):
        value = 0
        for ai in a:
            value *= x
            value += ai

        return value

    if len(a) != len(xs) + 1:
        return False
    else:
        for x in xs:
            value = poly(a, x)
            if value != 24196561:
                return False

        return True


if __name__ == '__main__':
    a = [...]
    a = [ai * 4919 for ai in a]
    flag_str = input('flag: ').strip()
    flag = make_correct_array(flag_str)
    if validate(a, flag):
        print('Yes, this is the flag!')
        print(flag_str)
    else:
        print('Incorrect, sorry. :(')
# okay decompiling checker.pyc

```

The array (a) was massive so I shortened with ..., After seeing this I copied the python and put it in a file. 

I ran a few tests and found out that a had a length of 58. Looking into the validate function I saw that this meant that the flag would need to be 57 characters long.

I then decided that my best course of action would be to simply use the python code to semi-brute force the correct values:

``` python
from random import randint

def make_correct_array(s):
    from itertools import accumulate
    s = map(ord, s)
    s = accumulate(s)
    return [x * 69684751861829721459380039 for x in s]


def validate(a, xs):

    def poly(a, x):
        value = 0
        for ai in a:
            value *= x
            value += ai

        return value

    for x in xs:
        value = poly(a, x)
        if value != 24196561:
            return False

    return True


if __name__ == '__main__':
    a = [...]
    a = [ai * 4919 for ai in a]

    flag_str = "justCTF{"

    while len(flag_str) < 57:
        i = 32
        while i < 127:
            print(flag_str + chr(i))
            flag = make_correct_array(flag_str + chr(i))
            if (validate(a, flag)):
                print("correct:     " + chr(i))
                flag_str += chr(i)
            i += 1

    print(flag_str)
```

I had done some testing so I knew that first part of the flag was justCTF{ which was the flag format. I then modified the validate function so that it no longer cared about the length of the input and instead only checked if it was correct. 

I then went through for the length of the flag and added each possible char to the end of the string and "hashed" the string using their equation. I then checked each combination until I got the correct character, at which point I went to the next one.

The program then spit out the following flag:

```
justCTF{this_is_very_simple_flag_afer_so_big_polynomails}
```