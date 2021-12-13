# idek Idek's ExponEntial Extravaganza Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments: Wrap password with idek{}

## Write up:

```c
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv) {
  if(argc != 2){
    printf("Usage: ./reverseme password\n");
    return 1;
  }
  if(strlen(argv[1])!=14){
    printf("Incorrect Length\n");
    return 1;
  }
  if(*argv[1] != 112){//Not enough precision
    printf("Password Incorrect\n");
    return 1;
  }
  double magic_numbers[7] ={-68822144.50341525673866271972656250000000000000000000000000, 56777293.39031631499528884887695312500000000000000000000000, -3274524.75536667229607701301574707031250000000000000000000, -85761.51255339206545613706111907958984375000000000000000, 8443.33244327564352715853601694107055664062500000000000, -166.67369627952575683593750000000000000000000000000000, 1.00000000000000000000000000000000000000000000000000, };
  for(int i = 0; i < 6;i++){
    double foo=1.0,bar=0.0;
    for(int j=0;j<7;j++){
      bar += magic_numbers[j] * foo;
      foo *= (float)log(*(float*)((unsigned long)argv[1]+2*i));
    }
    if((int)(100000*bar) != 0){
      printf("Password Incorrect\n");
      return 1;
    }
  }
  printf("Password Correct\n");
  return 0;
}
```

Looking at the program there are several things that stand out.

The first was:

```c
  if(strlen(argv[1])!=14){
    printf("Incorrect Length\n");
    return 1;
  }
```

This let me know that the total length of the password would be 14 characters long.

The second was:

```c
  if(*argv[1] != 112){//Not enough precision
    printf("Password Incorrect\n");
    return 1;
  }
```

The first character of the password is equal to 112 which is the ascii character 'p'.

I then finally decided to look at the main logic in the program:

```c
  for(int i = 0; i < 6;i++){
    double foo=1.0,bar=0.0;
    for(int j=0;j<7;j++){
      bar += magic_numbers[j] * foo;
      foo *= (float)log(*(float*)((unsigned long)argv[1]+2*i));
    }
    if((int)(100000*bar) != 0){
      printf("Password Incorrect\n");
      return 1;
    }
  }
```

The only line that gets changed by our input is:

```c
foo *= (float)log(*(float*)((unsigned long)argv[1]+2*i));
```

Where i is 0, 1, 2, 3, 4, and 5. Meaning that we get the 0th, 2nd, 4th, 6th, 8th, and 10th character. 

We also know that unsigned long is 8 bytes so we get the first 8 characters from the current character, however, we then convert it to a float so we only use the first 4 bytes (e.g. if we enter password, only pass will be used to generate the first value, and sswo would be used the second time and so on). This means that the last 2 characters of one section will be the first 2 characters of the second section. 

Since we only need sets of 4 characters and they are in ascii range we can brute force the possible values so I modified the program:

```c
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	double magic_numbers[7] = { -68822144.50341525673866271972656250000000000000000000000000, 56777293.39031631499528884887695312500000000000000000000000, -3274524.75536667229607701301574707031250000000000000000000, -85761.51255339206545613706111907958984375000000000000000, 8443.33244327564352715853601694107055664062500000000000, -166.67369627952575683593750000000000000000000000000000, 1.00000000000000000000000000000000000000000000000000, };

	char *nums = (char*) malloc(sizeof(unsigned long));

	for (char a = 32; a < 126; a++)
	{
		nums[0] = a;
		for (char b = 32; b < 126; b++)
		{
			nums[1] = b;
			for (char c = 32; c < 126; c++)
			{
				nums[2] = c;
				for (char d = 32; d < 126; d++)
				{
					nums[3] = d;
					double foo = 1.0, bar = 0.0;
					for (int j = 0; j < 7; j++)
					{
						bar += magic_numbers[j] *foo;

						foo *= (float) log(*(float*)((unsigned long) nums));
					}

					if ((int)(100000 *bar) == 0)
					{
						printf("%c%c%c%c\n", nums[0], nums[1], nums[2], nums[3]);
					}
				}
			}
		}
	}

	return 0;
}
```

I compiled it with the lm flag to link the math library:

```
gcc reverseme.c -lm
```

When run it output:

```
$Yn@
%Yn@
/fTw
0fTw
1fTw
21aL
2fTw
31aL
3fTw
41aL
4fTw
51aL
5fTw
61aL
6fTw
71aL
7fTw
81aL
8fTw
91aL
9fTw
:1aL
:fTw
;1aL
;fTw
<1aL
<fTw
=1aL
=fTw
>1aL
>fTw
?1aL
?fTw
@1aL
@fTw
A1aL
AfTw
B1aL
BfTw
C1aL
CfTw
D1aL
DfTw
E1aL
EfTw
F1aL
FfTw
G1aL
GfTw
H1aL
HfTw
I1aL
IfTw
J1aL
JfTw
K1aL
KfTw
L1aL
LfTw
M1aL
MfTw
NfTw
OfTw
PfTw
QfTw
RfTw
SLzf
SfTw
TLzf
TfTw
ULzf
UfTw
VLzf
VfTw
WLzf
WfTw
XLzf
XfTw
YLzf
YfTw
ZLzf
ZfTw
[Lzf
[fTw
\Lzf
\fTw
]Lzf
]fTw
^Lzf
^fTw
_Lzf
_fTw
`Lzf
`fTw
a0%Y
aLzf
afTw
b0%Y
bLzf
bfTw
c0%Y
cLzf
cfTw
d0%Y
dLzf
dfTw
e0%Y
eLzf
efTw
f0%Y
fLzf
ffTw
g0%Y
gLzf
gfTw
h0%Y
hLzf
hfTw
i0%Y
iLzf
ifTw
j0%Y
jLzf
jfTw
k0%Y
kLzf
kfTw
l0%Y
lLzf
lfTw
m0%Y
m@M1
mLzf
mfTw
n0%Y
n@M1
nLzf
nfTw
o0%Y
o@M1
oLzf
ofTw
p0%Y
p@M1
pLzf
pfTw
q0%Y
q@M1
qLzf
qfTw
r0%Y
r@M1
rLzf
rfTw
s0%Y
s@M1
sLzf
sfTw
t0%Y
t@M1
tLzf
tfTw
u0%Y
u@M1
uLzf
ufTw
v0%Y
v@M1
vLzf
vfTw
w0%Y
w@M1
wLzf
wfTw
x0%Y
x@M1
xLzf
xfTw
y0%Y
y@M1
yLzf
yfTw
z0%Y
z@M1
zLzf
zfTw
{0%Y
{@M1
{Lzf
{fTw
|0%Y
|@M1
|Lzf
|fTw
}0%Y
}@M1
}Lzf
}fTw
```

Knowing that the password started with p and that the last two characters would be the first two of the next one I then went through and picked out the password:

```
p0%Y - %Yn@ - n@M1 - M1aL - aLzf - zfTw
```

Which made the flag:

```
idek{p0%Yn@M1aLzfTw}
```