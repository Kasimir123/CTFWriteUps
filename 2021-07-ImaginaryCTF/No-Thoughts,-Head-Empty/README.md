# ImaginaryCTF No Thoughts, Head Empty Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 200

Comments: 

When I was making Roolang, of course I took a look at the mother of all esolangs! So, have some bf code. Run it here (https://copy.sh/brainfuck/) with 32 bit cells and dynamic memory enabled. Run the program to get the flag, and then some.

## Write up:

When we first ran the brainfuck we got that the output was:

```
iccttttffffffff{{{{{{{{{{{{{{{{00000000000000000000000000000000nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnÿ
```

We then removed the following brainfuck from the end of the code:

```bf
[>[->+>++<<]
>>[-<<+>>]<[[<]<[<]>.[>]>[>]<-][<]<[<]<[<]>[-]>[>]>-]
```

When looking at the memory dump we then saw:

```
00000:  000  105  099  116  102  123  048  110  051  095  099  104  .ictf{0n3_ch
00012:  064  114  064  099  116  051  114  095  048  102  095  100  @r@ct3r_0f_d
00024:  049  102  051  114  051  110  099  101  125  000  032  001  1f3r3nce}...
```

The flag was:

```
ictf{0n3_ch@r@ct3r_0f_d1f3r3nce}
```