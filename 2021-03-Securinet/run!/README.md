# Securinet RUN! Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

keygenme now!

```
nc bin.q21.ctfsecurinets.com 2324
```

NOTE: you may encounter some buffering issues while communicating with the remote server. This won't affect the challenge behaviour

## Write up:

This was a pretty fun challenge and took me longer than it should have because I overlooked something after having decided one section was unecessary. So rather than giving my solve outright I'm going to walk through the logic for how I solved this. In the code snippets im posting a lot of variables have been renamed and I added a good bit of comments.

All of the codes logic seemed to be in the main function which was nice. The first thing I took note of was the two inputs the program took, the username and the key:

```c
sub_7FF6BB101BF0(std::cout, (__int64)"Username: ");
sub_7FF6BB101FF0(std::cin, v56);
sub_7FF6BB101BF0(std::cout, (__int64)"Key: ");
sub_7FF6BB101FF0(std::cin, keyinput);
```

The next component I looked at was where the program read in the flag:

```c
  if ( v40 >= 0 )
  {
    for ( j = 0i64; j <= v40; ++j )
      v41[j] = rand() % 13371337;  // author has a sense of humor
  }
  v43 = 0;
  if ( v37 )
  {
    do
    {
      v44 = (v3 + v41[*v23]) % 13371337;
      v3 = v44;
      ++v43;
      ++v23;
    }
    while ( v43 < v37 );
    if ( v44 == 773558 )
    {
      v45 = fopen("./flag.txt", "r");
      fgets(Buffer, 50, v45);
      puts(Buffer);
      fclose(v45);
```

If v40 was greater than or equal to 0 we would loop through that number and add that many random values to an array. Afterwards, if v37 was not 0, it would loop through the array and add those values together using the values in v23 as indicies. This is then compared to 773558 and if it is the same value then we are given the flag.

Looking through the code I saw that v40 was being set by the key, so if the first two values of the key were 99 then v40 became 0x99.

I then looked for any more requirements on the key and found this:

```c
 if ( (v54 & 1) != 0 )
  {
    v21 = sub_7FF6BB101BF0(std::cout, (__int64)":(");
    std::ostream::operator<<(v21, sub_7FF6BB101DC0);
    exit(0);
  }
```

The key had to be an even number of characters but could be any length, this makes sense since it parses as hex values. 

Now, for some reason I did not notice the seeding of random right above this section for longer than I should probably admit but anyways, next I looked at this section of code:

```c
  afterusername = (unsigned int *)((char *)v7 + v57);
  v9 = 0i64;
  usernameLength = (char *)afterusername - (char *)beginofusername;
  if ( beginofusername > afterusername )
    usernameLength = 0i64;
  if ( usernameLength && usernameLength >= 8 )  // if username is longer than 8
  {
    v11 = usernameLength & '\xF8';              // 11111000
    v12 = (__m128i)'\0';
    v13 = (__m128i)'\0';
    do
    {
      v14 = _mm_cvtsi32_si128(*beginofusername);
      v15 = _mm_unpacklo_epi8(v14, v14);
      v12 = _mm_add_epi32(_mm_srai_epi32(_mm_unpacklo_epi16(v15, v15), 0x18u), v12);
      v16 = _mm_cvtsi32_si128(beginofusername[1]);
      v17 = _mm_unpacklo_epi8(v16, v16);
      v18 = _mm_add_epi32(_mm_srai_epi32(_mm_unpacklo_epi16(v17, v17), 0x18u), v13);
      v13 = v18;
      beginofusername += 2;
      v9 += '\b';
    }
    while ( v9 != v11 );
    v19 = _mm_add_epi32(v12, v18);
    v20 = _mm_add_epi32(v19, _mm_srli_si128(v19, 8));
    v4 = _mm_cvtsi128_si32(_mm_add_epi32(v20, _mm_srli_si128(v20, 4)));
  }
  for ( ; beginofusername != afterusername; beginofusername = (unsigned int *)((char *)beginofusername + 1) )
    v4 += *(char *)beginofusername;
  srand(v4);
```

The program did a bunch of operations on the username and then turned that into a value to seed the random with, after some testing I saw that this was deterministic so I didn't need to worry about it.

Next thing I did was select a large number for the first value, just did 99, so that I could generate a lot of the seed values from which I got:

