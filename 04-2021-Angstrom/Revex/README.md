# Angstrom Revex Write Up

## Details:
Points: 75

Jeopardy style CTF

Category: Reverse Engineering

Comments:

As an active reddit user, clam frequently browses r/ProgrammerHumor. However, the reposts about how hard regex is makes him go >:((((. So, clam decided to show them who's boss.

```re
^(?=.*re)(?=.{21}[^_]{4}\}$)(?=.{14}b[^_]{2})(?=.{8}[C-L])(?=.{8}[B-F])(?=.{8}[^B-DF])(?=.{7}G(?<pepega>..).{7}t\k<pepega>)(?=.*u[^z].$)(?=.{11}(?<pepeega>[13])s.{2}(?!\k<pepeega>)[13]s)(?=.*_.{2}_)(?=actf\{)(?=.{21}[p-t])(?=.*1.*3)(?=.{20}(?=.*u)(?=.*y)(?=.*z)(?=.*q)(?=.*_))(?=.*Ex)
```

## Write up:

I opened up the website: https://regex101.com/ which is fantastic since it allows for regex since it allows you to debug.

I decided to split the regex into all of its portions so that I could figure out what each part did:

- `(?=.*re)`
    - at some point in the string 're' will be present
- `(?=.{21}[^_]{4}\}$)`
    - after 21 characters there will be 4 characters that are not '_'
- `(?=.{14}b[^_]{2})`
    - after 14 characters there will be the character 'b'
    - after b there will be two characters that are not '_'
- `(?=.{8}[C-L])`
    - after 8 characters the character will be between C-L
- `(?=.{8}[B-F])`
    - after 8 characters the character will be between B-F
- `(?=.{8}[^B-DF])`
    - after 8 characters the character will not be B-D or F
    - this means the character will be 'E'
- `(?=.{7}G(?<pepega>..).{7}t\k<pepega>)`
    - after 7 characters we will have 'G'
    - then reads 2 characters and makes a copy in the group
    - then after 7 characters we will have 't'
    - then we will have the two characters we copied earlier
- `(?=.*u[^z].$)`
    - at some point we will have the character 'u'
    - the character after u is not z
- `(?=.{11}(?<pepeega>[13])s.{2}(?!\k<pepeega>)[13]s)`
    - after 11 characters we will either have 1 or 3
    - this will be followed by 's'
    - then 2 characters
    - then either 1 or 3 
    - then 's'
- `(?=.*_.{2}_)`
    - at some point there will be '_' 
    - followed by two characters 
    - then another '_'
- `(?=actf\{)`
    - the start is 'actf{'
- `(?=.{21}[p-t])`
    - after 21 characters we have a character in range p-t
- `(?=.*1.*3)`
    - at some point we have '1'
    - then at another point after we have '3'
- `(?=.{20}(?=.*u)(?=.*y)(?=.*z)(?=.*q)(?=.*_))(?=.*Ex)`
    - after 20 characters we have, in no particular order:
        - 'u'
        - 'y'
        - 'z'
        - 'q'
        - '_'
        - 'Ex'

Using all of these I got:

```
actf{reGEx_1s_b3stEx_qzuy}
```