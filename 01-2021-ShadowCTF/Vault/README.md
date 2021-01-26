# ShadowCTF vault Write Up

## Details:
Points: 400

Jeopardy style CTF

Category: Reversing

## Write up:

Since this was a 400 point challenge I assumed that the password wouldn't be visible in strings so I went ahead and opened it in Ida.

``` c++
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char v4[23]; // [rsp+12h] [rbp-AEh] BYREF
  char v5[5]; // [rsp+29h] [rbp-97h] BYREF
  __int16 v6; // [rsp+2Eh] [rbp-92h] BYREF
  char v7[50]; // [rsp+30h] [rbp-90h] BYREF
  char format[7]; // [rsp+69h] [rbp-57h] BYREF
  char s2[32]; // [rsp+70h] [rbp-50h] BYREF
  char v10[24]; // [rsp+90h] [rbp-30h] BYREF
  char s1[15]; // [rsp+ADh] [rbp-13h] BYREF
  int i; // [rsp+BCh] [rbp-4h]

  strcpy(s1, "hackers_access");
  for ( i = 10; i <= 19; ++i )
    ;
  strcpy(v10, "no-cash_password");
  strcpy(format, "sh@d0w");
  puts("     .--------.");
  printf("    / .------. \\n");
  printf("   / /         \\n");
  puts("   | |        | |");
  puts("   | |        | |");
  puts("  _| |________| |_");
  puts(".' |_|        |_| '.");
  puts("'._____ ____ _____.'");
  puts("|     .'____'.     |");
  puts("'.__.'.'    '.'.__.'");
  puts("'.__  Shad0w |  __.'");
  puts("|   '.'.____.'.'   |");
  puts("'.____'.____.'____.'");
  printf("'.________________.'");
  strcpy(v7, "Congrats, your flag is: {R3ver5e_chal1enge_pwned}");
  v6 = 123;
  strcpy(v5, "%16s");
  strcpy(&v4[13], "reversing");
  strcpy(&v4[2], "You Failed");
  strcpy(v4, "}");
  puts("secure vault");
  puts("Enter our password:");
  __isoc99_scanf("%16s", s2);
  if ( !strcmp(s1, s2) )
  {
    puts("You Win");
    printf(format);
    printf((const char *)&v6);
    printf(format);
    putchar(95);
    printf(&v4[13]);
    printf("_rul3s");
    printf(v4);
  }
  else
  {
    puts("You Failed");
  }
  return 0;
}
```

For this challenge I set up the debugger and set a breakpoint on the line where it asks for the password. Since s2 was the input I wanted to look at what s1 one so that I could just type the password.

![key](./Photos/key.png)

I then typed in hackers_access and the program spit out the flag.

```
     .--------.
    / .------. \n   / /         \n   | |        | |
   | |        | |
  _| |________| |_
.' |_|        |_| '.
'._____ ____ _____.'
|     .'____'.     |
'.__.'.'    '.'.__.'
'.__  Shad0w |  __.'
|   '.'.____.'.'   |
'.____'.____.'____.'
'.________________.'secure vault
Enter our password:
hackers_access
You Win
sh@d0w{sh@d0w_reversing_rul3s}
```