```python
[0x00000B2C, 0x00005206, 0x0000568A, 0x00004734, 0x00001E47, 0x000072CB, 0x00002933, 0x00006690, 0x00005770, 0x000069D9, 0x000023AE, 0x0000026D, 0x0000632B, 0x000024F3, 0x000062C5, 0x0000523F, 0x000031AB, 0x00004FCE, 0x00002E62, 0x00005D1B, 0x00005144, 0x000014BD, 0x000047A2, 0x00005F76, 0x00004D23, 0x0000508E, 0x00005D2A, 0x00003E30, 0x0000643D, 0x00003DAE, 0x000019BE, 0x00002AD1, 0x00002C85, 0x00005A94, 0x00000110, 0x00005D12, 0x00007662, 0x00000F5D, 0x00000ED5, 0x0000059F, 0x00000460, 0x000056B4, 0x00004C1E, 0x00005E1B, 0x00006B26, 0x00006DF5, 0x00004EE6, 0x00007A64, 0x00002DA8, 0x00005092, 0x000057E7, 0x00003F19, 0x0000699D, 0x000002B5, 0x000002B3, 0x0000215A, 0x00002834, 0x00006F27, 0x00003BFF, 0x000013CF, 0x00002702, 0x00005C70, 0x00007242, 0x000044EC, 0x00002D3F, 0x00003D40, 0x0000347F, 0x00001273, 0x00004133, 0x0000600D, 0x0000335E, 0x0000563A, 0x000051EB, 0x000041FE, 0x00005882, 0x0000702E, 0x00002930, 0x00000908, 0x00006817, 0x00003198, 0x000039BA, 0x00006957, 0x000060AA, 0x00000D9F, 0x00001DA0, 0x000079EF, 0x00000D3E, 0x0000330E, 0x00005913, 0x00003C93, 0x00007D9C, 0x00007358, 0x0000354C, 0x00005CE5, 0x000058EA, 0x00001ADA, 0x000047C3, 0x00006AD0, 0x00003E7F, 0x00001E5F, 0x000079A2, 0x00001423, 0x000028F6, 0x000041E9, 0x00002379, 0x0000217E, 0x00000781, 0x00001CAF, 0x00001133, 0x00003A72, 0x00000180, 0x00001E62, 0x00000249, 0x000034E4, 0x00001853, 0x000079B4, 0x00007A35, 0x000073B2, 0x00001B6C, 0x0000181B, 0x0000752A, 0x0000189A, 0x000022A8, 0x00007AD2, 0x00005502, 0x00000D57, 0x000002DF, 0x00002D22, 0x00001A78, 0x0000280C, 0x000070BA, 0x00002BDF, 0x00003E97, 0x00006EE6, 0x000045C4, 0x00006635, 0x00004072, 0x00003EFD, 0x00001BC4, 0x00003BA5, 0x00003B16, 0x00005A7C, 0x0000324B, 0x00001B4A, 0x000017BD, 0x00002202, 0x00005289, 0x00002861, 0x00003044, 0x00007D48, 0x00002564, 0x00000808, 0x000075DF, 0x3703]
``` 

I then wrote a little script to check which of these values could be used to make the value we needed. I did this by looping through twice and seeing which values equaled another if modded:

```python
# final value
finVal = 773558

# all seeded values
x = [0x00000B2C, 0x00005206, 0x0000568A, 0x00004734, 0x00001E47, 0x000072CB, 0x00002933, 0x00006690, 0x00005770, 0x000069D9, 0x000023AE, 0x0000026D, 0x0000632B, 0x000024F3, 0x000062C5, 0x0000523F, 0x000031AB, 0x00004FCE, 0x00002E62, 0x00005D1B, 0x00005144, 0x000014BD, 0x000047A2, 0x00005F76, 0x00004D23, 0x0000508E, 0x00005D2A, 0x00003E30, 0x0000643D, 0x00003DAE, 0x000019BE, 0x00002AD1, 0x00002C85, 0x00005A94, 0x00000110, 0x00005D12, 0x00007662, 0x00000F5D, 0x00000ED5, 0x0000059F, 0x00000460, 0x000056B4, 0x00004C1E, 0x00005E1B, 0x00006B26, 0x00006DF5, 0x00004EE6, 0x00007A64, 0x00002DA8, 0x00005092, 0x000057E7, 0x00003F19, 0x0000699D, 0x000002B5, 0x000002B3, 0x0000215A, 0x00002834, 0x00006F27, 0x00003BFF, 0x000013CF, 0x00002702, 0x00005C70, 0x00007242, 0x000044EC, 0x00002D3F, 0x00003D40, 0x0000347F, 0x00001273, 0x00004133, 0x0000600D, 0x0000335E, 0x0000563A, 0x000051EB, 0x000041FE, 0x00005882, 0x0000702E, 0x00002930, 0x00000908, 0x00006817, 0x00003198, 0x000039BA, 0x00006957, 0x000060AA, 0x00000D9F, 0x00001DA0, 0x000079EF, 0x00000D3E, 0x0000330E, 0x00005913, 0x00003C93, 0x00007D9C, 0x00007358, 0x0000354C, 0x00005CE5, 0x000058EA, 0x00001ADA, 0x000047C3, 0x00006AD0, 0x00003E7F, 0x00001E5F, 0x000079A2, 0x00001423, 0x000028F6, 0x000041E9, 0x00002379, 0x0000217E, 0x00000781, 0x00001CAF, 0x00001133, 0x00003A72, 0x00000180, 0x00001E62, 0x00000249, 0x000034E4, 0x00001853, 0x000079B4, 0x00007A35, 0x000073B2, 0x00001B6C, 0x0000181B, 0x0000752A, 0x0000189A, 0x000022A8, 0x00007AD2, 0x00005502, 0x00000D57, 0x000002DF, 0x00002D22, 0x00001A78, 0x0000280C, 0x000070BA, 0x00002BDF, 0x00003E97, 0x00006EE6, 0x000045C4, 0x00006635, 0x00004072, 0x00003EFD, 0x00001BC4, 0x00003BA5, 0x00003B16, 0x00005A7C, 0x0000324B, 0x00001B4A, 0x000017BD, 0x00002202, 0x00005289, 0x00002861, 0x00003044, 0x00007D48, 0x00002564, 0x00000808, 0x000075DF, 0x3703]

# loop through twice
for i in range(0, len(x)):
	for j in range(0, len(x)):
        # mod one to see if the other is good
		if finVal % x[i] == x[j]:
            # print index
			print(i, j)

```

