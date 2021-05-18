# picoCTF  Write Up

## Details:
Points: 110

Jeopardy style CTF

Category: Forensics

Comments: Use `srch_strings` from the sleuthkit and some terminal-fu to find a flag in this disk image: dds1-alpine.flag.img.gz

## Write up:

This was a pretty straitforward challenge, after extracting the zip file I ran the search string command and piped that into grep:

```
srch_strings -a dds1-alpine.flag.img| grep picoCTF

  SAY picoCTF{f0r3ns1c4t0r_n30phyt3_ad5c96c0}
```