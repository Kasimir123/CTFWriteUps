# FooBar Numerical Computing Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineerings

## Write up:

After some reversing I found the following code which checked the flag:

```c
  f = 0;
  qmemcpy(enc, "QWERTYUIOPASDFGHJK", sizeof(enc));
  *(_QWORD *)num = A_0_3887;
  *(_QWORD *)&num[2] = 0x5400000100LL;
  *(_QWORD *)&num[4] = 0xA0000002F0LL;
  *(_QWORD *)&num[6] = 0x1E000000670LL;
  *(_QWORD *)&num[8] = 0xCC000007B0LL;
  *(_QWORD *)&num[10] = 0x19400000250LL;
  *(_QWORD *)&num[12] = 0x1C800000700LL;
  *(_QWORD *)&num[14] = 0xA800000240LL;
  *(_QWORD *)&num[16] = 0xD8000007B0LL;
  v3 = "/home/abhi/try2/main.f90";
  v4 = 14;
  v1 = 128;
  v2 = 6;
  _gfortran_st_write(&v1);
  _gfortran_transfer_character_write(&v1, "Enter the flag : you got itWrong", 17LL);
  _gfortran_st_write_done(&v1);
  v3 = "/home/abhi/try2/main.f90";
  v4 = 15;
  v1 = 128;
  v2 = 5;
  _gfortran_st_read(&v1);
  _gfortran_transfer_character(&v1, inp, 18LL);
  _gfortran_st_read_done(&v1);
  for ( n = 1; n <= 18; ++n )
  {
    c1[0] = *(&v10 + n);
    c2[0] = *(&v8 + n);
    x1 = (unsigned __int8)c1[0];
    x2 = (unsigned __int8)c2[0];
    num2[n - 1] = (unsigned __int8)c2[0] ^ (unsigned __int8)c1[0];
  }
  for ( n = 1; n <= 18; ++n )
  {
    if ( (n - 1) % 2 == 1 )
      v0 = 4 * num2[n - 1];
    else
      v0 = 16 * num2[n - 1];
    num1[n - 1] = v0;
  }
  for ( n = 1; n <= 18; ++n )
  {
    if ( num1[n - 1] == num[n - 1] )
      ++f;
  }
  v3 = "/home/abhi/try2/main.f90";
  if ( f == 18 )
```

C1 ends up being the encoding string that we copy at the beginning and C2 is the input we give. These get xored and added into an array which is then used to check against the num1 array which gets instantiated at the start. If all 18 are the same then it print out that we were right. I then wrote the following python script:

```python
# num 1 array
num = [0x00000160, 0x0000006C, 0x00000100, 0x00000054, 0x000002F0, 0x000000A0, 0x00000670, 0x000001E0, 0x000007B0, 0x000000CC, 0x00000250, 0x00000194, 0x00000700, 0x000001C8, 0x00000240, 0x000000A8, 0x000007B0, 0x000000D8]

# c1 array
c1 = [0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4F, 0x50, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4A, 0x4B]

# string for results
s = ""

# loop through 18 times for the flag
for i in range(0, 18):
    # try each combination of characters for each character
	for j in range(32, 128):
        # set x
		x = 0

        # do the if/else that is done in the code
		if i % 2 == 1:
            # do the xor
			x = 4 * (c1[i] ^ j)
		else:
            # do the xor
			x = 16 * (c1[i] ^ j)

        # if it matches then add and break
		if x == num[i]:
			s += chr(j)
			print(s)
            break
```

This then output:

```
G
GL
GLU
GLUG
GLUG{
GLUG{q
GLUG{q2
GLUG{q21
GLUG{q214
GLUG{q214c
GLUG{q214cd
GLUG{q214cd6
GLUG{q214cd64
GLUG{q214cd644
GLUG{q214cd644c
GLUG{q214cd644cb
GLUG{q214cd644cb1
GLUG{q214cd644cb1}
```