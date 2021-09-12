# CSAW checker Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

What's up with all the zeros and ones? Where are my letters and numbers? (This is a reversing challenge.)

## Write up:

The first thing I did was look at the encode function:

```py
def encode(plain):
    d = 24
    x = up(plain)
    x = right(x,d)
    x = down(x)
    x = left(x,d)
    return x
```

I saw that I would have to do these function calls in reverse to decode the string. So I then started looking into the individual functions to see which ones I could just call and which ones I would have to rewrite.

```py
def right(x,d):
    x = x[d:] + x[0:d]
    return x

def left(x,d):
    x = right(x,len(x)-d)
    return x[::-1]
```

The functions left and right just reorganized the flag based on the length and the d value so these would not need to be rewritten.

```py
def down(x):
    x = ''.join(['1' if x[i] == '0' else '0' for i in range(len(x))])
    return x
```

The down function just reversed the bits so that also would not need to be rewritten.

```py
def up(x):
    x = [f"{ord(x[i]) << 1:08b}" for i in range(len(x))]
    return ''.join(x)
```

The up function however converted the flag to bits so I would need to rewrite that. I then added the decode function to the script:

```py
def up(x):
    x = [f"{ord(x[i]) << 1:08b}" for i in range(len(x))]
    return ''.join(x)

def down(x):
    x = ''.join(['1' if x[i] == '0' else '0' for i in range(len(x))])
    return x

def right(x,d):
    x = x[d:] + x[0:d]
    return x

def left(x,d):
    x = right(x,len(x)-d)
    return x[::-1]

def encode(plain):
    d = 24
    x = up(plain)
    x = right(x,d)
    x = down(x)
    x = left(x,d)
    return x


def decode(enc):
	d = 24
	enc = left(enc,d)
	enc = down(enc)
	enc = right(enc,d)
	
    # go 8 bits at a time and turn them into characters
	print(''.join([chr(int(''.join(enc[i:i+7]), 2)) for i in range(0, len(enc), 8)]))

def main():
    flag = "redacted"
    encoded = encode(flag)
    
    print("What does this mean?")
    encoded = "1010000011111000101010101000001010100100110110001111111010001000100000101000111011000100101111011001100011011000101011001100100010011001110110001001000010001100101111001110010011001100"
    decode(encoded)


if __name__ == "__main__":
  main()
```

When run this printed out:

```
What does this mean?
3vers!nG_w@rm_Up}flag{r
```

The flag was:

```
flag{r3vers!nG_w@rm_Up}
```