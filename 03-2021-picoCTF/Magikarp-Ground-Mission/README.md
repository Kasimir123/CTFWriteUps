# picoCTF Magikarp Ground Mission Write Up

## Details:
Points: 30

Jeopardy style CTF

Category: General Skills

Comments:

Do you know how to move between directories and read files in the shell? Start the container, `ssh` to it, and then `ls` once connected to begin. Login via `ssh` as `ctf-player` with the password, `6d448c9c`

## Write up:

After launching the instance I connected using the ssh they gave me. Once in the instance I followed the instructions:

```
ctf-player@pico-chall$ ls

1of3.flag.txt  instructions-to-2of3.txt

ctf-player@pico-chall$ cat 1of3.flag.txt 

picoCTF{xxsh_

ctf-player@pico-chall$ cat instructions-to-2of3.txt 

Next, go to the root of all things, more succinctly `/`

ctf-player@pico-chall$ cd /

ctf-player@pico-chall$ ls

2of3.flag.txt  bin  boot  dev  etc  home  instructions-to-3of3.txt  lib  lib64  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var

ctf-player@pico-chall$ cat 2of3.flag.txt 

0ut_0f_\/\/4t3r_

ctf-player@pico-chall$ cat instructions-to-3of3.txt 

Lastly, ctf-player, go home... more succinctly `~`

ctf-player@pico-chall$ cd ~

ctf-player@pico-chall$ ls

3of3.flag.txt  drop-in

ctf-player@pico-chall$ cat 3of3.flag.txt 

5190b070}

ctf-player@pico-chall$ Connection to venus.picoctf.net closed by remote host.
```