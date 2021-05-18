# Tenable Hacker Manifesto Write Up

## Details:
Points: 250

Jeopardy style CTF

Category: Reverse Engineering

Comment: We found this file on a compromised host. It appears to contain data, but we're not sure how to decode it. Maybe you can figure it out?

## Write up:

This was actually a pretty fun and hard challenge. My teammate and I tried lots of different things from strings to substitutions based on the hackers manifest. We noticed that the file started with I am a hacker and decided to match it with the following paragraph from the manifest:

```
 I am a hacker, enter my world...
        Mine is a world that begins with school... I'm smarter than most of
the other kids, this crap they teach us bores me...
        Damn underachiever.  They're all alike.
```

We then noticed a pattern where the file would use old letters to decipher new ones. if the current byte was not valid then we would check the next byte. If the next byte was a multiple of 4 and not valid then we would replace that byte with the index of the current minus the value of the current byte till the index minus the value of the current plus the remainder of the next value divided by 4. Else if the current byte was divisible by 4 the pattern was taking the index minus the previous value minus one till the index minus 1 minus the previous byte plus the remainder of the current byte diveded by 4.

``` python
b=open('./hacker_manifesto.txt','rb').read()
import time

QWE = 0

# get all printable values
valid_values = [10, 32, 39, 45, 46, 44, 95, ord('"')]
valid_values.extend(list(range(48, 58)))
valid_values.extend(list(range(65, 91)))
valid_values.extend(list(range(97, 127)))
print(valid_values)


for qqq in range(130):
    # iterate through all bytes
    for i in range(len(b)):
        # if the current byte is not valid
        if b[i] not in valid_values:
            # a contains the current byte, the next byte, and the previous byte
            a = [b[i-1], b[i], b[i+1]]


            # if the next byte if divisible by 4 and not a valid number
            if a[2] % 4 == 0 and a[2] not in valid_values:
                # p = current byte, q = next byte
                p,q = b[i], b[i+1]
                # tmp1 contains all bytes until
                tmp1 = b[:i]
                # tmp2 contains all bytes from next onward
                tmp2 = b[i+2:]
                # c = bytes from currentNumber - current byte to currentNumber - current byte + (next byte // 4 => how many times 4 can perfectly fit into it)
                c = b[i-p:i-p+q//4]
                # fix the bytes
                b = tmp1+c+tmp2
            # if the current byte is divisible by 4
            elif a[1] % 4 == 0:
                # p = last byte, q = current byte
                p, q = b[i-1], b[i]
                # tmp1 contains all bytes to last one
                tmp1 = b[:i-1]
                # tmp2 contains all bytes from next onward
                tmp2 = b[i + 1:]
                # c = bytes from currentNumber - current byte to currentNumber -1 - current byte + (next byte // 4 => how many times 4 can perfectly fit into it)
                c = b[i -1 - p:i -1 - p + q // 4]
                # fix the bytes
                b = tmp1 + c + tmp2
            break
    print(b[:500])
    print('###################\n\n')
```

The output of this was:

```
b'I am a hacker, enter my world...\nMine is a world that begins with school... I\'m smarter than most of the other kids, this crap they teach us bores me... \n\nDamn underachiever.  They\'re all alike.\n\nI\'m in junior high or high school.  I\'ve listened to teachers explain for the fifteey\th\x0e\x08i\x8b\x08 @\x08w/\x10r6\x08u0\x04e\x06\rf\x8e\x0cth\x08nT\x10 \x9e\x14s\x0f\x04a\x07\x08 \x0b\t.\x12\x08"\x00\x00N\x19\x04,\x05\x04M\x11\x04.\x04\x04S:\x04iA\x08,"\x0cd\xf1\x08n{\x04t\x88\x08hG\x0cmW\x15k:\x14d\x18\x08 #\x08 x\x0cm\x16\x08h\x8c\x08d\xf4\x0c"\xf4\x1ck\x1c\x08.$\x08P*\x04o\x10\x05a\x02\x04l\x1e\x08c\x08\x04p\xfe\x08da\x18T\xff\x1ca\xff\x1ce\xff\x10 t\td\x9b\x10d\xdf\x08c,\x04v\x91\x08y\xb1\x0cd\x0f\x04yh\x14f\n\x04u\x9d\x0caE\x0cmF\x04u\x99\r.\x15\x08W\xee\x08t\x12\x0cs\r\x04c\xc6\x08d\x8d\x1di\x90\ro)\x19tH\x08o\x86\rw\xe8\x11I\x07'
```

As you can see, this was sadly not completely functional, it hung up on values such as b'\t' which equals b'\x09' this was due to carryover, the value should have been either \x01\x08 or something else. We weren't able to figure out a good way to code this special case so instead we set b to the last decoded value and manually swapped out the expected value each run. After a bit of decoding we got the flag:

```
'I am a hacker, enter my world...\nMine is a world that begins with school... I\'m smarter than most of the other kids, this crap they teach us bores me... \n\nDamn underachiever.  They\'re all alike.\n\nI\'m in junior high or high school.  I\'ve listened to teachers explain for the fifteenth time how to reduce a fraction.  I understand it.  "No, Ms. Smith, I didn\'t show my work.  I did it in my head..."\n\nDamn kid.  Probably copied it.  They\'re all alike.\n\nI made a discovery today.  I found a computer.  Wait a second, this is cool.  It does what I want it to.  If it makes a mistake, it\'s because I screwed it up.  Not because it doesn\'t like me...\n\nOr feels threatened by me...\nOr thinks I\'m a smart ass...\nOr doesn\'t like teaching and shouldn\'t be here...\nDamn kid.  All he does is play games.  They\'re all alike.\n\nAnd then it happened... a door opened to a world... rushing through the phone line like heroin through an addict\'s veins, an electronic pulse is sent out, a refuge from the day-to-day incompetencies is sought... a board is found.\n\nThe flag is "flag{TheMentorArrested}".\n\n"This is it...
```