# nahamcon DDR Write Up

## Details:

Jeopardy style CTF

Category: scripting

## Write up:

In this challenge we were given the following image:

![](./Original-Files/ddr.png)

After looking a little bit I noticed that the background for the first flag was #FFFFFF. Since the flag format was flag{} I decided to follow the arrows and keep track of the background hex colors.

The next few values were all in the ascii range so I knew I was on the right track. Then when I hit x7D7D7D I knew I had hit the end so I made a python script to write the hex to chars:

```python
# values
x = [0xff, 0x6c, 0x61, 0x67, 0x7b, 0x32, 0x61, 0x34, 0x63, 0x36, 0x37, 0x30, 0x36, 0x37, 0x34, 0x30, 0x32, 0x33, 0x62, 0x39, 0x37, 0x32, 0x66, 0x34, 0x65, 0x66, 0x32, 0x62, 0x63, 0x31, 0x39, 0x32, 0x32, 0x34, 0x65, 0x32, 0x35, 0x7d]

# string to save to
s = ""

# convert to chars and add to string
for i in x:
        s += chr(i)

# print string
print(s)
```

This output:

```
ÿlag{2a4c670674023b972f4ef2bc19224e25}
```

The first character is "off" but we know its an f, thats just because of how they set up the first tile.