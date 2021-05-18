# picoCTF Transformation Write Up

## Details:
Points: 20

Jeopardy style CTF

Category: Reverse Engineering

Comments:

I wonder what this really is... enc 

```python
''.join([chr((ord(flag[i]) << 8) + ord(flag[i + 1])) for i in range(0, len(flag), 2)])
```

## Write up:

Looking at the "encryption" used on the flag I noticed a few things.

- The script loops through 2 characters at a time
- The script then left shifts the first character left by two bytes: 0xFF becomes 0xFF00
- This is then added to the value of the 2nd character and turned into a character

So in order to undo this we need to take each character, take the first 2 bytes and turn that into a character and take the last 2 bytes into a character as well:

```python
# encrypted text
out = "灩捯䍔䙻ㄶ形楴獟楮獴㌴摟潦弸弰㑣〷㘰摽"

# string to output to
s = ""

# loop through each character
for x in out:
    # get 1st character
	s += chr((ord(x)&0xFF00)>>8)
    # get 2nd character
	s += chr(ord(x)&0xFF)

# print string
print(s)
```

When run we get:

```
picoCTF{16_bits_inst34d_of_8_04c0760d}
```