# FooBar WarGames Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineerings

## Write up:

I used uncompyle6 to decompile the wargames .pyc file and found the following:

```python
game = ["FALKEN'S MAZE", 'TIC TAC TOE ', 'GLOBAL THERMONUCLEAR WAR']

def validateLaunchcode(launchcode):
    if len(launchcode[::-2]) != 12 or len(launchcode[15:]) != 9:
        print(denied)
        return False
    clen = len(launchcode)
    l1 = launchcode[:8]
    cc = []
    for i in range(0, len(l1), 2):
        q = []
        q.append(ord(l1[i]))
        q.append(ord(l1[(i + 1)]))
        cc.append(q)
    else:
        enc = []
        for i in range(len(cc)):
            val1 = cc[i][0] << 1
            val1 ^= 69
            val2 = cc[i][1] << 2
            val2 ^= 10
            enc.append(val1)
            enc.append(val2)
        else:
            correct = [
             159, 218, 153, 214, 45, 206, 153, 374]
            if enc != correct:
                print('ACCESS DENIED ok')
                return False
            l2 = launchcode[8:16]
            key = 'PEACEOUT'
            res = []
            [res.append(ord(key[i]) - ord(l2[i])) if i & 1 == 1 else res.append(ord(key[i]) + ord(l2[i])) for i in range(len(l2))]
            ok = [
             192, 18, 117, -32, 120, -16, 173, -2]
            if ok != res:
                print('ACCESS DENIED')
                return False
            l3 = launchcode[int(2 * clen / 3):]
            KEY = "There's no way to win"
            I = 7
            KARMA = [
             123, 47, 86, 28, 74, 50, 32, 114]
            MISSILE = []
            for x in l3:
                MISSILE.append((ord(x) + I ^ ord(KEY[I])) % 255)
                I = (I + 1) % len(KEY)
            else:
                if KARMA == MISSILE:
                    print(okk)
                    exit()
```

This split the key into 3 different components and checked each, this also told me that the game I wanted to get was 'GLOBAL THERMONUCLEAR WAR'.

I then wrote the following:

```python
# first encrypted key
correct = [159, 218, 153, 214, 45, 206, 153, 374]

# string to store result
s = ""

# decrypt the first key
for i in range(0, len(correct), 2):
            val1 = correct[i]^69
            val2 = correct[i+1]^10
            s += (chr(val1 >> 1))
            s += (chr(val2 >> 2))

# print what we have so far
print(s)

# second encrypted key
ok = [192, 18, 117, -32, 120, -16, 173, -2]
key = 'PEACEOUT'

# decrypt the second key
for i in range(0, len(ok)):
    if i & 1 == 1:
        s += chr(ord(key[i])-ok[i])
    else:
        s += chr(-ord(key[i])+ok[i])

# print what we have so far
print(s)

# third encrypted key
KARMA = [123, 47, 86, 28, 74, 50, 32, 114]
KEY = "There's no way to win"
I = 7

# decrypt third key
for i in range(0, len(KARMA)):
    for j in range(32, 127):
        if ((j + I ^ ord(KEY[I])) % 255) == KARMA[i]:
            s += chr(j)

    I = (I+1)%len(KEY)

# print all
print(s)
```

This then output:

```
m4n741n_
m4n741n_p34c3_XV
m4n741n_p34c3_XVT9022GLD
```

When entered into the "game" we got:

```
GLUG{15_7h15_r34l_0r_15_17_g4m3??}
```