# picoCTF Play Nice Write Up

## Details:
Points: 110

Jeopardy style CTF

Category: Cryptography

Comments: Not all ancient ciphers were so bad... The flag is not in standard format. nc mercury.picoctf.net 30568 playfair.py


## Write up:

Running the netcat command we get:

```
nc mercury.picoctf.net 30568

Here is the alphabet: 0fkdwu6rp8zvsnlj3iytxmeh72ca9bg5o41q
Here is the encrypted message: herfayo7oqxrz7jwxx15ie20p40u1i
What is the plaintext message?
```

Looking into the python script provided I first generated the matrix using the provided alphabet so that I had it to look at:

```python
# [['0', 'f', 'k', 'd', 'w', 'u'], 
#  ['6', 'r', 'p', '8', 'z', 'v'], 
#  ['s', 'n', 'l', 'j', '3', 'i'], 
#  ['y', 't', 'x', 'm', 'e', 'h'], 
#  ['7', '2', 'c', 'a', '9', 'b'], 
#  ['g', '5', 'o', '4', '1', 'q']]
```

I then started slowing reversing what the encryption did. From the encryption I saw that for each character one was added so the length would be the same. But that every pair relied on each other so I would need to iterate through two at a time:

```python
# decrypt string function
def decrypt_string(s, matrix):
	# place to store result
    result = ""
    
    # Iterate through string two at a time
	for i in range(0, len(s), 2):

        # pass the pairs to the decrypt_pair function
		result += decrypt_pair(s[i:i+2], matrix)

    # return result
	return result
```

Next I looked at the encrypt pair function. I then looked at how the different cases were encrypted, checked for those cases and then decrypted:

```python
# decrypt each pair
def decrypt_pair(pair, matrix):

    # get the indices in the matrix
	p1 = get_index(pair[0], matrix)
	p2 = get_index(pair[1], matrix)

    # if the first index is the same
	if p1[0] == p2[0]:
		return matrix[p1[0]][(p1[1]-1)%SQUARE_SIZE]+matrix[p2[0]][(p2[1]-1)%SQUARE_SIZE]

    # if the second index is the same
	if p1[1] == p2[1]:
		return matrix[(p1[0] - 1)  % SQUARE_SIZE][p1[1]] + matrix[(p2[0] - 1)  % SQUARE_SIZE][p2[1]]

    # else
	return matrix[p1[0]][p2[1]] + matrix[p2[0]][p1[1]]
```

This gave me the following script:

```python
# alphabet for matrix
alphabet = "0fkdwu6rp8zvsnlj3iytxmeh72ca9bg5o41q"

# square size
SQUARE_SIZE = 6

def generate_square(alphabet):
	assert len(alphabet) == pow(SQUARE_SIZE, 2)
	matrix = []
	for i, letter in enumerate(alphabet):
		if i % SQUARE_SIZE == 0:
			row = []
		row.append(letter)
		if i % SQUARE_SIZE == (SQUARE_SIZE - 1):
			matrix.append(row)
	return matrix

def get_index(letter, matrix):
	for row in range(SQUARE_SIZE):
		for col in range(SQUARE_SIZE):
			if matrix[row][col] == letter:
				return (row, col)
	print("letter not found in matrix.")
	exit()


# decrypt each pair
def decrypt_pair(pair, matrix):

	# get the indices in the matrix
	p1 = get_index(pair[0], matrix)
	p2 = get_index(pair[1], matrix)

	# if the first index is the same
	if p1[0] == p2[0]:
		return matrix[p1[0]][(p1[1]-1)%SQUARE_SIZE]+matrix[p2[0]][(p2[1]-1)%SQUARE_SIZE]

	# if the second index is the same
	if p1[1] == p2[1]:
		return matrix[(p1[0] - 1)  % SQUARE_SIZE][p1[1]] + matrix[(p2[0] - 1)  % SQUARE_SIZE][p2[1]]

	# else
	return matrix[p1[0]][p2[1]] + matrix[p2[0]][p1[1]]

# decrypt string function
def decrypt_string(s, matrix):
	# place to store result
	result = ""

	# Iterate through string two at a time
	for i in range(0, len(s), 2):

		# pass the pairs to the decrypt_pair function
		result += decrypt_pair(s[i:i+2], matrix)

	# return result
	return result

# generate square
m = generate_square(alphabet)

# encrypted message
enc_msg = "herfayo7oqxrz7jwxx15ie20p40u1i"

# decrypt string
print(decrypt_string(enc_msg, m))
```

When run I got:

```
emf57mgc51tp693dtt4g3h7f8ouwq3
```

When put into the nc instance I got the flag:

```
nc mercury.picoctf.net 30568

Here is the alphabet: 0fkdwu6rp8zvsnlj3iytxmeh72ca9bg5o41q
Here is the encrypted message: herfayo7oqxrz7jwxx15ie20p40u1i
What is the plaintext message? emf57mgc51tp693dtt4g3h7f8ouwq3
Congratulations! Here's the flag: 007d0a696aaad7fb5ec21c7698e4f754
```