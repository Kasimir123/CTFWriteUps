# ImaginaryCTF Stings Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Points: 100

Comments: 

Enter the beehive. Don't get stung.

(Note: the password/flag is in the format ictf{.*})

## Write up:

Reversing the main function we get:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{

  v35 = __readfsqword(0x28u);
  strcpy(
    s,
    "     %           %\n"
    "         %           %\n"
    "            %           %\n"
    "               %          %\n"
    "                 %          %\n"
    "                   %          %                   :::\n"
    "                    %          %                ::::::\n"
    "                 %%%%%%  %%%%%%%%%            ::::::::\n"
    "              %%%%%ZZZZ%%%%%%   %%%ZZZZ     ::::::::::         ::::::\n"
    "             %%%ZZZZZ%%%%%%%%%%%%%%ZZZZZZ  :::::::::::    :::::::::::::::::\n"
    "             ZZZ%ZZZ%%%%%%%%%%%%%%%ZZZZZZZ::::::::::***:::::::::::::::::::::\n"
    "          ZZZ%ZZZZZZ%%%%%%%%%%%%%%ZZZZZZZZZ::::::***:::::::::::::::::::::::\n"
    "        ZZZ%ZZZZZZZZZZ%%%%%%%%%%ZZZZZZ%ZZZZ:::***:::::::::::::::::::::::\n"
    "       ZZ%ZZZZZZZZZZZZZZZZZZZZZZZ%%%%% %ZZZ:**::::::::::::::::::::::\n"
    "      ZZ%ZZZZZZZZZZZZZZZZZZZ%%%%% | | %ZZZ *:::::::::::::::::::\n"
    "      Z%ZZZZZZZZZZZZZZZ%%%%%%%%%%%%%%%ZZZ::::::::::::::::::::::::::\n"
    "       ZZZZZZZZZZZ%%%%%ZZZZZZZZZZZZZZZZZ%%%%:::ZZZZ:::::::::::::::::\n"
    "         ZZZZ%%%%%ZZZZZZZZZZZZZZZZZZ%%%%%ZZZ%%ZZZ%ZZ%%*:::::::::::\n"
    "            ZZZZZZZZZZZZZZZZZZ%%%%%%%%%ZZZZZZZZZZ%ZZ%:::*:::::::\n"
    "            *:::%%%%%%%%%%%%%%%%%%%%%%%ZZZZZZZZZZ%%%*::::*::::\n"
    "          *:::::::%%%%%%%%%%%%%%%%%%%%%%%ZZZZZ%%      *:::Z\n"
    "         **:ZZZZ:::%%%%%%%%%%%%%%%%%%%%%%%%%%%ZZ      ZZZZZ\n"
    "        *:ZZZZZZZ       %%%%%%%%%%%%%%%%%%%%%ZZZZ    ZZZZZZZ\n"
    "       *::::ZZZZZZ         %%%%%%%%%%%%%%%ZZZZZZZ      ZZZ\n"
    "        *::ZZZZZZ           Z%%%%%%%%%%%ZZZZZZZ%%\n"
    "          ZZZZ              ZZZZZZZZZZZZZZZZ%%%%%\n"
    "                           %%%ZZZZZZZZZZZ%%%%%%%%\n"
    "                          Z%%%%%%%%%%%%%%%%%%%%%\n"
    "                          ZZ%%%%%%%%%%%%%%%%%%%\n"
    "                          %ZZZZZZZZZZZZZZZZZZZ\n"
    "                          %%ZZZZZZZZZZZZZZZZZ\n"
    "                           %%%%%%%%%%%%%%%%\n"
    "                            %%%%%%%%%%%%%\n"
    "                             %%%%%%%%%\n"
    "                              ZZZZ\n"
    "                              ZZZ\n"
    "                             ZZ\n"
    "                            Z\n");
  *(_DWORD *)&s[1964] = 0;
  memset(&s[1968], 0, 0x850uLL);
  strcpy(v6, "jdug|tus2oht`5s4ou`i2ee4o`28c32b7:~");
  v7 = 0LL;
  v8 = 0LL;
  v9 = 0LL;
  v10 = 0LL;
  v11 = 0LL;
  v12 = 0LL;
  v13 = 0LL;
  v14 = 0LL;
  v15 = 0LL;
  v16 = 0LL;
  v17 = 0LL;
  v18 = 0LL;
  v19 = 0LL;
  v20 = 0LL;
  v21 = 0LL;
  v22 = 0LL;
  v23 = 0LL;
  v24 = 0LL;
  v25 = 0LL;
  v26 = 0LL;
  v27 = 0LL;
  v28 = 0LL;
  v29 = 0LL;
  v30 = 0LL;
  v31 = 0LL;
  v32 = 0LL;
  v33 = 0LL;
  puts(s);
  puts("Welcome to the beehive.");
  puts("Enter the password, or you'll get stung!");
  __isoc99_scanf("%50s", v5);
  while ( v4 <= 34 )
  {
    if ( v5[v4] != v6[v4] - 1 )
    {
      puts("I'm disappointed. *stings you*");
      return -1;
    }
    ++v4;
  }
  puts("Congrats! The password is the flag.");
  return 0;
}
```

We see that we compare the input, v5, with some sort of flag string where we subtract 1 from it, v6. When we look into the code we see that v6 is:

```
"jdug|tus2oht`5s4ou`i2ee4o`28c32b7:~"
```

So I wrote a script that goes through all of these values, subtracts 1, and prints as a char:

```py
# goes through all characters, get the ord for it, subtracts, turns it into a char, then joins the array as a string
print(''.join([chr(ord(i) - 1) for i in "jdug|tus2oht`5s4ou`i2ee4o`28c32b7:~"]))
```

When run I got:

```
ictf{str1ngs_4r3nt_h1dd3n_17b21a69}
```