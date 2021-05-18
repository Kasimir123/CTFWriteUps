# Angstrom FREE FLAGS Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: Reverse Engineering

Comments:

Clam was browsing armstrongctf.com when suddenly a popup appeared saying "GET YOUR FREE FLAGS HERE!!!" along with a download. Can you fill out the survey for free flags?

Find it on the shell server at /problems/2021/free_flags or over netcat at nc shell.actf.co 21703.


## Write up:

Decompiling the main function we get:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v4; 
  int v5;
  int v6; 
  int v7;
  char s[264]; 
  unsigned __int64 v9; 

  v9 = __readfsqword(0x28u);
  puts("Congratulations! You are the 1000th CTFer!!! Fill out this short survey to get FREE FLAGS!!!");
  puts("What number am I thinking of???");
  __isoc99_scanf("%d", &v7);
  if ( v7 == 31337 )
  {
    puts("What two numbers am I thinking of???");
    __isoc99_scanf("%d %d", &v6, &v5);
    if ( v5 + v6 == 1142
      && v5 * v6 == 302937
      && (puts("What animal am I thinking of???"),
          __isoc99_scanf(" %256s", s),
          s[strcspn(s, "\n")] = 0,
          !strcmp(s, "banana")) )
    {
      puts("Wow!!! Now I can sell your information to the Russian government!!!");
      puts("Oh yeah, here's the FREE FLAG:");
      print_flag();
      v4 = 0;
    }
    else
    {
      puts("Wrong >:((((");
      v4 = 1;
    }
  }
  else
  {
    puts("Wrong >:((((");
    v4 = 1;
  }
  return v4;
}
```

We can see that we need to answer a few questions. The first one we need to answer 31337, the second one we need to find two numbers that match the equation, I chose 723 and 419, and for the last one we need to answer banana. When I ran the server and entered the answers I got:

```
Congratulations! You are the 1000th CTFer!!! Fill out this short survey to get FREE FLAGS!!!
What number am I thinking of???
31337
What two numbers am I thinking of???
723 419
What animal am I thinking of???
banana
Wow!!! Now I can sell your information to the Russian government!!!
Oh yeah, here's the FREE FLAG:
actf{what_do_you_mean_bananas_arent_animals}
```