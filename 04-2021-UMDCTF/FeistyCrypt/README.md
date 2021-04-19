# UMDCTF FeistyCrypt Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

We've created a Feisty new cipher called FeistyCrypt that we're quite proud of! Before we release it to the world we would like you to try out our encyption program. Can you decrypt the flag?

## Write up:

This was really the only really nice challenge in this CTF in my opinion. But anyways, I started by decompiling the main function:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  int v3; 
  int v4; 
  ssize_t v5; 
  int v6; 
  unsigned __int64 v7;
  int i; 
  __int64 v9; 
  unsigned int v10;
  unsigned __int32 ptr; 
  struct timeval timeout; 
  fd_set readfds;
  int v15[256]; 
  int buf[66]; 
  unsigned __int64 v17; 

  v17 = __readfsqword(0x28u);
  timeout.tv_sec = 0LL;
  timeout.tv_usec = 0LL;
  memset(&readfds, 0, sizeof(readfds));
  readfds.fds_bits[0] |= 1uLL;
  memset(buf, 0, 0x100uLL);
  sync();
  v3 = select(1024, &readfds, 0LL, 0LL, &timeout);
  if ( v3 > 0 )
  {
    v5 = read(0, buf, 0x100uLL);
    if ( (v5 & 7) != 0 )
      fwrite(
        "I like 8 byte blocks.\n"
        "The number of bytes you gave me wasn't a multiple of 8 so I padded your input with NULL bytes.\n"
        "It's cool, but I just wanted to let you know.  Mkay.\n"
        "\n",
        1uLL,
        0xABuLL,
        stderr);
    v6 = 0;
    v7 = 31337LL;
    while ( v6 <= 255 )
    {
      v7 = 0x5851F42D4C957F2DLL * v7 + 1;
      v15[v6++] = v7 >> 33;
    }
    for ( i = 0; i < v5; i += 8 )
    {
      v9 = encrypt(_byteswap_ulong(buf[i / 4u + 1]) | ((unsigned __int64)_byteswap_ulong(buf[i / 4u]) << 32), v15);
      v10 = v9;
      ptr = _byteswap_ulong(HIDWORD(v9));
      fwrite(&ptr, 4uLL, 1uLL, stdout);
      ptr = _byteswap_ulong(v10);
      fwrite(&ptr, 4uLL, 1uLL, stdout);
    }
    v4 = 0;
  }
  else
  {
    v4 = v3;
    if ( v3 )
    {
      puts("shrug emoji");
      v4 = 1;
    }
    else
    {
      puts(
        "If you give me plaintext via stdin I will give you ciphertext via stdout.\n"
        "\n"
        "Maybe try running me like this...\n"
        "\n"
        "echo -n 'Hello, plaintext' | ./feisty_crypt_encrypt | hexdump -C\n"
        "\n"
        "echo -n 'Hello, plaintext' | ./feisty_crypt_encrypt > ciphertext.bin\n"
        "\n"
        "cat plaintext.bin | ./feisty_crypt_encrypt | hexdump -C\n"
        "\n"
        "The ciphertext bytes for 'baseball' are: 70 09 36 2d 3a 75 45 7c\n"
        "\n"
        "Oh, you wanna decypt stuff?  Good luck with that! ;)\n"
        "\n");
    }
  }
  return v4;
}
```

The main function seemed to create some sort of array with length 256 and then seemed to encrypt 8 bytes at a time using the encryption function. Decompiling the encryption function I got:

```c
__int64 __fastcall encrypt(unsigned __int64 a1, __int64 a2)
{
  unsigned __int64 v2;
  unsigned int v3; 
  unsigned int i;
  unsigned int v5; 
  unsigned int v6; 

  v2 = HIDWORD(a1);
  v3 = a1;
  for ( i = 0; i <= 0xFF; ++i )
  {
    v5 = v2 ^ f(v3, *(unsigned int *)(a2 + 4LL * i));
    if ( i == 255 )
    {
      v6 = v3;
      v3 = v5;
      v5 = v6;
    }
    v2 = v3;
    v3 = v5;
  }
  return (v2 << 32) | v3;
}
```

Our cryptography person recognized this as a feistel cipher which made the whole process a lot easier for me since for a feistel cipher you just need to run the decryption with the array run in reverse.

The first thing I did was extract the key:

```python
key = [0x23FD5E89, 0x5B2261AF, 0x2FE1BEFF, 0x5639C8B3, 0x055DB46F, 0x06C72EC1, 0x5C3075CA, 0x292DB8AB, 0x41198FF2, 0x64317860, 0x56B49DA4, 0x4184F32C, 0x62007C3B, 0x43DADC7C, 0x7522E0C0, 0x6D7F9573, 0x2A22ED32, 0x382E0D39, 0x71FC8B4F, 0x0261CE41, 0x136EDD56, 0x137A86CE, 0x407D887A, 0x2B90757D, 0x77B52F05, 0x676937CB, 0x26A3DC5E, 0x64B29390, 0x78C00DDB, 0x28DFACC6, 0x691C3744, 0x1B86B44F, 0x68C94ADA, 0x3FF7330D, 0x2086AD97, 0x4883547D, 0x71F066E7, 0x7394A4D9, 0x1BFA3CCF, 0x67E98CB6, 0x140971F5, 0x779C6FEE, 0x187D9E70, 0x6BFC76E9, 0x0DB407B6, 0x59B1A815, 0x21543363, 0x1AC7824E, 0x6EAD0FB1, 0x1653B8A9, 0x03DD45F6, 0x113E5EFF, 0x6D89001B, 0x68ABEA02, 0x78B81FEF, 0x666F2F50, 0x57AF0FC2, 0x6BFBEC39, 0x0DE762D8, 0x04F360C2, 0x2AD69155, 0x74326AF5, 0x43723F1A, 0x5DE93D42, 0x6D534483, 0x04CFACFA, 0x5BA32576, 0x0DBF4AC9, 0x505BD98D, 0x25E58465, 0x450E55DD, 0x0EDA33F4, 0x11EE20D8, 0x61340D9E, 0x1E72FD16, 0x29484E9B, 0x42A58FAB, 0x20941CB3, 0x680A4CA1, 0x2950AB13, 0x52D1A096, 0x3774533B, 0x3B9987FB, 0x74F739DA, 0x4625D2A5, 0x3CD379D0, 0x3D80B811, 0x7013C78B, 0x0A68AD9B, 0x74DFCB56, 0x56762842, 0x12405E6A, 0x0477F8A6, 0x336C4FE0, 0x7A5A3B66, 0x3A0246BD, 0x765D0F54, 0x7B9AF87D, 0x7405CDE8, 0x61BEAE87, 0x32223FF4, 0x43D86AFD, 0x1304E42C, 0x4CA72E7E, 0x23C87D96, 0x15E67802, 0x4D4740B2, 0x6B0AF4B1, 0x21C5C4BA, 0x7EF7B118, 0x0C023FE5, 0x4DF209AE, 0x79BD474F, 0x0DAB7F41, 0x70551B15, 0x68552E29, 0x4F4FAC4E, 0x20B8B2BF, 0x60B97860, 0x1C88E618, 0x1BEBA0B5, 0x393F26A7, 0x3FDC3997, 0x3BA169A1, 0x00C28FE7, 0x08A01788, 0x027BBBEB, 0x1FED911C, 0x3A039C20, 0x366A6CBA, 0x1C3D2083, 0x1557A297, 0x79225613, 0x724264BB, 0x6DD81AEB, 0x251955A8, 0x09168C1F, 0x7B346B10, 0x0103442D, 0x39BA64C8, 0x64AC5BA7, 0x02B46AAA, 0x7F3254C0, 0x7D9E3EC0, 0x3B3A9491, 0x3D76232C, 0x53FFB65F, 0x27E611E8, 0x3ABF8A2F, 0x58EF8F13, 0x2EB74A4A, 0x0C161EC1, 0x21D6041F, 0x7E1B1105, 0x567EFDEB, 0x7C35EECF, 0x65C94361, 0x38D7B0C4, 0x6D9B74B3, 0x0CE9D485, 0x7B332EA6, 0x6AC6D4DD, 0x2DB54BC0, 0x0A48240D, 0x33825E57, 0x16180294, 0x60155387, 0x4F71FC30, 0x55766CA3, 0x4E7E45E8, 0x083A5B37, 0x453A20C9, 0x604054CC, 0x0DFA79A8, 0x6278C591, 0x4D38EAA5, 0x1D1C1AF9, 0x52709B38, 0x0D9891B5, 0x5DEECDB1, 0x76C7ABF0, 0x61B1C4E3, 0x58A22C33, 0x40C7FF21, 0x134979D7, 0x580B9029, 0x428AE425, 0x06033889, 0x7FC31DC1, 0x3AB7B92C, 0x129A917D, 0x0EE37FB2, 0x002E98EB, 0x528AFD7B, 0x7C3C2714, 0x6F3857EB, 0x40D48D39, 0x2DC13029, 0x2BD24D7D, 0x7553D9E9, 0x2D5E9A50, 0x2408D01D, 0x3C1238BC, 0x332CB98B, 0x745EE111, 0x28AC04E4, 0x43F757B3, 0x2D34C20D, 0x2573FD2F, 0x60FD1290, 0x21596A8F, 0x20DADC3E, 0x2117FE1E, 0x1273A3B4, 0x5C710DFF, 0x7905C80E, 0x5DD5D44E, 0x4FD665B0, 0x1942323B, 0x46F421ED, 0x60C5C3C4, 0x10D40898, 0x5FC642AB, 0x7C201248, 0x61764C25, 0x6401497D, 0x1E85F88E, 0x64502C64, 0x090DA24D, 0x0C718924, 0x0B2D579F, 0x6A6D82EC, 0x72902F96, 0x05EF7679, 0x10D156E8, 0x36A89AEE, 0x69F1A459, 0x662CE4F8, 0x5D7A8D28, 0x3FE21E01, 0x4C95B18E, 0x327FC3F5, 0x5093BA58, 0x320CFD69, 0x7CA511FA, 0x4E3E518B, 0x2B77F722, 0x63D49291, 0x57B42564, 0x64022FA5, 0x238FB5E9, 0x7725EC84, 0x312BAD71, 0x10B70C12, 0x430643A8, 0x5861D29E]
```

Then I started creating the script, I noticed the f function in the encryption and decompiled that too:

```c
__int64 __fastcall f(int a1, int a2)
{
  return a2 & a1 ^ 0xBA5EBA11;
}
```

I then wrote the following script:

```python
# key used
key = [0x23FD5E89, 0x5B2261AF, 0x2FE1BEFF, 0x5639C8B3, 0x055DB46F, 0x06C72EC1, 0x5C3075CA, 0x292DB8AB, 0x41198FF2, 0x64317860, 0x56B49DA4, 0x4184F32C, 0x62007C3B, 0x43DADC7C, 0x7522E0C0, 0x6D7F9573, 0x2A22ED32, 0x382E0D39, 0x71FC8B4F, 0x0261CE41, 0x136EDD56, 0x137A86CE, 0x407D887A, 0x2B90757D, 0x77B52F05, 0x676937CB, 0x26A3DC5E, 0x64B29390, 0x78C00DDB, 0x28DFACC6, 0x691C3744, 0x1B86B44F, 0x68C94ADA, 0x3FF7330D, 0x2086AD97, 0x4883547D, 0x71F066E7, 0x7394A4D9, 0x1BFA3CCF, 0x67E98CB6, 0x140971F5, 0x779C6FEE, 0x187D9E70, 0x6BFC76E9, 0x0DB407B6, 0x59B1A815, 0x21543363, 0x1AC7824E, 0x6EAD0FB1, 0x1653B8A9, 0x03DD45F6, 0x113E5EFF, 0x6D89001B, 0x68ABEA02, 0x78B81FEF, 0x666F2F50, 0x57AF0FC2, 0x6BFBEC39, 0x0DE762D8, 0x04F360C2, 0x2AD69155, 0x74326AF5, 0x43723F1A, 0x5DE93D42, 0x6D534483, 0x04CFACFA, 0x5BA32576, 0x0DBF4AC9, 0x505BD98D, 0x25E58465, 0x450E55DD, 0x0EDA33F4, 0x11EE20D8, 0x61340D9E, 0x1E72FD16, 0x29484E9B, 0x42A58FAB, 0x20941CB3, 0x680A4CA1, 0x2950AB13, 0x52D1A096, 0x3774533B, 0x3B9987FB, 0x74F739DA, 0x4625D2A5, 0x3CD379D0, 0x3D80B811, 0x7013C78B, 0x0A68AD9B, 0x74DFCB56, 0x56762842, 0x12405E6A, 0x0477F8A6, 0x336C4FE0, 0x7A5A3B66, 0x3A0246BD, 0x765D0F54, 0x7B9AF87D, 0x7405CDE8, 0x61BEAE87, 0x32223FF4, 0x43D86AFD, 0x1304E42C, 0x4CA72E7E, 0x23C87D96, 0x15E67802, 0x4D4740B2, 0x6B0AF4B1, 0x21C5C4BA, 0x7EF7B118, 0x0C023FE5, 0x4DF209AE, 0x79BD474F, 0x0DAB7F41, 0x70551B15, 0x68552E29, 0x4F4FAC4E, 0x20B8B2BF, 0x60B97860, 0x1C88E618, 0x1BEBA0B5, 0x393F26A7, 0x3FDC3997, 0x3BA169A1, 0x00C28FE7, 0x08A01788, 0x027BBBEB, 0x1FED911C, 0x3A039C20, 0x366A6CBA, 0x1C3D2083, 0x1557A297, 0x79225613, 0x724264BB, 0x6DD81AEB, 0x251955A8, 0x09168C1F, 0x7B346B10, 0x0103442D, 0x39BA64C8, 0x64AC5BA7, 0x02B46AAA, 0x7F3254C0, 0x7D9E3EC0, 0x3B3A9491, 0x3D76232C, 0x53FFB65F, 0x27E611E8, 0x3ABF8A2F, 0x58EF8F13, 0x2EB74A4A, 0x0C161EC1, 0x21D6041F, 0x7E1B1105, 0x567EFDEB, 0x7C35EECF, 0x65C94361, 0x38D7B0C4, 0x6D9B74B3, 0x0CE9D485, 0x7B332EA6, 0x6AC6D4DD, 0x2DB54BC0, 0x0A48240D, 0x33825E57, 0x16180294, 0x60155387, 0x4F71FC30, 0x55766CA3, 0x4E7E45E8, 0x083A5B37, 0x453A20C9, 0x604054CC, 0x0DFA79A8, 0x6278C591, 0x4D38EAA5, 0x1D1C1AF9, 0x52709B38, 0x0D9891B5, 0x5DEECDB1, 0x76C7ABF0, 0x61B1C4E3, 0x58A22C33, 0x40C7FF21, 0x134979D7, 0x580B9029, 0x428AE425, 0x06033889, 0x7FC31DC1, 0x3AB7B92C, 0x129A917D, 0x0EE37FB2, 0x002E98EB, 0x528AFD7B, 0x7C3C2714, 0x6F3857EB, 0x40D48D39, 0x2DC13029, 0x2BD24D7D, 0x7553D9E9, 0x2D5E9A50, 0x2408D01D, 0x3C1238BC, 0x332CB98B, 0x745EE111, 0x28AC04E4, 0x43F757B3, 0x2D34C20D, 0x2573FD2F, 0x60FD1290, 0x21596A8F, 0x20DADC3E, 0x2117FE1E, 0x1273A3B4, 0x5C710DFF, 0x7905C80E, 0x5DD5D44E, 0x4FD665B0, 0x1942323B, 0x46F421ED, 0x60C5C3C4, 0x10D40898, 0x5FC642AB, 0x7C201248, 0x61764C25, 0x6401497D, 0x1E85F88E, 0x64502C64, 0x090DA24D, 0x0C718924, 0x0B2D579F, 0x6A6D82EC, 0x72902F96, 0x05EF7679, 0x10D156E8, 0x36A89AEE, 0x69F1A459, 0x662CE4F8, 0x5D7A8D28, 0x3FE21E01, 0x4C95B18E, 0x327FC3F5, 0x5093BA58, 0x320CFD69, 0x7CA511FA, 0x4E3E518B, 0x2B77F722, 0x63D49291, 0x57B42564, 0x64022FA5, 0x238FB5E9, 0x7725EC84, 0x312BAD71, 0x10B70C12, 0x430643A8, 0x5861D29E]

# f function
def f(a1, a2):
	return a2 & a1 ^ 0xBA5EBA11

# encryption/decryption function
def encrypt(a1, a2):
	v2 = (a1 & 0xFFFFffFF00000000)>>32
	v3 = a1
	i = 0
	while i <= 0xff:
		v5 = v2 ^ f(v3, a2[i])
		if i == 255:
			v6 = v3
			v3 = v5
			v5 = v6
		v2 = v3
		v3 = v5
		i += 1
	return (v2 << 32) | v3

# string to save to
s = ""

# read in the encrypted flag
poc = bytearray(open('flag_ciphertext.bin', 'rb').read())

# loop through 8 bytes at a time
for i in range(0, len(poc), 8):
    # decrypt
	x = (encrypt(int(poc[i:i+8].hex(), 16), key[::-1]))& 0xFFFFFFFFFFFFFFFF
	
    # turn to characters
    for j in range(0, len(hex(x)[2:]), 2):
		s += chr(int(hex(x)[2+j]+hex(x)[2+j+1], 16))

# print
print(s)
```

When run the following gets printed:

```
UMDCTF-{h0r57_f31573l_wuz_h3r3.}
```