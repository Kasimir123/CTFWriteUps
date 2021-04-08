
# picoCTF Mod 26 Write Up

## Details:
Points: 10

Jeopardy style CTF

Category: Cryptography

Comments:

Cryptography can be easy, do you know what ROT13 is? 
```
cvpbPGS{arkg_gvzr_V'yy_gel_2_ebhaqf_bs_ebg13_MAZyqFQj}
```

## Write up:

ROT13 is a cipher that rotates each character 13 letters over. The mod 26 is a hint about looping back around. You can use an online decoder but since I'm trying to explain things a bit more here I decided to make a small script to decode this:

```python
# initial encrypted text
flag = "cvpbPGS{arkg_gvzr_V'yy_gel_2_ebhaqf_bs_ebg13_MAZyqFQj}"

# A-Z
AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

# a-z
az = "abcdefghijklmnopqrstuvwxyz"

# string to store result
s = ""

# iterate through encrypted flag
for x in flag:
    # if the character is in AZ
	if x in AZ:
        # go 13 characters further from the current character
		s += AZ[(AZ.index(x)+13)%len(AZ)]
    	# if the character is in az
	elif x in az:
        # go 13 characters further from the current character
		s += az[(az.index(x)+13)%len(az)]
	else:
        # else add the character
		s += x

# print string
print(s)
```

Once run we get:

```
picoCTF{next_time_I'll_try_2_rounds_of_rot13_ZNMldSDw}
```