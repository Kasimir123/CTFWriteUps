# picoCTF Matryoshka Doll Write Up

## Details:
Points: 30

Jeopardy style CTF

Category: Forensics

Comments: Matryoshka dolls are a set of wooden dolls of decreasing size placed one inside another. What's the final one?

## Write up:

My first step here was to binwalk the image provided, I saw that there was data so I extracted this:

```
binwalk -D=".*" dolls.jpg 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             PNG image, 594 x 1104, 8-bit/color RGBA, non-interlaced
3226          0xC9A           TIFF image data, big-endian, offset of first image directory: 8
272492        0x4286C         Zip archive data, at least v2.0 to extract, compressed size: 378954, uncompressed size: 383940, name: base_images/2_c.jpg
651612        0x9F15C         End of Zip archive, footer length: 22
```

I then extracted the zip file and extracted the image inside of there:

```
binwalk -D=".*" 2_c.jpg    

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             PNG image, 526 x 1106, 8-bit/color RGBA, non-interlaced
3226          0xC9A           TIFF image data, big-endian, offset of first image directory: 8
187707        0x2DD3B         Zip archive data, at least v2.0 to extract, compressed size: 196045, uncompressed size: 201447, name: base_images/3_c.jpg
383807        0x5DB3F         End of Zip archive, footer length: 22
383918        0x5DBAE         End of Zip archive, footer length: 22
```

I then had to do this 2 more times before I found the flag.txt which when cat'ed gave me the flag:

```
cat flag.txt            
picoCTF{e3f378fe6c1ea7f6bc5ac2c3d6801c1f}
```