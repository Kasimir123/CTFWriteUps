# Angstrom Infinity Gauntlet Write Up

## Details:
Points: 75

Jeopardy style CTF

Category: Reverse Engineering

Comments: 

All clam needs to do is snap and finite will turn into infinite...

Find it on the shell server at /problems/2021/infinity_gauntlet or over netcat at nc shell.actf.co 21700

## Write up:

Reversing the main function we get:

```c
  v32 = __readfsqword(0x28u);
  setvbuf(_bss_start, 0LL, 2, 0LL);
  v3 = fopen("flag.txt", "r");
  if ( v3 )
  {
    v4 = v3;
    fgets(s, 256, v3);
    fclose(v4);
    v5 = strcspn(s, "\n");
    v6 = v5;
    s[v5] = 0;
    if ( v5 )
    {
      v7 = s;
      v8 = 17 * v5;
      v9 = 0;
      do
      {
        *v7 ^= v9;
        v9 += 17;
        ++v7;
      }
      while ( (_BYTE)v9 != v8 );
    }
    v10 = 1;
    v11 = time(0LL);
    srand(v11);
    puts("Welcome to the infinity gauntlet!");
    puts("If you complete the gauntlet, you'll get the flag!");
    while ( 1 )
    {
      printf("=== ROUND %d ===\n", (unsigned int)v10);
      v14 = rand();
      v15 = v10 > 49 ? (unsigned __int8)s[v14 % v6] | ((unsigned __int8)(v14 % v6 + v10) << 8) : rand() % 0x10000;
      if ( (rand() & 1) != 0 )
      {
        v12 = rand() % 3;
        if ( v12 )
        {
          if ( v12 == 1 )
          {
            v22 = rand();
            printf("foo(%u, ?) = %u\n", (unsigned int)(v22 % 1337), (v22 % 1337) ^ (v15 + 1) ^ 0x539);
          }
          else
          {
            v13 = rand();
            printf("foo(%u, %u) = ?\n", v15 ^ (v13 % 1337 + 1) ^ 0x539, (unsigned int)(v13 % 1337));
          }
        }
        else
        {
          v19 = rand();
          printf("foo(?, %u) = %u\n", (unsigned int)(v19 % 1337), v15 ^ (v19 % 1337 + 1) ^ 0x539);
        }
      }
      else
      {
        v16 = rand();
        if ( (v16 & 3) != 0 )
        {
          v17 = v16 % 4;
          if ( v17 == 1 )
          {
            v24 = rand() % 1337;
            v25 = rand();
            v26 = (unsigned int)(v25 >> 31);
            LODWORD(v26) = v25 % 1337;
            printf("bar(%u, ?, %u) = %u\n", v24, v26, v24 + v15 * (v25 % 1337 + 1));
          }
          else if ( v17 == 2 )
          {
            v27 = rand() % 1337;
            v28 = rand();
            v29 = (unsigned int)(v28 >> 31);
            LODWORD(v29) = v28 % 1337;
            printf("bar(%u, %u, ?) = %u\n", v27, v29, v27 + v28 % 1337 * (v15 + 1));
          }
          else
          {
            v18 = v15 <= 0x539 ? rand() % v15 : rand() % 1337;
            printf("bar(%u, %u, %u) = ?\n", v15 % v18, v18, v15 / v18 - 1);
          }
        }
        else
        {
          v20 = rand() % 1337;
          v21 = rand();
          printf("bar(?, %u, %u) = %u\n", v20, (unsigned int)(v21 % 1337), v15 + v20 * (v21 % 1337 + 1));
        }
      }
      __isoc99_scanf("%u", &v30);
      if ( v30 != v15 )
        break;
      printf("Correct! Maybe round %d will get you the flag ;)\n", (unsigned int)++v10);
    }
    puts("Wrong!");
    result = 0;
  }
```

The first thing I did was reverse the various foo and bar functions to figure out what value I needed to type:

```python
def foo1(a, c):
	return (a^c^0x539)-1

def foo2(a, b):
	return a^(b+1)^0x539

def foo3(b, c):
	return (b+1)^c^0x539

def bar1(a, c, d):
	return (d-a)/((c&0xFFF)+1)

def bar2(a, b, d):
	if b == 0:
		return -1
	return ((d-a)/((b&0xFFF)))-1


def bar3(a, b, c):
	return ((c+1)*b)+a

def bar4(b, c, d):
	return d-(b*(c+1))
```

v15 was the value that we got each time, this value was set on this line:

```c
v15 = v10 > 49 ? (unsigned __int8)s[v14 % v6] | ((unsigned __int8)(v14 % v6 + v10) << 8) : rand() % 0x10000;
```

Using this we can get the original value from s as well as the position value. So if we do a bunch of iterations of the problems we can theoretically solve for all the values.

s is the encrypted flag that gets set here:

```c
    v5 = strcspn(s, "\n");
    v6 = v5;
    s[v5] = 0;
    if ( v5 )
    {
      v7 = s;
      v8 = 17 * v5;
      v9 = 0;
      do
      {
        *v7 ^= v9;
        v9 += 17;
        ++v7;
      }
      while ( (_BYTE)v9 != v8 );
    }
```

