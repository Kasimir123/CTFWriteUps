# Securinet Little Baby Rev Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

Running the file we see that we are in an infinite loop:

```
./warmup 
I have a number from 1 to 10, what is it? 
2
Try again
I have a number from 1 to 10, what is it? 
6
```

Looking for these strings in the binary does not prove useful at all either but when I ctrl-c'd I noticed that it seemed to be running in a .nim file:

```
Try again
I have a number from 1 to 10, what is it? 
^CTraceback (most recent call last)
/root/rev/warmup.nim(39) warmup
/root/Nim/lib/system/io.nim(491) readLine
```

Instead of going down the rabbit hole of trying to figure out what nim is I decided to do some dynamic analysis.

I found the following function which seemed to be the while loop that I wanted:

```c
 while ( 1 )
  {
    v5 = 38LL;
    v6 = "/root/rev/warmup.nim";
    nimZeroMem_0(&v10, 8LL);
    v10 = decodeStr__R9b5IlyQjG2mcdkp9a67LGTQ((__int64)&TM__ijE9cayl8YPnol3rizbiT0g_5, 0x3F1997CCu);
    echoBinSafe(&v10, 1LL);
    asgnRef_1(&guess__62AlRyOQv9cCViqvgI14ssA, 0LL);
    v5 = 39LL;
    v6 = "/root/rev/warmup.nim";
    v1 = readLine__IfmAdseskhTUnfEYpOo5fA(stdin);
    asgnRef_1(&guess__62AlRyOQv9cCViqvgI14ssA, v1);
    v5 = 41LL;
    v6 = "/root/rev/warmup.nim";
    if ( (unsigned __int8)eqStrings(guess__62AlRyOQv9cCViqvgI14ssA, answer__2bKjAtEJJ5cp19bpmXcStjQ) )
      break;
    v5 = 42LL;
    v6 = "/root/rev/warmup.nim";
    nimZeroMem_0(&v9, 8LL);
    v9 = decodeStr__R9b5IlyQjG2mcdkp9a67LGTQ((__int64)&TM__ijE9cayl8YPnol3rizbiT0g_7, 0x2149F624u);
    echoBinSafe(&v9, 1LL);
  }
```

After reading in my string it compared it against the answer__2bKjAtEJJ5cp19bpmXcStjQ so I took a look at that memory while the program was running.

I saw that that contained:

```
7FD1C85020A0
```

And decided to look at the memory that this pointed to where I saw the following:

```
 53 65 63 75 72 69 6E 65  74 73 7B 4E 69 6D 70 65  Securinets{Nimpe
 72 30 72 5F 54 68 65 5F  61 4E 69 6D 41 74 6F 72  r0r_The_aNimAtor
 7D 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  }...............
 ```

 The flag was:

 ```
 Securinets{Nimper0r_The_aNimAtor}
 ```