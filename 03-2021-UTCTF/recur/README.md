# UTCTF recur Write Up

## Details:
Points: 624

Jeopardy style CTF

Category: reverse engineering

Comment:

I found this binary that is supposed to print flags.

It doesn't seem to work properly though...

## Write up:

Running the file we get the following output:

```
./recur 
utflag{
```

After this the program seems to keep chugging, if left for a bit of time we get one more letter, 0. 

Next I checked what type of file it was and saw that it was a 64 bit ELF file. At this point I started reverse engineering.

The main function was:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char v3; // bl
  unsigned __int8 v4; // al
  int i; // [rsp+Ch] [rbp-14h]

  for ( i = 0; i <= 27; ++i )
  {
    v3 = flag[i];
    v4 = recurrence(i * i);
    putchar(v3 ^ v4);
    fflush(stdout);
  }
  return 0;
}
```

This loops through some flag value and then passes the current place squared to the recurrence function before xor'ing them together.

The recurrence function looked like this:

```c
__int64 __fastcall recurrence(int a1)
{
  int v2; // ebx

  if ( !a1 )
    return 3LL;
  if ( a1 == 1 )
    return 5LL;
  v2 = 2 * recurrence(a1 - 1);
  return v2 + 3 * (unsigned int)recurrence(a1 - 2);
}
```

I then also looked at the values of flag:

```python
flag = [0x76, 0x71, 0xC5, 0xA9, 0xE2, 0x22, 0xD8, 0xB5, 0x73, 0xF1, 0x92, 0x28, 0xB2, 0xBF, 0x90, 0x5A, 0x76, 0x77, 0xFC, 0xA6, 0xB3, 0x21, 0x90, 0xDA, 0x6F, 0xB5, 0xCF, 0x38]
```

As the purpose of this challenge was to complete this program I decided that to do this we would need to optimize the recurrence function. Rather than finding the same numbers over and over again I decided to store all found values in an array and return them. I decided to code this in python since I didn't want to deal with memory allocation in c.

```python
# known flag values
flag = [0x76, 0x71, 0xC5, 0xA9, 0xE2, 0x22, 0xD8, 0xB5, 0x73, 0xF1, 0x92, 0x28, 0xB2, 0xBF, 0x90, 0x5A, 0x76, 0x77, 0xFC, 0xA6, 0xB3, 0x21, 0x90, 0xDA, 0x6F, 0xB5, 0xCF, 0x38]

# string to store flag in
s = ""

# known return values
nums = [3, 5]

# modified recurrence function
def recurrence(i):

    # if i is in the array then return the found value
    if i < len(nums):
        return nums[i]

    # otherwise do the normal stuff
    x = 2*recurrence(i-1)
    ret = x + 3* recurrence(i-2)

    # if i is the next number in the array then add it
    if i == len(nums):
        nums.append(ret)

    # return the value 
    return ret

# loop through the flag
for i in range(0,28):
    # loop through the values from i to i*i -1
	for j in range(i, i*i):
        # add all values to the array
		recurrence(j)
    # add value to string, we need to and the result of recurrence with 0xFF 
    # since char only accepts 2 hex values
	s += chr(flag[i]^(recurrence(i*i)&0xFF))

# print the string
print(s)
```

When run we get the flag:

```
utflag{0pt1m1z3_ur_c0d3_l0l}
```