# Circle City Con Guardian Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

Looking at the main function I saw:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  const char *v3; 
  const char *v4; 
  size_t v5;
  char *v6;
  __int64 v7; 

  setup(argc, argv, envp);
  v3 = (const char *)getflag();
  if ( !v3 )
    goto LABEL_12;
  v4 = v3;
  v5 = strlen(v3);
  v6 = (char *)calloc(1uLL, v5 + 2);
  if ( !v6 )
    goto LABEL_12;
  __printf_chk(1LL, "%s\n\nHOOOOOOOOOO Goes there? Do you have the password?\n> ", owl);
  if ( !fgets(v6, (int)v5 + 1, stdin) )
    goto LABEL_12;
  if ( v5 )
  {
    v7 = 0LL;
    while ( v6[v7] == v4[v7] )
    {
      ++v7;
      __printf_chk(1LL, byte_200F);
      if ( (v7 & 7) != 0 )
      {
        if ( v7 == v5 )
          goto LABEL_10;
      }
      else
      {
        putchar(10);
        if ( v7 == v5 )
          goto LABEL_10;
      }
    }
    puts("\nHoo hoo hoo!\nThat is incorrect, my guardian.");
LABEL_12:
    exit(0);
  }
LABEL_10:
  puts("\nWe will do our best.....you have fought well.");
  return 0;
}
```

I noticed the checkflag function and took a look at that as well:

```c
char *getflag()
{
  FILE *v0; 
  int v1; 
  __off_t v2;
  char *v3; 
  char *v4;
  void *v6;
  struct stat v7;
  unsigned __int64 v8; 

  v8 = __readfsqword(0x28u);
  v0 = fopen("flag.txt", "r");
  v1 = fileno(v0);
  __fxstat(1, v1, &v7);
  v2 = v7.st_size;
  v3 = (char *)calloc(1uLL, v7.st_size + 2);
  if ( fread(v3, 1uLL, v2 + 1, v0) )
  {
    v4 = &v3[v2];
    if ( *v4 == 10 )
      *v4 = 0;
  }
  else
  {
    v6 = v3;
    v3 = 0LL;
    free(v6);
  }
  fclose(v0);
  return v3;
}
```

What I got from this is that we could connect and simply brute force the flag one character at a time by reading how many check marks were sent, every 8 characters there would be a new line character added which would mess with our script. 

For this one I was fairly lazy and just manually added the new line each time and near the end I ended up with the following:

```python
from pwn import *

#flag = "CCC{let_"
# final flag
flag = "CCC{let_m3_thr0ugh!_let_me_p4ss!_d0_y0u_th1nk_y0u_c4n_h3r?"

# how many items are in the output array when there are no new knowns
cur = 2

# check for the while loop, if nothing new is found then exit, this was mainly for testing
check = True
while (check):
	check = False

  # brute force all the possible characters
	for i in range(32, 127):
		i = chr(i)
		s = remote("35.224.135.84", 2000)

		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		s.sendline(flag + i + "}")
    # these were added each iteration
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		(s.recvline())
		output = str(s.recvline()).split(" ")

    # parse the output
		if len(output) > cur:
			print(output)
			cur = len(output)
			print(cur)
			flag += i
			print(flag)
			check = True
			break
		(s.recvline())
```