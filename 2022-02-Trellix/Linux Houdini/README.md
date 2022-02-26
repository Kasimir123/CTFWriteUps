# Trellix Linux Houdini Write Up

## Details:

Jeopardy style CTF

Category: Linux

Comments:

```
Catmen has discovered that the dogs are keeping information regarding the pieces of Anubis on a tightly-controlled Linux server. One of your fellow agents has automatically generated an account for you on this server, which you can access at 0.cloud.chals.io:34661. The rest is up to you, agent!
```

## Write up:

Connecting to the nc I got the following:

```
Welcome the flag can be printed using /usr/bin/echo_flag
Good luck!
```

My first step here was to base64 the file and then I loaded the file into binja:

```
lJCACWY4dps7D@2d14c2a26738:~$ base64 /usr/bin/echo_flag
```

This gave me the following main function:

```c
000007fa  int32_t main(int32_t argc, char** argv, char** envp)
00000802      int32_t var_1c = argc
00000821      int32_t rax_10
00000821      if (strcmp(*argv, "/private/echo_flag") != 0)
000008a7          printf(format: "This program is being ran from t…", *argv)
000008ac          rax_10 = 1
0000082a      else
0000082a          puts(str: "Congrats!")
0000083d          FILE* rax_3 = fopen(filename: "/private/flag", mode: &data_965)
0000084b          if (rax_3 != 0)
0000087c              for (char var_11_1 = fgetc(fp: rax_3); var_11_1 != 0xff; var_11_1 = fgetc(fp: rax_3))
00000864                  putchar(c: sx.d(var_11_1))
00000885              fclose(fp: rax_3)
0000088a          rax_10 = 0
000008b2      return rax_10
```

I saw that I would need to set the first argument to `/private/echo_flag`. So I reconnected to the server and used exec to set the argument:

```
Welcome the flag can be printed using /usr/bin/echo_flag
Good luck!
k4Y2LznJ9o720@2d14c2a26738:~$ exec -a /private/echo_flag /usr/bin/echo_flag
exec -a /private/echo_flag /usr/bin/echo_flag
Congrats!
ATR[7H3-11NUX-15-57r0N6-W17H-7H15-0N3]
```

