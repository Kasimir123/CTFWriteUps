# Zer0pts Infected Write Up

## Details:

Jeopardy style CTF

Category: Reversing

## Write up:

This challenge had an nc connection with a proof of work check. Upon connecting to the system you saw that the backdoor file provided by the challenge was in /dev. The challenge has told us that the flag was in root but we did not have access to the directory.

After downloading the file I tried running it but it crashed immediately. I then started reversing the file. After reversing for a bit I found the following function:

```c
unsigned __int64 __fastcall backdoor_write(__int64 a1, const char *a2, size_t a3)
{
  int v3; // eax
  char *s; // [rsp+30h] [rbp-C0h]
  const char *s1; // [rsp+38h] [rbp-B8h]
  char *file; // [rsp+40h] [rbp-B0h]
  const char *nptr; // [rsp+48h] [rbp-A8h]
  struct stat64 v10; // [rsp+50h] [rbp-A0h] BYREF
  unsigned __int64 v11; // [rsp+E8h] [rbp-8h]

  v11 = __readfsqword(0x28u);
  s = strndup(a2, a3);
  if ( s )
  {
    s1 = strtok(s, ":");
    file = strtok(0LL, ":");
    nptr = strtok(0LL, ":");
    if ( s1 && file && nptr && !strncmp(s1, "b4ckd00r", 8uLL) )
    {
      stat64(file, &v10);
      if ( (v10.st_mode & 0xF000) != 0x8000 || (v3 = atoi(nptr), chmod(file, v3)) )
        fuse_reply_err(a1, 22LL);
      else
        fuse_reply_write(a1, a3);
    }
    else
    {
      fuse_reply_err(a1, 22LL);
    }
    free(s);
  }
  else
  {
    fuse_reply_err(a1, 22LL);
  }
  return __readfsqword(0x28u) ^ v11;
}
```

This seemed to compare some string to the format :b4ckd00r:file:value. File was then checked to see if it was a file with stat64, and was then passed the chmod, and the value was passed as the second parameter to chmod meaning it was the value for which permissions to set. After setting a value the program would call fuse error, if the write was not correct then the function would call the standard fuse write.

After connecting to the server again I tested this by trying to change the permissions on /init. I did this by trying echo ":b4ckd00r:/init:4095" > /dev/backdoor. with 4095 being full permissions. This ended up changing the permissions as expected. From here I needed to find a way to gain shell access. We then set up the following c program:

```c
#include <stdlib.h>
#include <unistd.h>

int main()
{
	setuid(0);
	system("/bin/sh");
}
```

Compiling this and setting it with full permissions worked on our system so we piped the input into base64 and copied it. Then we went onto the server and gave /init full permissions before piping the copied bytes into base64 decoder and into the /init file, overwriting it. After giving /init full permissions once again we were then able to run it and get a shell. From here we went into /root where there was a txt file with the flag.