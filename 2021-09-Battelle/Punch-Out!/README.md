# Battelle Punch-Out Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 50

Comments:

```
We uncovered an early version of Mike Tyson's Punch-Out! This is quite possibly the greatest fighting game ever released featuring a 2-D Mike Tyson.

Rumor has it that this version of the game had a few bugs impacting the difficulty... See if you can beat it!

Flag format: the flag will be the sequence of actions you enter to win the game
```

## Write up:

Opening the program in a decompiler I saw that the main function was:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{

  dest[5] = __readfsqword(0x28u);
  *(_QWORD *)s = 0LL;
  v9 = 0LL;
  v10 = 0LL;
  v11 = 0LL;
  v6[4] = 0;
  dest[0] = 0LL;
  dest[1] = 0LL;
  dest[2] = 0LL;
  dest[3] = 0LL;
  srand(0x539u);
  puts(
    "=======================================================================\n"
    "                Mike Tyson's Puch-Out! (pre-alpha build)!              \n"
    "                (imagine this is flashing and looks cool)              \n"
    "                        >> graphics go here <<                         \n"
    "=======================================================================");
  printf(
    "Choose your opponent: \n0:  %s\n1:  %s\n2:  %s\n3:  %s\n4:  %s\n5:  %s\n6:  %s\n7:  %s\n8:  %s\n9:  %s\n>> ",
    boxers,
    off_4028,
    off_4030,
    off_4038,
    off_4040,
    off_4048,
    off_4050,
    off_4058,
    off_4060,
    off_4068);
  *(_DWORD *)v6 = (unsigned __int8)getchar();
  v3 = v6[0] - 48;
  if ( v3 <= 9 )
    __asm { jmp     rax }
  v4 = strlen(src);
  memcpy(dest, src, v4);
  puts("Sorry that boxer is not available. We're sending you in with Mike Tyson, smarty pants.");
  getchar();
  printf(
    "\n"
    "Action list: \n"
    "0: Dodge and counter right punch.\n"
    "1: Dodge and counter left punch.\n"
    "2: Dodge and counter right kick.\n"
    "3: Dodge and counter left kick.\n"
    "\n"
    "%s's so fast, you need to pick your action before he does!\n",
    (const char *)dest);
  for ( i = 0; i <= 23; ++i )
  {
    printf("Action: ");
    fgets(s, 32, _bss_start);
    __isoc99_sscanf(s, "%u", &v6[1]);
    if ( !(unsigned int)combatAction(dest, *(unsigned int *)&v6[1]) )
    {
      printf("[Attack: %d] %s connects snd knocks you down! KO!\n\n", (unsigned int)(i + 1), (const char *)dest);
      return 0;
    }
    printf(
      "[Attack: %d] You dodge %s's attack and connect with your own! Keep going!\n\n",
      (unsigned int)(i + 1),
      (const char *)dest);
  }
  puts("\nRoll credits. You win! Your flag is the winning comabt sequence. i.e. 020130112...\n");
  return 0;
}
```

I noticed two things here, the first was that to get the right combination we would need to loop through 24 times return 1 from the combatAction function each time:

```c
  for ( i = 0; i <= 23; ++i )
  {
    printf("Action: ");
    fgets(s, 32, _bss_start);
    __isoc99_sscanf(s, "%u", &v6[1]);
    if ( !(unsigned int)combatAction(dest, *(unsigned int *)&v6[1]) )
    {
      printf("[Attack: %d] %s connects snd knocks you down! KO!\n\n", (unsigned int)(i + 1), (const char *)dest);
      return 0;
    }
    printf(
      "[Attack: %d] You dodge %s's attack and connect with your own! Keep going!\n\n",
```

I also noticed that the program seeded rand:

```c
  srand(0x539u);
```

From here I decided to look more into the combatAction function:

```c
_BOOL8 __fastcall combatAction(const char *a1, int a2)
{

  v3 = artificialIntelligence();
  printf("%s %s\n", a1, (&combatMoves)[v3]);
  return a2 == v3;
}
```

```c
__int64 artificialIntelligence()
{
  return (unsigned int)(rand() % 4);
}
```

a2 was the number that we input each time, so in order to get the correct number (move) we would need to predict the next rand number.

The program looked like it was written in C so I decided to write a little program to populate srand and then predict all the numbers needed to win:

```c
int main()
{
	srand(1337);
	for (int i = 0; i <= 23; i++) {
		printf("%d", rand() % 4);
	}
}
```

This output:

```
123322100333030310222003
```

Which was the flag.