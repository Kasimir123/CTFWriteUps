# picoCTF New Vignere Write Up

## Details:
Points: 300

Jeopardy style CTF

Category: Cryptography

Comments: Another slight twist on a classic, see if you can recover the flag. (Wrap with picoCTF{}) 

```
bkglibgkhghkijphhhejggikgjkbhefgpienefjdioghhchffhmmhhbjgclpjfkp
```

## Write up:

Looking at the code I noticed that this encryption was similar to new caesar but with a longer key length. Because of this I was able to reuse some of the old code I had.

Looking at the code we can see that the key will always be less than 15, given that the encrypted text is 64 characters, plain text is 32, that means that the key will wrap around so there should be repeats. Based on this we can generate a set of possible keys for each set of encrypted values. After we have the values we can then use the Kasiski examination method to check for the key length, and then we can "brute force" the cipher.

For that I wrote the following code:

```python
# encrypted text
enc = "bkglibgkhghkijphhhejggikgjkbhefgpienefjdioghhchffhmmhhbjgclpjfkp"


keys = []

# create array
[keys.append([]) for i in range(0,32)]

# loop through alphabet twice
for a in ALPHABET:
	for b in ALPHABET:
		# generate key pair
		key = str(a) + str(b)
		
		# store plain text		
		pt = ""

		# unshift all values with key pair
		for i,c in enumerate(enc):
			pt += unshift(c, key[i%len(key)])

		# decode
		pt = b16_decode(pt)

		# loop through decrypted plaintext
		for cur in range(0, len(pt)):

			# check each plaintext char to see if its valid, if it is then add the arrays
			if pt[cur] in "abcdef0123456789":
				keys[cur].append(key)

# print the possible key pairs
for key in keys:
	print(key)
```

Which when run generated:

```
['le', 'lf', 'lg', 'lh', 'li', 'lj', 'ob', 'oc', 'od', 'oe', 'of', 'og', 'oh', 'oi', 'oj', 'ok']
['af', 'ag', 'ah', 'ai', 'aj', 'ak', 'dc', 'dd', 'de', 'df', 'dg', 'dh', 'di', 'dj', 'dk', 'dl']
['ca', 'cl', 'cm', 'cn', 'co', 'cp', 'fa', 'fb', 'fi', 'fj', 'fk', 'fl', 'fm', 'fn', 'fo', 'fp']
['ae', 'af', 'ag', 'ah', 'ai', 'aj', 'db', 'dc', 'dd', 'de', 'df', 'dg', 'dh', 'di', 'dj', 'dk']
['ba', 'bb', 'bc', 'bd', 'be', 'bf', 'ea', 'eb', 'ec', 'ed', 'ee', 'ef', 'eg', 'en', 'eo', 'ep']
['be', 'bf', 'bg', 'bh', 'bi', 'bj', 'eb', 'ec', 'ed', 'ee', 'ef', 'eg', 'eh', 'ei', 'ej', 'ek']
['cd', 'ce', 'cf', 'cg', 'ch', 'ci', 'fa', 'fb', 'fc', 'fd', 'fe', 'ff', 'fg', 'fh', 'fi', 'fj']
['jb', 'jc', 'jd', 'je', 'jf', 'jg', 'ma', 'mb', 'mc', 'md', 'me', 'mf', 'mg', 'mh', 'mo', 'mp']
['bb', 'bc', 'bd', 'be', 'bf', 'bg', 'ea', 'eb', 'ec', 'ed', 'ee', 'ef', 'eg', 'eh', 'eo', 'ep']
['ba', 'bb', 'bc', 'bd', 'be', 'bf', 'bg', 'bh', 'bi', 'bj', 'od', 'oe', 'of', 'og', 'oh', 'oi']
['aa', 'ab', 'ac', 'ad', 'ae', 'af', 'da', 'db', 'dc', 'dd', 'de', 'df', 'dg', 'dn', 'do', 'dp']
['ce', 'cf', 'cg', 'ch', 'ci', 'cj', 'fb', 'fc', 'fd', 'fe', 'ff', 'fg', 'fh', 'fi', 'fj', 'fk']
['ad', 'ae', 'af', 'ag', 'ah', 'ai', 'da', 'db', 'dc', 'dd', 'de', 'df', 'dg', 'dh', 'di', 'dj']
['ea', 'el', 'em', 'en', 'eo', 'ep', 'ha', 'hb', 'hi', 'hj', 'hk', 'hl', 'hm', 'hn', 'ho', 'hp']
['ba', 'bb', 'bc', 'bd', 'bo', 'bp', 'ea', 'eb', 'ec', 'ed', 'ee', 'el', 'em', 'en', 'eo', 'ep']
['ca', 'cb', 'cc', 'cd', 'ce', 'cf', 'cg', 'cn', 'co', 'cp', 'pa', 'pb', 'pc', 'pd', 'pe', 'pf']
['jc', 'jd', 'je', 'jf', 'jg', 'jh', 'ma', 'mb', 'mc', 'md', 'me', 'mf', 'mg', 'mh', 'mi', 'mp']
['be', 'bf', 'bg', 'bh', 'bi', 'bj', 'bk', 'bl', 'bm', 'bn', 'oh', 'oi', 'oj', 'ok', 'ol', 'om']
['ba', 'bb', 'bc', 'bd', 'be', 'bf', 'bm', 'bn', 'bo', 'bp', 'oa', 'ob', 'oc', 'od', 'oe', 'op']
['da', 'db', 'dc', 'dn', 'do', 'dp', 'ga', 'gb', 'gc', 'gd', 'gk', 'gl', 'gm', 'gn', 'go', 'gp']
['ci', 'cj', 'ck', 'cl', 'cm', 'cn', 'ff', 'fg', 'fh', 'fi', 'fj', 'fk', 'fl', 'fm', 'fn', 'fo']
['ab', 'ac', 'ad', 'ae', 'af', 'ag', 'da', 'db', 'dc', 'dd', 'de', 'df', 'dg', 'dh', 'do', 'dp']
['ba', 'bb', 'bm', 'bn', 'bo', 'bp', 'ea', 'eb', 'ec', 'ej', 'ek', 'el', 'em', 'en', 'eo', 'ep']
['ba', 'bb', 'bc', 'bd', 'be', 'bp', 'ea', 'eb', 'ec', 'ed', 'ee', 'ef', 'em', 'en', 'eo', 'ep']
['ca', 'cb', 'cc', 'cd', 'ce', 'cf', 'cg', 'ch', 'co', 'cp', 'pb', 'pc', 'pd', 'pe', 'pf', 'pg']
['gg', 'gh', 'gi', 'gj', 'gk', 'gl', 'jd', 'je', 'jf', 'jg', 'jh', 'ji', 'jj', 'jk', 'jl', 'jm']
['bb', 'bc', 'bd', 'be', 'bf', 'bg', 'ea', 'eb', 'ec', 'ed', 'ee', 'ef', 'eg', 'eh', 'eo', 'ep']
['ld', 'le', 'lf', 'lg', 'lh', 'li', 'oa', 'ob', 'oc', 'od', 'oe', 'of', 'og', 'oh', 'oi', 'oj']
['aa', 'ab', 'am', 'an', 'ao', 'ap', 'da', 'db', 'dc', 'dj', 'dk', 'dl', 'dm', 'dn', 'do', 'dp']
['fj', 'fk', 'fl', 'fm', 'fn', 'fo', 'ig', 'ih', 'ii', 'ij', 'ik', 'il', 'im', 'in', 'io', 'ip']
['da', 'db', 'dc', 'dd', 'de', 'dp', 'ga', 'gb', 'gc', 'gd', 'ge', 'gf', 'gm', 'gn', 'go', 'gp']
['ej', 'ek', 'el', 'em', 'en', 'eo', 'hg', 'hh', 'hi', 'hj', 'hk', 'hl', 'hm', 'hn', 'ho', 'hp']
```

