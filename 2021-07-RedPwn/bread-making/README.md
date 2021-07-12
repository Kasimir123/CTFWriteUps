# RedPwn Bread Making Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

Looking at the main function I saw:

```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  __int64 v3;
  __int64 v4;
  __int64 v5; 
  __int64 v6; 
  int v7; 
  char v9[136];
  unsigned __int64 v10;

  v10 = __readfsqword(0x28u);
  setbuf(stdin, 0LL);
  setbuf(stdout, 0LL);
  setbuf(stderr, 0LL);
  signal(14, handler);
  v3 = 0LL;
  qword_6440 = 0LL;
  do
  {
    alarm(*(_DWORD *)*(&off_6020 + v3));
    puts(*((const char **)*(&off_6020 + qword_6440) + 1));
    do
    {
      if ( fgets(v9, 128, stdin) )
      {
        v9[strcspn(v9, "\n")] = 0;
        v4 = (__int64)*(&off_6020 + qword_6440);
        v5 = *(_QWORD *)(v4 + 24);
        if ( v5 )
        {
          v6 = 0LL;
          while ( strcmp(v9, *(const char **)(v4 + 16 * v6 + 32)) )
          {
            if ( v5 == ++v6 )
              goto LABEL_17;
          }
          v7 = (*(__int64 (**)(void))(v4 + 16 * v6 + 40))();
          if ( v7 != -1 )
            continue;
        }
      }
LABEL_17:
      sub_24A0();
    }
    while ( v7 );
    ++qword_6440;
    puts("");
    v3 = qword_6440;
  }
  while ( (unsigned __int64)qword_6440 <= 0xA );
  alarm(0);
  puts("it's the next morning");
  if ( dword_641C )
  {
    if ( dword_6418 )
    {
      if ( dword_6414 )
      {
        if ( dword_6410 )
        {
          if ( dword_640C )
            sub_25C0();
          else
            puts("mom finds the fire alarm in the laundry room and accuses you of making bread");
        }
        else
        {
          puts("mom finds the window opened and accuses you of making bread");
        }
      }
      else
      {
        puts("mom finds burnt bread on the counter and accuses you of making bread");
      }
    }
    else
    {
      puts("mom finds flour on the counter and accuses you of making bread");
    }
  }
  else
  {
    puts("mom finds flour in the sink and accuses you of making bread");
  }
  return 0LL;
}
```

From this I could see that we had to enter several strings in order to pass the "tests" and make bread.

The first thing I did here was look to see all the possible strings I could input and I found the following:

