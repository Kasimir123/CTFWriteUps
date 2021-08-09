# RaRCTF Very TriVial ReVersing Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 300

Comments: Hey, at least it's not Haskell!

## Write up:

Looking into the main function of the program I saw a call to a function called main__main:

```c
__int64 main__main()
{
  __int64 v0;
  __int64 result; 
  __int64 v2[4]; 
  __int64 v3[2]; 

  v2[2] = (__int64)L_1252;
  v2[3] = 0x10000000ELL;
  v2[0] = os__input(L_1252, 0x10000000ELL);
  v2[1] = v0;
  memmove_plt(v3, v2, 16LL);
  result = (unsigned __int8)main__check(v3[0], v3[1]);
  if ( (_BYTE)result )
    result = println(L_1254, 0x100000003LL);
  return result;
}
```

This function read the user input into v2 as a pointer and then called main__check:

```c
__int64 __fastcall main__check(__int64 a1, __int64 a2)
{
  int v2; 
...
  __int64 v57;

  v56 = a1;
  v57 = a2;
  _new_array_with_default(v54, 0LL, 0LL, 1LL, 0LL);
  memmove_plt(v55, v54, 32LL);
  v51[0] = 19;
  v51[1] = 55;
  new_array_from_c_array(v52, 2LL, 2LL, 4LL, v51);
  memmove_plt(v53, v52, 32LL);
  for ( i = 0; i < (int)v57; ++i ) // loop through the length of the input
  {
    v49 = *(_BYTE *)(i + v56);
    v47 = &v29;
    memmove_plt(&v29, v53, 32LL);
    v6 = (_DWORD *)array_get(0, (unsigned int)v53, v2, v3, v4, v5, v29, v30, v31); // get a value
    v46 = *v6 ^ v49; // xor that value
    v47 = &v29;
    memmove_plt(&v29, v53, 32LL);
    v11 = (_DWORD *)array_get(1, (unsigned int)v53, v7, v8, v9, v10, v29, v30, v31); // get another value
    v48 = *v11 + v46; // add that value
    array_push(v55, &v48);
    v47 = &v29;
    memmove_plt(&v29, v53, 32LL);
    v44[0] = *(_DWORD *)array_get(1, (unsigned int)v53, v12, v13, v14, v15, v29, v30, v31);
    v47 = &v29;
    memmove_plt(&v29, v53, 32LL);
    v44[1] = *(_DWORD *)array_get(0, (unsigned int)v53, v16, v17, v18, v19, v29, v30, v31);
    new_array_from_c_array(v45, 2LL, 2LL, 4LL, v44); // swap the locations of the two values
    memmove_plt(v53, v45, 32LL);
  }
  memmove_plt(v41, v55, 32LL);
  v40 = v43;
  _new_array(v38, 0LL, v43, 4LL);
  memmove_plt(v39, v38, 32LL);
  for ( j = 0; j < v40; ++j )
  {
    v36 = *(_BYTE *)(j + v42);
    v35 = anon_fn_e2d96d4126f6333f_byte__int_215(v36);
    array_push(v39, &v35);
  }
  v33[0] = 152; // create encrypted flag
  v33[1] = 105;
  v33[2] = 152;
  v33[3] = 103;
  v33[4] = 158;
  v33[5] = 100;
  v33[6] = 159;
  v33[7] = 119;
  v33[8] = 173;
  v33[9] = 101;
  v33[10] = 118;
  v33[11] = 118;
  v33[12] = 178;
  v33[13] = 105;
  v33[14] = 158;
  v33[15] = 115;
  v33[16] = 169;
  v33[17] = 87;
  v33[18] = 180;
  v33[19] = 35;
  v33[20] = 158;
  v33[21] = 119;
  v33[22] = 179;
  v33[23] = 146;
  v33[24] = 169;
  v33[25] = 88;
  v33[26] = 174;
  v33[27] = 45;
  v33[28] = 89;
  v33[29] = 101;
  v33[30] = 168;
  v33[31] = 21;
  v33[32] = 89;
  v33[33] = 33;
  v33[34] = 173;
  v33[35] = 102;
  v33[36] = 165;
  new_array_from_c_array(v34, 37LL, 37LL, 4LL, v33);
  v47 = &v29;
  memmove_plt(&v29, v34, 32LL);
  v47 = &v25;
  memmove_plt(&v25, v39, 32LL);
  if ( !(unsigned __int8)Array_int_arr_eq(
                           (unsigned int)&v25,
                           (unsigned int)v39,
                           v20,
                           v21,
                           v22,
                           v23,
                           v25,
                           v26,
                           v27,
                           v28,
                           v29,
                           v30,
                           v31) ) // check the flag
    return 0LL;
  v32 = 1;
  return 1LL;
}
```

The program went through the entire input we had and xor'ed it by a, then added by b, then swapped a and b for the next one. From this I made the following script:

```python
# encrypted flag
x = [152, 105, 152, 103, 158, 100, 159, 119, 173, 101, 118, 118, 178, 105, 158, 115, 169, 87, 180, 35, 158, 119, 179, 146, 169, 88, 174, 45, 89, 101, 168, 21, 89, 33, 173, 102, 165]

# a and b
a = 0x13
b = 0x37

s = ""

# go through length of encrypted bytes
for i in range(len(x)):

    # subtract b and xor a
	s += chr(0xFF & ((x[i] - b) ^ a))

    # swap a and b
	c = a
	a = b
	b = c

# print flag
print(s)
```

Once run I got:

```
rarctf{See,ThatWasn'tSoHard-1eb519ed}
```