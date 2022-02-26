# Trellix Good'n Burger Write Up

## Details:

Jeopardy style CTF

Category: Network

Comments:

```
The K9's are trading passwords in plain sight. By using excerpts of publicly available books as their passwords, they can pass around credentials completely incognito! Catmen Sanfrancisco has provided you a SHA256 hash of their password and books she suspects they are using. Discover the sentence that pertains to the hash AAEEA96DD86D6ECBFE21755151B00266C79368584289D15B0BFE58D6B3498A36 to help thwart their nefurious plans!

NOTE: Insert the entire discovered sentence in the flag format before submitting, e.g. ATR[Every good dog does fine!]
```

## Write up:

This challenge was pretty straightforward. I needed to make a python script that would go through and try to hash every single sentence and see if it was the right one:

```py
# import os and hashlib
import os, hashlib

# get our hash
hash = "AAEEA96DD86D6ECBFE21755151B00266C79368584289D15B0BFE58D6B3498A36".lower()

# get all of the files in the path
files = [f for f in os.listdir('.') if os.path.isfile(f)]

# loop through all files
for file in files:
    print(file)

    # open the file
    with open(file, "r") as f:
        sentence = b''
        f = f.read()

        # loop through all characters
        for char in f:

            # if we end the sentence then try hashing it
            if char == "?" or char == "!" or char == ".":
                sentence += char.encode()
                if hash == hashlib.sha256(sentence.strip()).hexdigest():
                    print(sentence.strip())
                sentence = b''
            elif char == "\n":
                sentence += b' '
            else:
                sentence += char.encode()
```

Once run we got:

```
902-0.txt
42108-0.txt
84-0.txt
1250-0.txt
6130-0.txt
1184-0.txt
58585-0.txt
74-0.txt
158-0.txt
run.py
2591-0.txt
58975-0.txt
1342-0.txt
2554-0.txt
408-0.txt
1400-0.txt
43-0.txt
203-0.txt
1661-0.txt
4300-0.txt
46-0.txt
219-0.txt
2701-0.txt
25344-0.txt
45-0.txt
113-0.txt
45858-0.txt
16-0.txt
1952-0.txt
76-0.txt
205-0.txt
2600-0.txt
120-0.txt
98-0.txt
11-0.txt
b'The Cat only grinned when it saw Alice.'
1080-0.txt
```