```
strings bread       

/lib64/ld-linux-x86-64.so.2
mfUa
libc.so.6
exit
fopen
signal
puts
__stack_chk_fail
stdin
fgets
strcspn
stdout
stderr
alarm
__cxa_finalize
setbuf
strcmp
__libc_start_main
GLIBC_2.4
GLIBC_2.2.5
_ITM_deregisterTMCloneTable
__gmon_start__
_ITM_registerTMCloneTable
ATUSH
[]A\A]A^A_
u+UH
ATUSH
tOE1
[]A\A]
[]A\A]A^A_
flag.txt
it's the next morning
mom doesn't suspect a thing, but asks about some white dots on the bathroom floor
couldn't open/read flag file, contact an admin if running on server
mom finds flour in the sink and accuses you of making bread
mom finds flour on the counter and accuses you of making bread
mom finds burnt bread on the counter and accuses you of making bread
mom finds the window opened and accuses you of making bread
mom finds the fire alarm in the laundry room and accuses you of making bread
the tray burns you and you drop the pan on the floor, waking up the entire house
the flaming loaf sizzles in the sink
the flaming loaf sets the kitchen on fire, setting off the fire alarm and waking up the entire house
pull the tray out with a towel
there's no time to waste
pull the tray out
the window is closed
the fire alarm is replaced
you sleep very well
time to go to sleep
close the window
replace the fire alarm
brush teeth and go to bed
you've taken too long and fall asleep
the dough has risen, but mom is still awake
the dough has been forgotten, making an awful smell the next morning
the dough has risen
the bread needs to rise
wait 2 hours
wait 3 hours
the oven makes too much noise, waking up the entire house
the oven glows a soft red-orange
the dough is done, and needs to be baked
the dough wants to be baked
preheat the oven
preheat the toaster oven
mom comes home and finds the bowl
mom comes home and brings you food, then sees the bowl
the ingredients are added and stirred into a lumpy dough
mom comes home before you find a place to put the bowl
the box is nice and warm
leave the bowl on the counter
put the bowl on the bookshelf
hide the bowl inside a box
the kitchen catches fire, setting off the fire alarm and waking up the entire house
the bread has risen, touching the top of the oven and catching fire
45 minutes is an awfully long time
you've moved around too much and mom wakes up, seeing you bake bread
return upstairs
watch the bread bake
the sink is cleaned
the counters are cleaned
everything appears to be okay
the kitchen is a mess
wash the sink
clean the counters
get ready to sleep
the half-baked bread is disposed of
flush the bread down the toilet
the oven shuts off
cold air rushes in
there's smoke in the air
unplug the oven
unplug the fire alarm
open the window
you put the fire alarm in another room
one of the fire alarms in the house triggers, waking up the entire house
brother is still awake, and sees you making bread
you bring a bottle of oil and a tray
it is time to finish the dough
you've shuffled around too long, mom wakes up and sees you making bread
work in the kitchen
work in the basement
flour has been added
yeast has been added
salt has been added
water has been added
add ingredients to the bowl
add flour
add yeast
add salt
add water
we don't have that ingredient at home!
the timer makes too much noise, waking up the entire house
the bread is in the oven, and bakes for 45 minutes
you've forgotten how long the bread bakes
the timer ticks down
use the oven timer
set a timer on your phone
:*3$"
GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
.shstrtab
.interp
.note.gnu.property
.note.gnu.build-id
.note.ABI-tag
.gnu.hash
.dynsym
.dynstr
.gnu.version
.gnu.version_r
.rela.dyn
.rela.plt
.init
.plt.got
.plt.sec
.text
.fini
.rodata
.eh_frame_hdr
.eh_frame
.init_array
.fini_array
.dynamic
.data
.bss
.comment
```

From here I then stated debugging and each time before inputting a string I was able to see which choices were possible for each item, I then got the following list:

```
add flour
add yeast
add salt
add water
hide the bowl inside a box
wait 3 hours
work in the basement
preheat the toaster oven
set a timer on your phone
watch the bread bake
pull the tray out with a towel
unplug the fire alarm
open the window
unplug the oven
clean the counters
flush the bread down the toilet
wash the sink
get ready to sleep
close the window
replace the fire alarm
brush teeth and go to bed
```

I then pasted this into the netcat and the program ran successfully:

```
nc mc.ax 31796   

add ingredients to the bowl
add flour
add yeast
add salt
add water
hide the bowl inside a box
wait 3 hours
work in the basement
preheat the toaster oven
set a timer on your phone
watch the bread bake
pull the tray out with a towel
unplug the fire alarm
open the window
unplug the oven
clean the counters
flush the bread down the toilet
wash the sink
get ready to sleep
close the window
replace the fire alarm
brush teeth and go to bedflour has been added
yeast has been added
salt has been added
water has been added

the ingredients are added and stirred into a lumpy dough
the box is nice and warm

the bread needs to rise
the dough has risen

it is time to finish the dough
you bring a bottle of oil and a tray

the dough is done, and needs to be baked
the oven glows a soft red-orange

the bread is in the oven, and bakes for 45 minutes
the timer ticks down

45 minutes is an awfully long time
the bread has risen, touching the top of the oven and catching fire

there's no time to waste
the flaming loaf sizzles in the sink

there's smoke in the air
you put the fire alarm in another room
cold air rushes in
the oven shuts off

the kitchen is a mess
the counters are cleaned
the half-baked bread is disposed of
the sink is cleaned
everything appears to be okay

time to go to sleep
the window is closed
the fire alarm is replaced

you sleep very well

it's the next morning
mom doesn't suspect a thing, but asks about some white dots on the bathroom floor
flag{m4yb3_try_f0ccac1a_n3xt_t1m3???0r_dont_b4k3_br3ad_at_m1dnight}
```

The flag was:

```
flag{m4yb3_try_f0ccac1a_n3xt_t1m3???0r_dont_b4k3_br3ad_at_m1dnight}
```