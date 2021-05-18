# picoCTF Wireshark doo dooo do doo... Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: Forensics

Comments: Can you find the flag?

## Write up:

I opened the pcap file in wireshark, the first thing I did was go to Statistics-> Protocol Hierarchy. Here I saw that there were packets of lined based text data. I right clicked and selected to apply filter to that type of packets. I noticed that the first packet had:

```
0000   02 3b c6 1a ae f5 02 fb 68 4c e9 41 08 00 45 00   .;......hL.A..E.
0010   01 72 b7 8b 40 00 24 06 7e 86 12 de 25 86 c0 a8   .r..@.$.~...%...
0020   26 68 00 50 fa 5d 67 a1 ef 44 dd 29 e1 5b 50 18   &h.P.]g..D.).[P.
0030   01 e7 d7 f5 00 00 48 54 54 50 2f 31 2e 31 20 32   ......HTTP/1.1 2
0040   30 30 20 4f 4b 0d 0a 44 61 74 65 3a 20 4d 6f 6e   00 OK..Date: Mon
0050   2c 20 31 30 20 41 75 67 20 32 30 32 30 20 30 31   , 10 Aug 2020 01
0060   3a 35 31 3a 34 35 20 47 4d 54 0d 0a 53 65 72 76   :51:45 GMT..Serv
0070   65 72 3a 20 41 70 61 63 68 65 2f 32 2e 34 2e 32   er: Apache/2.4.2
0080   39 20 28 55 62 75 6e 74 75 29 0d 0a 4c 61 73 74   9 (Ubuntu)..Last
0090   2d 4d 6f 64 69 66 69 65 64 3a 20 46 72 69 2c 20   -Modified: Fri, 
00a0   30 37 20 41 75 67 20 32 30 32 30 20 30 30 3a 34   07 Aug 2020 00:4
00b0   35 3a 30 32 20 47 4d 54 0d 0a 45 54 61 67 3a 20   5:02 GMT..ETag: 
00c0   22 32 66 2d 35 61 63 33 65 65 61 34 66 63 66 30   "2f-5ac3eea4fcf0
00d0   31 22 0d 0a 41 63 63 65 70 74 2d 52 61 6e 67 65   1"..Accept-Range
00e0   73 3a 20 62 79 74 65 73 0d 0a 43 6f 6e 74 65 6e   s: bytes..Conten
00f0   74 2d 4c 65 6e 67 74 68 3a 20 34 37 0d 0a 4b 65   t-Length: 47..Ke
0100   65 70 2d 41 6c 69 76 65 3a 20 74 69 6d 65 6f 75   ep-Alive: timeou
0110   74 3d 35 2c 20 6d 61 78 3d 31 30 30 0d 0a 43 6f   t=5, max=100..Co
0120   6e 6e 65 63 74 69 6f 6e 3a 20 4b 65 65 70 2d 41   nnection: Keep-A
0130   6c 69 76 65 0d 0a 43 6f 6e 74 65 6e 74 2d 54 79   live..Content-Ty
0140   70 65 3a 20 74 65 78 74 2f 68 74 6d 6c 0d 0a 0d   pe: text/html...
0150   0a 47 75 72 20 73 79 6e 74 20 76 66 20 63 76 70   .Gur synt vf cvp
0160   62 50 47 53 7b 63 33 33 78 6e 6f 30 30 5f 31 5f   bPGS{c33xno00_1_
0170   66 33 33 5f 68 5f 71 72 6e 71 6f 72 72 73 7d 0a   f33_h_qrnqorrs}.
```

So I then right clicked and chose to follow the packet as http stream:

```
GET / HTTP/1.1
Host: 18.222.37.134
Connection: keep-alive
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.105 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9

HTTP/1.1 200 OK
Date: Mon, 10 Aug 2020 01:51:45 GMT
Server: Apache/2.4.29 (Ubuntu)
Last-Modified: Fri, 07 Aug 2020 00:45:02 GMT
ETag: "2f-5ac3eea4fcf01"
Accept-Ranges: bytes
Content-Length: 47
Keep-Alive: timeout=5, max=100
Connection: Keep-Alive
Content-Type: text/html

Gur synt vf cvpbPGS{c33xno00_1_f33_h_qrnqorrs}
```

There was not enough there for a substitution cipher so I decided to try caesar cipher and got:

```
The flag is picoCTF{p33kab00_1_s33_u_deadbeef}
```