Looking through this it became fairly obvious that the key pairs repeated every 9 times making the key length 9.

I then wrote a decryption script to recursively add together the key pairs as well as add one last letter and then test all the possible 9 length keys:

```python
# import string
import string

# constants
LOWERCASE_OFFSET = ord("a")
ALPHABET = string.ascii_lowercase[:16]

# see caesar cipher for what these are
def b16_decode(cipher):
	dec = ""

	for c in range(0, len(cipher), 2):

		b = ""
		b += "{0:b}".format(ALPHABET.index(cipher[c])).zfill(4)
		b += "{0:b}".format(ALPHABET.index(cipher[c+1])).zfill(4)

		dec += chr(int(b,2))
    
	return dec

def unshift(c, k):
	t1 = ord(c) - LOWERCASE_OFFSET
	t2 = ord(k) - LOWERCASE_OFFSET
	return ALPHABET[(t1 - t2) % len(ALPHABET)]

# tries to decrypt
def get_key(s, matrix):
	# if we can't go further down
	if len(matrix) == 1:
		# add the last value
		for a in ALPHABET:
			k = str(s) + str(a)
			pt = ""
			for i,c in enumerate(enc):
				pt += unshift(c, k[i%len(k)])

			pt = b16_decode(pt)

			# if the plain text is good then print it
			if all(c in "abcdef0123456789" for c in pt):
				print(pt)
		return
	
	# recursively build key string
	for x in matrix[0]:
		s2 = str(s) + str(x)
		get_key(s2, matrix[1:len(matrix)])

# encrypted text
enc = "bkglibgkhghkijphhhejggikgjkbhefgpienefjdioghhchffhmmhhbjgclpjfkp"


keys = []

# create array
[keys.append([]) for i in range(0,32)]

# loop through alphabet twice
for a in ALPHABET:
	for b in ALPHABET:
		# generate key pair
		key = str(a) + str(b)
		
		# store plain text		
		pt = ""

		# unshift all values with key pair
		for i,c in enumerate(enc):
			pt += unshift(c, key[i%len(key)])

		# decode
		pt = b16_decode(pt)

		# loop through decrypted plaintext
		for cur in range(0, len(pt)):

			# check each plaintext char to see if its valid, if it is then add the arrays
			if pt[cur] in "abcdef0123456789":
				keys[cur].append(key)

# print the possible key pairs
for key in keys:
	print(key)

# decrypt the code
get_key("", keys[0:5])
```

When run this output:

```
698987ddce418c11e9aa564229c50fda
```