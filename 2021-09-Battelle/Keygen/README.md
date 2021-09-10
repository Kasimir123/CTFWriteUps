# Battelle Keygen Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 100

Comments:

```
One of our employees doesn't trust "Big Crypto" and decided to roll his own key generator, claiming it was impossible ot predict! See if you can crack his code... the Flag will be the input that produces the requested password

The solution will be in ALL CAPS
```

## Write up:

Opening the main function in a decompiler I saw:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  void *v3; // esp
  size_t v5; // [esp+2Ch] [ebp-41Ch]
  char Destination[16]; // [esp+30h] [ebp-418h] BYREF
  char Buffer[1032]; // [esp+40h] [ebp-408h] BYREF

  v3 = alloca(16);
  __main();
  printf("Name/Password Generator:\n");
  printf("1) Figure out the algorithm\n");
  printf("2) Name: ?    Password: AFMWAFPE\n");
  do
  {
    printf("Name: ");
    fgets(Buffer, 1024, (FILE *)__iob[0]._ptr);
    Destination[strlen(Buffer) + 15] = 0;
  }
  while ( !Buffer[0] );
  Sleep(0x5DCu);
  strncpy(Destination, Buffer, 8u);
  Destination[8] = 0;
  v5 = strlen(Buffer);
  computePassword(Destination, v5);
  return 0;
}
```

From here I could see that our username will always be 8 characters since that is all the program looks at. I then went into the computePassword function:

```c
int __cdecl computePassword(_BYTE *a1, char a2)
{
  int v2; // ecx

  data[0] = (unsigned __int8)(122 - *a1) % 0x1Au + 65;
  v2 = 1;
  do
  {
    data[v2] = (unsigned __int8)(a2 + a1[v2] + v2 + data[v2] - data[v2 - 1]) % 0x1Au + 65;
    ++v2;
  }
  while ( (v2 & 8) == 0 );
  Sleep(0x5DCu);
  return printf("Password: %s\n", data);
}
```

I also extracted the data array:

```
ABCDEFGH
```

Looking at the above function I could tell that it modified the data based on the input name and the previously solved for password character for the second character on and simply subtracted 122 by the first character, modding it, and then adding 65 to make it uppercase for the first character. The program then printed out data as the encrypted password.

Knowing that our goal was to get the password: AFMWAFPE, I decided to make a z3py script to solve for all the possible usernames that get use that password:

```py
# original data values
og = [0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48]

# end values that we want to get to
end = [0x41, 0x46, 0x4D, 0x57, 0x41, 0x46, 0x50, 0x45]

# import z3py
from z3 import *

# create a flag of length 8
flag = [BitVec(f'flag[{i}]', 32) for i in range(0,8)]

# length of flag - put this separately because I wanted to do some tests for shortening list of possible values
length = 8

# initialize solver
s = Solver()

# loop through all flag characters and assign what characters they can and cannot be - added some !='s because there were too many potential flags
for i in range(0, len(flag)):

	s.add(flag[i] >= 33)
	s.add(flag[i] <= 122)
	s.add(flag[i] != 46)
	s.add(flag[i] != 44)
	s.add(flag[i] != 91)
	s.add(flag[i] != 92)
	s.add(flag[i] != 93)
	s.add(flag[i] != 58)
	s.add(flag[i] != 59)
	s.add(flag[i] != 47)
	s.add(flag[i] != 96)
	s.add(flag[i] != 39)

# add the first character calculation
s.add(end[0] == ((122 - flag[0]) % 26) + 65)

# loop through and add the rest of the conditions
for i in range(1,8):
	s.add(end[i] == ((length + flag[i] + i + og[i] - end[i-1]) % 26) + 65)

# array for possible flags
flags = []

# loop through all satisfiable models
while s.check() == sat:
	m = s.model()

	sFlag = ""

	# convert model to flag and print
	for i in range(0, len(flag)):
		sFlag += chr(int(str(m[flag[i]])))

	flags.append(sFlag)

    # make sure we don't have the same flag the next time
	s.add(Or(
		flag[0] != m[flag[0]],
		flag[1] != m[flag[1]],
		flag[2] != m[flag[2]],
		flag[3] != m[flag[3]],
		flag[4] != m[flag[4]],
		flag[5] != m[flag[5]],
		flag[6] != m[flag[6]],
		flag[7] != m[flag[7]]
		))

# print all flags
[print(i) for i in flags]
print(len(flags))
```

When I ran this script the first time I saw that there were 864 flags which was way too many to test, however I noticed that some had the prefix FISH so I decided to grep for that:

```
FISHTuhK
FISHTuh1
FISHTu41
FISHTu4K
FISHTANe
FISHTAN1
FISHTuNe
FISHnA41
FISHnAN1
FISHTA41
FISHnANK
FISHnANe
FISHTANK
FISHnuNK
FISHnuN1
FISHnuNe
FISHTuNK
FISHTuN1
FISHTA4K
FISHTu4e
FISHTA4e
FISHTAh1
FISHTAhK
FISHTAhe
FISHTuhe
FISHnA4K
FISHnAh1
FISHnAhK
FISHnA4e
FISHnAhe
FISHnu4K
FISHnuhK
FISHnuh1
FISHnu41
FISHnu4e
FISHnuhe
```

I saw that FISHTANK was an option and I tried it and it ended up being the flag.