Which can be reversed fairly simply by using the position value plus 1 times 17 xor'ed with the value we received. I then made the following script:

```python
# pwntools import
from pwn import *

# reversed functions
def foo1(a, c):
	return (a^c^0x539)-1

def foo2(a, b):
	return a^(b+1)^0x539

def foo3(b, c):
	return (b+1)^c^0x539

def bar1(a, c, d):
	return (d-a)/((c&0xFFF)+1)

def bar2(a, b, d):
	if b == 0:
		return -1
	return ((d-a)/((b&0xFFF)))-1

def bar3(a, b, c):
	return ((c+1)*b)+a

def bar4(b, c, d):
	return d-(b*(c+1))

# open remote shell
r = remote('shell.actf.co', 21700)

print(r.recvline())
print(r.recvline())

i = 1

solved = []

clen = 50

new4 = [0 for i in range(0, clen+1)]

while True:
	if i > 30000:
		break


	r.recvline()
	x = r.recvline()
	t = x.decode().split('(')[1].split(')')[0]
	t=t.replace(' ','').split(',')
	t.append(x[x.decode().index('= ')+2:x.decode().index('\n')].decode())
	ind = t.index('?')
	ans = 0

    # answer the question
	if 'foo' in x.decode():
		if ind == 0:
			ans = foo3(int(t[1]),int(t[2]))
		elif ind == 1:
			ans = foo1(int(t[0]),int(t[2]))
		else:
			ans = foo2(int(t[0]),int(t[1]))
	else:
		if ind == 0:
			ans = bar4(int(t[1]), int(t[2]), int(t[3]))
		elif ind == 1:
			ans = bar1(int(t[0]), int(t[2]), int(t[3]))
		elif ind == 2:
			ans = bar2(int(t[0]), int(t[1]), int(t[3]))
		else:
			ans = bar3(int(t[0]), int(t[1]), int(t[2]))
	r.send(str(int(math.ceil(ans)+2**32)) + '\n')
	r.recvline()
	i += 1

    # if i greater than 50, this is a check on their side
	if i > 50:
		ans = int(ans)

        # get position
		a = ((((((ans & 0xFF00) >> 8)))&0xFF)-i)

        # if not already in the solved
		if a > 0 and a < clen and a not in solved:
			solved.append(a)

            # get encrypted value
			b = ((ans | ((a)+i) << 8)&0x00FF)

            # decrypt and print
			solved.append(chr(0xFF&(((a+1)*17)^b)))
			new4[a] = chr(0xFF&(((a+1)*17)^b))
			print(new4)
```

After letting it run for a few seconds we get:

```
[+] Opening connection to shell.actf.co on port 21700: Done
b'Welcome to the infinity gauntlet!\n'
b"If you complete the gauntlet, you'll get the flag!\n"
[0, 0, 0, '{', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 0, 0, '{', 0, 0, 0, 0, 'p', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 0, 0, '{', 0, 0, 0, 0, 'p', 0, 0, 0, 'a', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 0, 0, '{', 0, 0, 0, 0, 'p', 0, 0, 0, 'a', 0, 'a', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 0, 0, '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 0, 'a', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 0, '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 0, 'a', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 0, 'a', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 0, 'a', 0, 0, 0, 0, 0, 0, 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 0, 'a', 'y', 0, 0, 0, 0, 0, 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 0, 'a', 'y', '_', 0, 0, 0, 0, 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 'w', 'a', 'y', '_', 0, 0, 0, 0, 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 'w', 'a', 'y', '_', 0, 0, 0, '_', 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 0, 'p', 'e', 0, 0, 'a', 'w', 'a', 'y', '_', 't', 0, 0, '_', 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 'p', 'p', 'e', 0, 0, 'a', 'w', 'a', 'y', '_', 't', 0, 0, '_', 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 'p', 'p', 'e', 0, '_', 'a', 'w', 'a', 'y', '_', 't', 0, 0, '_', 0, 'n', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 'p', 'p', 'e', 0, '_', 'a', 'w', 'a', 'y', '_', 't', 0, 0, '_', 0, 'n', 0, '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 0, 0, 0, 'p', 'p', 'e', 0, '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 0, '_', 0, 'n', 0, '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 's', 0, 0, 'p', 'p', 'e', 0, '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 0, '_', 0, 'n', 0, '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 's', 0, 0, 'p', 'p', 'e', 0, '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 'e', '_', 0, 'n', 0, '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 's', 0, 0, 'p', 'p', 'e', 'd', '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 'e', '_', 0, 'n', 0, '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 's', 'n', 0, 'p', 'p', 'e', 'd', '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 'e', '_', 0, 'n', 0, '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 's', 'n', 'a', 'p', 'p', 'e', 'd', '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 'e', '_', 0, 'n', 0, '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 's', 'n', 'a', 'p', 'p', 'e', 'd', '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 'e', '_', 0, 'n', 'd', '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
[0, 't', 'f', '{', 's', 'n', 'a', 'p', 'p', 'e', 'd', '_', 'a', 'w', 'a', 'y', '_', 't', 'h', 'e', '_', 'e', 'n', 'd', '}', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

```

The flag ended up being:

```
actf{snapped_away_the_end}
```