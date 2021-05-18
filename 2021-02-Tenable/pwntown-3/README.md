# Tenable Pwntown 3 Write Up

## Details:
Points: 200

Jeopardy style CTF

Category: Reverse Engineering

Comment:


I've been trying to make a video game. It's pretty rough around the edges right now. Character sprites don't always cooperate, one of the character classes can't even attack yet, but I'm pretty proud of it so far. I made it using Unity, a starter package called uMMORPG, and a bunch of assets from the public domain.

It's really difficult, but I'm sure a hacker such as yourself won't have any problem figuring it out. ;)

The flag for this challenge can be obtained by finding the hidden message in the ocean.


## Write up:

While walking along the right side of the map with the minimap scroll out the entire way you could see something in the ocean. Using cheat engine you could find the camera values and go to the spot in the ocean. I then was able to simply able to draw the QR code out in a pixel editor and got:

![](./Photos/pwntown3.png)

Scanning this I was able to get the flag.