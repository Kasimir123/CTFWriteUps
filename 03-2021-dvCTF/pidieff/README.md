# dvCTF pidieff Write Up

## Details:
Points: 25

Jeopardy style CTF

Category: Stega

## Write up:

Since this was stegonography, my first step was to binwalk the pdf to see if there was any hidden data:

```
binwalk super_secret.pdf 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             PDF document, version: "1.3"
106           0x6A            Zlib compressed data, default compression

```

I then extracted the data with 

```
binwalk -D=".*" super_secret.pdf
```

In the extracted data there was a txt file that contained:

```
dvCTF{look_at_the_files}
```