This then output:

```
25 76
```

I then wrote a program to output the key needed for the username I wrote. I knew the larger number would have to be first so that all the necessary values would be generated and that 37 times the second value after subtracting the 1st would give me what I needed:

```python
# final value
finVal = 773558

# all seeded values
x = [0x00000B2C, 0x00005206, 0x0000568A, 0x00004734, 0x00001E47, 0x000072CB, 0x00002933, 0x00006690, 0x00005770, 0x000069D9, 0x000023AE, 0x0000026D, 0x0000632B, 0x000024F3, 0x000062C5, 0x0000523F, 0x000031AB, 0x00004FCE, 0x00002E62, 0x00005D1B, 0x00005144, 0x000014BD, 0x000047A2, 0x00005F76, 0x00004D23, 0x0000508E, 0x00005D2A, 0x00003E30, 0x0000643D, 0x00003DAE, 0x000019BE, 0x00002AD1, 0x00002C85, 0x00005A94, 0x00000110, 0x00005D12, 0x00007662, 0x00000F5D, 0x00000ED5, 0x0000059F, 0x00000460, 0x000056B4, 0x00004C1E, 0x00005E1B, 0x00006B26, 0x00006DF5, 0x00004EE6, 0x00007A64, 0x00002DA8, 0x00005092, 0x000057E7, 0x00003F19, 0x0000699D, 0x000002B5, 0x000002B3, 0x0000215A, 0x00002834, 0x00006F27, 0x00003BFF, 0x000013CF, 0x00002702, 0x00005C70, 0x00007242, 0x000044EC, 0x00002D3F, 0x00003D40, 0x0000347F, 0x00001273, 0x00004133, 0x0000600D, 0x0000335E, 0x0000563A, 0x000051EB, 0x000041FE, 0x00005882, 0x0000702E, 0x00002930, 0x00000908, 0x00006817, 0x00003198, 0x000039BA, 0x00006957, 0x000060AA, 0x00000D9F, 0x00001DA0, 0x000079EF, 0x00000D3E, 0x0000330E, 0x00005913, 0x00003C93, 0x00007D9C, 0x00007358, 0x0000354C, 0x00005CE5, 0x000058EA, 0x00001ADA, 0x000047C3, 0x00006AD0, 0x00003E7F, 0x00001E5F, 0x000079A2, 0x00001423, 0x000028F6, 0x000041E9, 0x00002379, 0x0000217E, 0x00000781, 0x00001CAF, 0x00001133, 0x00003A72, 0x00000180, 0x00001E62, 0x00000249, 0x000034E4, 0x00001853, 0x000079B4, 0x00007A35, 0x000073B2, 0x00001B6C, 0x0000181B, 0x0000752A, 0x0000189A, 0x000022A8, 0x00007AD2, 0x00005502, 0x00000D57, 0x000002DF, 0x00002D22, 0x00001A78, 0x0000280C, 0x000070BA, 0x00002BDF, 0x00003E97, 0x00006EE6, 0x000045C4, 0x00006635, 0x00004072, 0x00003EFD, 0x00001BC4, 0x00003BA5, 0x00003B16, 0x00005A7C, 0x0000324B, 0x00001B4A, 0x000017BD, 0x00002202, 0x00005289, 0x00002861, 0x00003044, 0x00007D48, 0x00002564, 0x00000808, 0x000075DF, 0x3703]

# key
key = [76]

# add to key
[key.append(25) for b in range(0, 37)]

# string to print to
s = ""

# create key
for x in key:
	s += str(hex(x))[2:]

# print key
print(s)
```

This then output:

```
4c19191919191919191919191919191919191919191919191919191919191919191919191919
```

I then connected to the nc instance and used my username and key:

```
nc bin.q21.ctfsecurinets.com 2324

username
4c19191919191919191919191919191919191919191919191919191919191919191919191919
Username: Key: flag{fda7a71c8778db5771fc176b0cb62247}
```

The flag was:

```
flag{fda7a71c8778db5771fc176b0cb62247}
```