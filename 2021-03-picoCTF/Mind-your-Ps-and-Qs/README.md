# picoCTF Mind your Ps and Qs Write Up

## Details:
Points: 20

Jeopardy style CTF

Category: Cryptography

Comments: In RSA, a small e value can be problematic, but what about N? Can you decrypt this?

## Write up:

Using cat on the file I got:

```
cat values           

Decrypt my super sick RSA:
c: 62324783949134119159408816513334912534343517300880137691662780895409992760262021
n: 1280678415822214057864524798453297819181910621573945477544758171055968245116423923
e: 65537  
```

This was a pretty small n value so I decided to use alpertron to find the prime factors:

1899107986527483535344517113948531328331 and 674357869540600933870145899564746495319033

I then plugged these values and the c, n, and e into the rsa cracker script I wrote for dvCTF:

```python
# all prime factors
x = [1899107986527483535344517113948531328331, 674357869540600933870145899564746495319033]

# n
n = 1280678415822214057864524798453297819181910621573945477544758171055968245116423923

# e
e = 65537

# cipher text
ct = 62324783949134119159408816513334912534343517300880137691662780895409992760262021

# start value for phi(n)
i = 1

# loop through prime factors and multiply them together with (factor-1)*(nextFactor-1)...
for a in x:
	i = i * (a-1)

# inverse pow (3.8+ syntax, for previous versions of python use gmpy2.invert)
d = pow(e, -1, i)

# solve for the answer
ans = pow(ct, d, n)

# print the answer
print(bytes.fromhex(hex(ans)[2:]).decode('ascii'))
```

When run I got:

```
picoCTF{sma11_N_n0_g0od_05012767}
```