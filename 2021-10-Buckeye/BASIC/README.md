# Buckeye BASIC Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

Get ready to go back to high school cause we're breaking out those graphing calculators! The ROM file is just to help run the challenge, you shouldn't have to reverse any of it. P.S. Remember that flags are case sensitive!

## Write up:

Running file on the provided program I was able to see that the .8xp file was a program for a tI-83 Graphing Calculator. I then did a bit of googling and came across the following [tool](https://github.com/benbalter/ti-83-programs).

After running the tool I got the following output:

```
BASIC
not protected
Single file dated Thu Oct 21 03:37:38 20
ClrHome
&statClrList &L4
"F4X67ENQPK0{MTJRHL}O3G59UB-ZAWV8S2YI1CD"->Str2
{26,25,38,10,6,35,6,12,13,2,14,17,27,38,18,29,23,23,27,30,2,33,27,26,11,16,37,7,22,19}->&L4
Input "GUESS: ",Str7
If length(Str7)!=30:Then
Disp "WRONG"
Stop
End
For(I,1,30,1)
If sub(Str7,I,1)!=sub(Str2,&L4(I),1)
Then
Disp "WRONG"
Stop
End
End
Disp "CORRECT"   
```

This program loops through the input which needs to be of length 30. It then compares the current character to a character in Str2 by getting an index from L4.

I then wrote the following solve script:

```py
# Str2 string from the program
str2 = "F4X67ENQPK0{MTJRHL}O3G59UB-ZAWV8S2YI1CD"

# Array with all the indices
L4 = [26,25,38,10,6,35,6,12,13,2,14,17,27,38,18,29,23,23,27,30,2,33,27,26,11,16,37,7,22,19]

# final string
s = ""

# loop through length
for i in range(0, 30):

    # append each character that we find by looping
	s += chr(ord(str2[L4[i]-1]))

# print out string
print(s)
```

When run this output:

```
BUCKEYE{M4TH-CLA55-W4S-B0R1NG}
```