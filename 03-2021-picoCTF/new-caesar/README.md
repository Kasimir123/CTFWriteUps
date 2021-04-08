# picoCTF New Caesar Write Up

## Details:
Points: 60

Jeopardy style CTF

Category: Cryptography

Comments:

We found a brand new type of encryption, can you break the secret code? (Wrap with picoCTF{}) 

```
mlnklfnknljflfmhjimkmhjhmljhjomhmmjkjpmmjmjkjpjojgjmjpjojojnjojmmkmlmijimhjmmj
```

## Write up:

After looking through their code I found a few important facts:

- the key was one letter in the first 16 lowercase characters of the alphabet
- to decode I would need to unshift first and then decode

So I started dissecting the shift function:

```python
def shift(c, k):
	t1 = ord(c) - LOWERCASE_OFFSET
	t2 = ord(k) - LOWERCASE_OFFSET
	return ALPHABET[(t1 + t2) % len(ALPHABET)]
```

To undo this I would simply need to subtract t2 from t1 rather than adding to it, I created the new function and moved to the encode function:

```python
def b16_encode(plain):
	enc = ""
	for c in plain:
		binary = "{0:08b}".format(ord(c))
		enc += ALPHABET[int(binary[:4], 2)]
		enc += ALPHABET[int(binary[4:], 2)]
	return enc
```

This function takes the text and turns each character into binary, then turns the first 4 and last 4 bits into integers and then gets a letter for each of these from the alphabet list. To undo this we need to get the letter placement in ALPHABET and then convert them to binary, combine, and turn into a character.

With this information I created a python script:

```python
# import string
import string

# constants
LOWERCASE_OFFSET = ord("a")
ALPHABET = string.ascii_lowercase[:16]

# decode function
def b16_decode(cipher):
	dec = ""
    # loop through the cipher 2 characters at a time
	for c in range(0, len(cipher), 2):
        # turn the two characters into one binary string
		b = ""
		b += "{0:b}".format(ALPHABET.index(cipher[c])).zfill(4)
		b += "{0:b}".format(ALPHABET.index(cipher[c+1])).zfill(4)
        # turn the binary string to a character and add
		dec += chr(int(b,2))
    
    # return
	return dec

# unshift the text
def unshift(c, k):
	t1 = ord(c) - LOWERCASE_OFFSET
	t2 = ord(k) - LOWERCASE_OFFSET
	return ALPHABET[(t1 - t2) % len(ALPHABET)]

# encrypted flag
enc = "mlnklfnknljflfmhjimkmhjhmljhjomhmmjkjpmmjmjkjpjojgjmjpjojojnjojmmkmlmijimhjmmj"

# loop through all possible keys
for key in ALPHABET:
    # initialize string
	s = ""

    # loop through the encrypted text
	for i,c in enumerate(enc):
        # unshift it based on key
		s += unshift(c, key[i % len(key)])

    # decode
	s = b16_decode(s)

    # print key
	print(s)	
```

This gives us 16 different possible texts when run:

```
ËÚµÚÛµÇÊÇËÇÌÌÊËÈÇÉ
ºÉ¤ÉÊ¤¶¹¶º¶»»¹º·¶¸
©¸¸¹s¥v¨¥u©u|¥ªx}ªzx}|tz}||{|z¨©¦v¥z§
§§¨beddkgliglkcilkkjkiei
qQqTSSZV[XV[ZRX[ZZYZXTX
v`@`rCurBvBIrwEJwGEJIAGJIIHIGuvsCrGt
et_tu?_a2da1e18af49f649806988786deb2a6c
TcNcd.NP!SP T 'PU#(U%#('/%(''&'%STQ!P%R
CR=RS=OBOCODDBC@OA
2A,AB
12?>0,>1>2>33
!01ûþ -ý!ýô-"ðõ"òðõôüòõôôóôò !.þ-ò/
/
/ ê
íììãïäáïäãëáäããâãáíá
ùÙù
   Ü
    ÛÛÒ
       ÞÓÐÞÓÒÚÐÓÒÒÑÒÐ
                     Ü
                      Ð
ÈèúËýúÊþÊÁúÿÍÂÿÏÍÂÁÉÏÂÁÁÀÁÏýþûËúÏü
íü×üý·×éºìé¹í¹°éî¼±î¾¼±°¸¾±°°¿°¾ìíêºé¾ë
ÜëÆëì¦ÆØ©ÛØ¨Ü¨¯ØÝ« Ý­« ¯§­ ¯¯®¯­ÛÜÙ©Ø­Ú
```

Of all of these the only one that looks right, and is relevant to the title, is:

```
et_tu?_a2da1e18af49f649806988786deb2a6c
```

which gives us:

```
picoCTF{et_tu?_a2da1e18af49f649806988786deb2a6c}
```