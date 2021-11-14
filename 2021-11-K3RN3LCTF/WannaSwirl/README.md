# K3RN3LCTF WannaSwirl Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering


## Write up:

Opening the main function in a decompiler we see:

```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{

  if ( a1 == 2 )
  {
    sub_2540(path, a2[1], a3);
    sub_24A6(&unk_6180, path);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
  }
  sub_2540(directory, "", a3);
  while ( 1 )
  {
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(path, &unk_6180);
    v3 = sub_2663(path);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
    if ( v3 <= 0 )
      break;
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(path, directory);
    sub_29E3((unsigned int)path, (unsigned int)directory, v4, v5, v6, v7, directory[0]);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
  }
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(path, directory);
  sub_29E3((unsigned int)path, (unsigned int)directory, v8, v9, v10, v11, directory[0]);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(directory);
  return 0LL;
}
```

```c
 if ( a1 == 2 )
  {
    sub_2540(path, a2[1], a3);
    sub_24A6(&unk_6180, path);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
  }
```

We see that the function checks if a1 is equal to 2, since this is the main function we know that a1 is argc, so this check passes when we pass an argument to the binary. In the if statement we set the path to the command line argument. 

```c
  sub_2540(directory, "", a3);
  while ( 1 )
  {
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(path, &unk_6180);
    v3 = sub_2663(path);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
    if ( v3 <= 0 )
      break;
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(path, directory);
    sub_29E3((unsigned int)path, (unsigned int)directory, v4, v5, v6, v7, directory[0]);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
  }
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(path, directory);
  sub_29E3((unsigned int)path, (unsigned int)directory, v8, v9, v10, v11, directory[0]);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(path);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(directory);
```

We then set the directory to "" and then enter a while loop. We pass the path to sub_2663 and check for the response, if the value is less than or equal to 0 we break out of the loop. Otherwise we call the other function.

First we want to analyze sub_2663:

```c
__int64 __fastcall sub_2663(const char **a1)
{
  unsigned int v1; // er12
  DIR *v2; // rbx
  struct dirent *i; // rax

  v1 = 0;
  v2 = opendir(*a1);
  for ( i = readdir(v2); i; i = readdir(v2) )
  {
    if ( i->d_type == 4 && i->d_name[0] != 46 )
      ++v1;
  }
  return v1;
}
```

We know that a1 is the path since we use opendir using that string. We then attempt to all the items in the directory. Looking up d_type on the object returned from readdir we can see that 4 is the enum for DT_DIR. So this function increments v1 for every directory in the directory that doesn't start with ".". This means this function is most likely used to count the number of directories within the current directory.

Going back into the main loop we go to look at the other function that is called:

```c
__int64 __fastcall sub_29E3(__int64 a1)
{

  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(name, &qword_6180);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::append(name, a1);
  v1 = opendir(name[0]);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v8, &qword_6180);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, name);
  if ( v10[1] == (const char *)0x3FFFFFFFFFFFFFFFLL )
    std::__throw_length_error("basic_string::append");
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_append(v10, "/", 1LL);
  sub_24A6(&qword_6180, v10, v2, v3);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
  if ( v1 )
  {
    v5 = readdir(v1);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, name);
    v6 = sub_2663(v10);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
    if ( v6 )
    {
      while ( v5 )
      {
        std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, name);
        sub_2BE4(v10, v5);
        std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
        v5 = readdir(v1);
      }
    }
    else
    {
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, a1);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v9, v8);
      sub_2C32(v9, v10, v5, v1);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v9);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
    }
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::resize(
      &qword_6180,
      qword_6188 - 1 - *(_QWORD *)(a1 + 8));
    closedir(v1);
  }
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v8);
  return std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(name);
}
```

```c
  v1 = opendir(name[0]);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v8, &qword_6180);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, name);
  if ( v10[1] == (const char *)0x3FFFFFFFFFFFFFFFLL )
    std::__throw_length_error("basic_string::append");
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_append(v10, "/", 1LL);
  sub_24A6(&qword_6180, v10, v2, v3);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
  if ( v1 )
```

The first that happens in this function is that we open the directory and then check if it is null, if it is not null then we continue to the meat of this function:

```c
    v5 = readdir(v1);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, name);
    v6 = sub_2663(v10);
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
    if ( v6 )
    {
      while ( v5 )
      {
        std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, name);
        sub_2BE4(v10, v5);
        std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
        v5 = readdir(v1);
      }
    }
    else
    {
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, a1);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v9, v8);
      sub_2C32(v9, v10, v5, v1);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v9);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
    }
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::resize(
      &qword_6180,
      qword_6188 - 1 - *(_QWORD *)(a1 + 8));
    closedir(v1);
```

We call readdir on the directory we opened earlier and then call sub_2663 (number of directories) again on the path we used to open the directory. 

If the number of directories is not 0 we have:

```c
    if ( v6 )
    {
      while ( v5 )
      {
        std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, name);
        sub_2BE4(v10, v5);
        std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
        v5 = readdir(v1);
      }
    }
```

We loop while the value from readdir is not null and then we call sub_2BE4 on the directory path and the current entity from readdir:

```c
__int64 __fastcall sub_2BE4(__int64 a1, __int64 a2)
{

  if ( *(_BYTE *)(a2 + 18) == 4 && *(_BYTE *)(a2 + 19) != 46 )
  {
    sub_2540(v3, (const char *)(a2 + 19));
    sub_29E3((__int64)v3);
    result = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v3);
  }
  return result;
}
```

In this function we check again if the entity is a directory and doesn't start with ".", if it is a directory we call the function we were just in.

Going back out to the function we are in we are now going to take a look at the else, what gets run when the number of directories in the current directory is 0:

```c
    else
    {
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v10, a1);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v9, v8);
      sub_2C32(v9, v10, v5, v1);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v9);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v10);
    }
```

We call some function and pass in the directory path, the entity, and the current diectory as well as one other value which we can reverse to find is the old path before the current directory was added to the path, making it the path one directory up:

```c
__int64 __fastcall sub_2C32(__int64 a1, __int64 a2, struct dirent *a3, DIR *a4)
{
  _BYTE *v5; // rax
  void *v6; // r12
  __int64 v7; // rax
  unsigned __int64 v8; // rdx
  char *v9; // rax
  unsigned __int64 v10; // rdx
  __int64 v11; // rax
  __int64 v12; // rbx
  char *v13; // r14
  __int64 v14; // rax
  __int64 v15; // rax
  size_t size; // [rsp+8h] [rbp-2E0h]
  __int64 v18; // [rsp+10h] [rbp-2D8h]
  char v20[32]; // [rsp+28h] [rbp-2C0h] BYREF
  __int64 v21[4]; // [rsp+48h] [rbp-2A0h] BYREF
  _QWORD v22[4]; // [rsp+68h] [rbp-280h] BYREF
  char *command[2]; // [rsp+88h] [rbp-260h] BYREF
  unsigned __int64 v24; // [rsp+98h] [rbp-250h] BYREF
  __int64 v25[2]; // [rsp+A8h] [rbp-240h] BYREF
  unsigned __int64 v26; // [rsp+B8h] [rbp-230h] BYREF

  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v20, a1);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::append(v20, a2);
  v5 = malloc(0x3E8uLL);
  size = 1000LL;
  *v5 = 0;
  v6 = v5;
  v18 = 0LL;
  while ( a3 )
  {
    if ( a3->d_type == 8 )
    {
      sub_2540(v25, a3->d_name);
      sub_2540(v22, "/");
      v7 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::insert(v22, 0LL, v20);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(command, v7);
      v8 = 15LL;
      v9 = &command[1][v25[1]];
      if ( (unsigned __int64 *)command[0] != &v24 )
        v8 = v24;
      if ( (unsigned __int64)v9 <= v8 )
        goto LABEL_13;
      v10 = 15LL;
      if ( (unsigned __int64 *)v25[0] != &v26 )
        v10 = v26;
      if ( (unsigned __int64)v9 > v10 )
LABEL_13:
        v11 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::append(command, v25);
      else
        v11 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::insert(v25, 0LL, command);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v21, v11);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(command);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v22);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
      std::ifstream::basic_ifstream(v25, v21[0], 6LL);
      v12 = std::istream::tellg((std::istream *)v25);
      v13 = (char *)operator new[](v12 + 1);
      std::istream::seekg(v25, 0LL, 0LL);
      std::istream::read((std::istream *)v25, v13, v12);
      std::ifstream::close(v25);
      v13[v12] = 0;
      while ( v18 + strlen(v13) > size )
      {
        size *= 2LL;
        v6 = realloc(v6, size);
      }
      strcat((char *)v6, v13);
      v18 += v12;
      free(v13);
      std::ifstream::~ifstream(v25);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v21);
    }
    a3 = readdir(a4);
  }
  sub_2540(v25, ".txt");
  v14 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::insert(v25, 0LL, v20);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v22, v14);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
  sub_2540(v25, "rm -rf ");
  v15 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::append(v25, v20);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(command, v15);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
  system(command[0]);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v25, v22);
  sub_26A3(v18, v6, v25);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
  free(v6);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(command);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v22);
  return std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v20);
}
```

This is a pretty meaty function so I decided to split it apart quite a bit, the first thing I did was look at what was at the big while loop:

```c
  while ( a3 )
  {
    if ( a3->d_type == 8 )
    {
      sub_2540(v25, a3->d_name);
      sub_2540(v22, "/");
      v7 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::insert(v22, 0LL, v20);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(command, v7);
      v8 = 15LL;
      v9 = &command[1][v25[1]];
      if ( (unsigned __int64 *)command[0] != &v24 )
        v8 = v24;
      if ( (unsigned __int64)v9 <= v8 )
        goto LABEL_13;
      v10 = 15LL;
      if ( (unsigned __int64 *)v25[0] != &v26 )
        v10 = v26;
      if ( (unsigned __int64)v9 > v10 )
LABEL_13:
        v11 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::append(command, v25);
      else
        v11 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::insert(v25, 0LL, command);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v21, v11);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(command);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v22);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
      std::ifstream::basic_ifstream(v25, v21[0], 6LL);
      v12 = std::istream::tellg((std::istream *)v25);
      v13 = (char *)operator new[](v12 + 1);
      std::istream::seekg(v25, 0LL, 0LL);
      std::istream::read((std::istream *)v25, v13, v12);
      std::ifstream::close(v25);
      v13[v12] = 0;
      while ( v18 + strlen(v13) > size )
      {
        size *= 2LL;
        v6 = realloc(v6, size);
      }
      strcat((char *)v6, v13);
      v18 += v12;
      free(v13);
      std::ifstream::~ifstream(v25);
      std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v21);
    }
    a3 = readdir(a4);
  }
```

Again here we seem to be looping through all of the items in the directory. However, this time we compare to a different enum (we can look this up to see it is DT_REG, or just regular files). We then seem to be doing some string work in order get the path to the current file, we know this since we use the current directory string, "/", and the current entity name. We then get the length of the file using seekg and we read the file into memory. We then reallocate the char array is we are above a certain size and hen we concatenate what we just read to another array.

After having concatenated all of the file contents into the array we move on the section after the while loop.


```c
  sub_2540(v25, ".txt");
  v14 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::insert(v25, 0LL, v20);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v22, v14);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
  sub_2540(v25, "rm -rf ");
  v15 = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::append(v25, v20);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(command, v15);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
  system(command[0]);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(v25, v22);
  sub_26A3(v18, v6, v25);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v25);
  free(v6);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(command);
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v22);
  return std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::_M_dispose(v20);
```

First we append ".txt" and the directory path to some new string. We then call rm -rf on the directory and then pass the concatenated bytes to another function, sub_26A3:

```c
__int64 __fastcall sub_26A3(__int64 a1, void *a2, _QWORD *a3)
{

  v3 = a1;
  std::ofstream::basic_ofstream(v51, *a3, 4LL);
  v4 = sub_25CA(a1);
  v47 = realloc(a2, a1 + v4);
LABEL_2:
  v5 = sqrt((double)(int)v3);
  v6 = 0LL;
  v7 = (unsigned int)(int)v5;
  while ( 1 )
  {
    if ( v7 + 1 < v6 )
    {
      v47[v3++] = 0;
      goto LABEL_2;
    }
    v8 = v6 + v7;
    v9 = v3 / (v6 + v7);
    v46 = v9;
    v10 = v3 % (v6 + v7);
    if ( !v10 )
      break;
    ++v6;
  }
  v11 = 8 * v9;
  v12 = 0LL;
  v13 = malloc(8 * v9);
  v14 = (_DWORD **)malloc(v11);
  v15 = 8 * v8;
  v16 = v14;
  while ( v12 < v46 )
  {
    v49 = v15;
    v13[v12] = malloc(v15);
    v17 = malloc(4 * v8);
    v15 = v49;
    v16[v12++] = v17;
  }
  v18 = 0LL;
  v19 = 0;
  v20 = 0LL;
  while ( v18 < v46 )
  {
    for ( i = 0LL; i < v8; ++i )
    {
      *(_BYTE *)(v13[v18] + i) = v47[v20 + i];
      v16[v18][i] = v19 + i;
    }
    ++v18;
    v20 += v8;
    v19 += v8;
  }
  v22 = v3;
  v23 = 1;
  size = 4LL * (int)v3;
  v24 = malloc(size);
  v25 = 69;
  v26 = v24;
  *v24 = **v16;
  while ( v23 < v22 )
  {
    v27 = v25;
    v50 = v25;
    sub_2606(v25, 1);
    v28 = sub_2606(v27, 0);
    v32 = v31 + v28;
    switch ( v25 )
    {
      case 'N':
        v33 = v30 >> 2 <= v32;
        goto LABEL_27;
      case 'E':
        v34 = v29;
        v35 = v8;
        v36 = v30 >> 2;
        goto LABEL_23;
      case 'S':
        v35 = v46;
        v34 = v32;
        v36 = v30 >> 2;
LABEL_23:
        if ( v34 >= v35 - v36 )
          goto LABEL_28;
LABEL_30:
        v37 = v23++;
        v26[v37] = v16[v32][v29];
        break;
      case 'W':
        v33 = v30 >> 2 <= v29;
LABEL_27:
        if ( v33 )
          goto LABEL_30;
LABEL_28:
        v25 = sub_263A(v50);
        break;
      default:
        goto LABEL_30;
    }
  }
  v38 = malloc(size);
  v39 = v22 - (__int64)(v22 / 5);
  for ( j = v39; v22 > j; ++j )
  {
    v43 = v26[j];
    v38[v10++] = v43;
  }
  v41 = (char *)&v38[v10];
  if ( v39 < 0 )
    v39 = 0LL;
  v42 = 0;
  qmemcpy(v41, v26, 4 * v39);
  while ( v42 < v22 )
  {
    v44 = sub_2656((__int64)v26, v42);
    sub_2522((__int64)v51, v47[v38[v44]]);
    ++v42;
  }
  sub_2522((__int64)v51, 97);
  sub_2522((__int64)v51, 98);
  sub_2522((__int64)v51, 114);
  sub_2522((__int64)v51, 111);
  sub_2522((__int64)v51, 108);
  sub_2522((__int64)v51, 121);
  std::ofstream::close(v51);
  return std::ofstream::~ofstream(v51);
}
```

This function was pretty obviously the algorithm so lets get started:

```c
  v3 = a1;
  std::ofstream::basic_ofstream(v51, *a3, 4LL);
  v4 = sub_25CA(a1);
  v47 = realloc(a2, a1 + v4);
```

We start off by opening an outstream and then we call sub_25CA:

```c
__int64 __fastcall sub_25CA(__int64 a1)
{

  v1 = a1;
  do
  {
    v2 = (double)(int)v1;
    v3 = v1;
    v4 = v1++;
    v5 = v3 - a1;
    v6 = sqrt(v2);
  }
  while ( v4 != (unsigned int)(int)v6 * (unsigned __int64)(unsigned int)(int)v6 );
  return v5;
}
```

This function seems to start with the length that is passed to it and then increases that value till it is a perfect square, we then return the difference between the perfect square and the current length.

After the function call we allocate the perfect square length of data. The next component of the algorithm is a while loop:

```c
LABEL_2:
  v5 = sqrt((double)(int)v3);
  v6 = 0LL;
  v7 = (unsigned int)(int)v5;
  while ( 1 )
  {
    if ( v7 + 1 < v6 )
    {
      v47[v3++] = 0;
      goto LABEL_2;
    }
    v8 = v6 + v7;
    v9 = v3 / (v6 + v7);
    v46 = v9;
    v10 = v3 % (v6 + v7);
    if ( !v10 )
      break;
    ++v6;
  }
```

This loop keeps looping until v3 (length) modded by v6 plus v7 was equal to 0. v7 starts out by being the sqrt of the length of the data. We then keep incrementing v6 from 0 till we reach the checkpoint. If we are unable to make the data rectangle shaped (perfectly dividing closest square plus a value less than or equal to that value) then we add a \x00 to the end of the data and redo the loop.

We then move on to the next loop:

```c
  v11 = 8 * v9;
  v12 = 0LL;
  v13 = malloc(8 * v9);
  v14 = (_DWORD **)malloc(v11);
  v15 = 8 * v8;
  v16 = v14;
  while ( v12 < v46 )
  {
    v49 = v15;
    v13[v12] = malloc(v15);
    v17 = malloc(4 * v8);
    v15 = v49;
    v16[v12++] = v17;
  }
```

What we are doing here is just going through and mallocing space for all the data we need to shuffle around. We then move on to the next loop:

```c
  v18 = 0LL;
  v19 = 0;
  v20 = 0LL;
  while ( v18 < v46 )
  {
    for ( i = 0LL; i < v8; ++i )
    {
      *(_BYTE *)(v13[v18] + i) = v47[v20 + i];
      v16[v18][i] = v19 + i;
    }
    ++v18;
    v20 += v8;
    v19 += v8;
  }
```

In this function we are creating two different arrays, one with the values from the original data, and the other is the index we used to get that value from the original data. Ok, time for the next section:

```c
 v22 = v3;
  v23 = 1;
  size = 4LL * (int)v3;
  v24 = malloc(size);
  v25 = 69;
  v26 = v24;
  *v24 = **v16;
  while ( v23 < v22 )
  {
    v27 = v25;
    v50 = v25;
    sub_2606(v25, 1);
    v28 = sub_2606(v27, 0);
    v32 = v31 + v28;
    switch ( v25 )
    {
      case 'N':
        v33 = v30 >> 2 <= v32;
        goto LABEL_27;
      case 'E':
        v34 = v29;
        v35 = v8;
        v36 = v30 >> 2;
        goto LABEL_23;
      case 'S':
        v35 = v46;
        v34 = v32;
        v36 = v30 >> 2;
LABEL_23:
        if ( v34 >= v35 - v36 )
          goto LABEL_28;
LABEL_30:
        v37 = v23++;
        v26[v37] = v16[v32][v29];
        break;
      case 'W':
        v33 = v30 >> 2 <= v29;
LABEL_27:
        if ( v33 )
          goto LABEL_30;
LABEL_28:
        v25 = sub_263A(v50);
        break;
      default:
        goto LABEL_30;
    }
  }
```

The first thing we need to figure out in this function is what the sub_2606 function does:

```c
__int64 __fastcall sub_2606(char a1, int a2)
{
  bool v2; // al
  __int64 result; // rax

  if ( a1 != 78 )
  {
    if ( a1 == 69 )
    {
      LOBYTE(result) = a2 == 1;
    }
    else
    {
      if ( a1 != 83 )
      {
        v2 = a2 == 1;
        return (unsigned int)-v2;
      }
      LOBYTE(result) = a2 != 1;
    }
    return (unsigned __int8)result;
  }
  v2 = a2 != 1;
  return (unsigned int)-v2;
}
```

This function gets passed 2 values, a1 is either 'N', 'E', 'S', or 'W', while a2 is either 1 or 0. This function then either returns 1, 0, or -1 based on a1 and a2.

We also have another function, sub_263A, that we need to consider:

```c
char __fastcall sub_263A(char a1)
{
  char result; // al

  result = 69;
  if ( a1 != 78 )
  {
    result = 83;
    if ( a1 != 69 )
      result = 9 * (a1 == 83) + 78;
  }
  return result;
}
```

This function takes in either 'N', 'E', 'S', or 'W' and then returns another value based on that.

Looking back at the algorithm, we can see that we loop through the length of the data and then get two different directions, then based on an if statement  we either get a new move direction or we copy data from the second array to the new array we malloced.

We then move on to the next section:

```c
  v38 = malloc(size);
  v39 = v22 - (__int64)(v22 / 5);
  for ( j = v39; v22 > j; ++j )
  {
    v43 = v26[j];
    v38[v10++] = v43;
  }
  v41 = (char *)&v38[v10];
  if ( v39 < 0 )
    v39 = 0LL;
  v42 = 0;
  qmemcpy(v41, v26, 4 * v39);
  while ( v42 < v22 )
  {
    v44 = sub_2656((__int64)v26, v42);
    sub_2522((__int64)v51, v47[v38[v44]]);
    ++v42;
  }
  sub_2522((__int64)v51, 97);
  sub_2522((__int64)v51, 98);
  sub_2522((__int64)v51, 114);
  sub_2522((__int64)v51, 111);
  sub_2522((__int64)v51, 108);
  sub_2522((__int64)v51, 121);
  std::ofstream::close(v51);
```

We then malloc a new byte array and move data from the array we just made into the array. We move the data by first getting a value by dividing the length by 5 and then we move the data starting at that index to the end of the data to the new array. Then we move the data from the start to that index over. So something like abcde would become bcdea.

After that we loop through all the data and write it out to a file, after writing all the data we write abroly to the file as well, and then close the file.

From here you can make a python version of the algorithm:

```py
# Imports
import numpy as np

# Factor the length as square-like as possible
# for maximum swirlage
def factosize(msg):
    ln = len(msg)
    # Start at the square
    start = int(np.sqrt(ln))
    # Keep checking
    for i in range((start+2)//1):
        x = start + i
        if ln % x == 0:
            return ln // x, x
    return "Not square enough! :C"

def WannaSwirl(in_bytes, nperm=-1):

    # Ceil function
    def ceil(a, b):
        return -(-a // b)

    # Shape input bytes to a matrix
    while True:
        arrshape = factosize(in_bytes)
        if type(arrshape) != str:
            break
        # Add padding when necessary
        in_bytes += b'\x00'
    bytmat = np.array(list(in_bytes)).reshape(arrshape)
    
    # Some standard amount of swirl
    if nperm < 0:
        nperm = bytmat.size//5
    
    # Move dictionaries
    move_dic = {'N':(-1,0),'E':(0,1),'S':(1,0),'W':(0,-1)}
    turn_dic = {'N':'E','E':'S','S':'W','W':'N'}
    
    # SWIRL TIME 
    i_mat = np.arange(bytmat.size).reshape(bytmat.shape)
    # Get matrix shape
    h, w = bytmat.shape
    # Initial conditions
    y,x = 0,0    # start at 0,0
    movdir = 'E' # start going East
    vortex = [i_mat[y,x]]
    # Loop time
    i = 1
    while len(vortex) < i_mat.size:
        try:
            # Get new coords
            xnew = x + move_dic[movdir][1]
            ynew = y + move_dic[movdir][0]
            # Check if we hit an edge or a previous path
            if (movdir=='N') and (ynew < i//4): raise
            if (movdir=='E') and (xnew >= w-i//4): raise
            if (movdir=='S') and (ynew >= h-i//4): raise
            if (movdir=='W') and (xnew < i//4): raise
            # Append it to vortex list
            vortex.append(i_mat[ynew,xnew])
        except:
            # Turn clockwise
            movdir = turn_dic[movdir]
            i += 1
            continue
        # Set new coords
        x = xnew
        y = ynew
    # Permutate vortex
    if (nperm != 0) or (nperm != len(vortex)):
        perm = np.append(vortex[-nperm:], vortex[:-nperm])
    else:
        perm = vortex
    # Permutation dictionary
    perm_dic = { vortex[i] : int(perm[i]) for i in range(bytmat.size) }
    # Swirl the data
    swirled = np.zeros(bytmat.size, dtype=int)
    for i in range(bytmat.size):
        swirled[i] = bytmat.flatten()[ perm_dic[i] ]
    # Ta-da!
    return bytes(list(swirled))

trial = b'flag{}'

enc = (WannaSwirl(trial,nperm=-1)) + b'abroly'
```

This works as a proof of concept and if you only encrypt one directory deep using the binary you can get the same result.

 I then made a python script to undo what the original swirl function had done. We know that everything is already sized correctly if we remove the abroly at the end so we can just reshape the matrix rather than trying to find the correct size. The vortex will be the same as well, so then all we need to do is use the values from the dictionary to undo what the swirl has done:

 ```py 
for i in range(bytmat.size):
    swirled[perm_dic[i]] = msg[i]
```

Writing a full script I got:

```py
# Imports
import numpy as np
import sys

# Factor the length as square-like as possible
# for maximum swirlage
def factosize(msg):
    ln = len(msg)
    # Start at the square
    start = int(np.sqrt(ln))
    # Keep checking
    for i in range((start+2)//1):
        x = start + i
        if ln % x == 0:
            return ln // x, x
    return "Not square enough! :C"

    
def unswirl(msg):
    bytmat = np.array(list(msg)).reshape(factosize(msg))
    
    # Some standard amount of swirl
    nperm = bytmat.size//5
    
    # Move dictionaries
    move_dic = {'N':(-1,0),'E':(0,1),'S':(1,0),'W':(0,-1)}
    turn_dic = {'N':'E','E':'S','S':'W','W':'N'}
    
    # SWIRL TIME 
    i_mat = np.arange(bytmat.size).reshape(bytmat.shape)
    # Get matrix shape
    h, w = bytmat.shape
    # Initial conditions
    y,x = 0,0    # start at 0,0
    movdir = 'E' # start going East
    vortex = [i_mat[y,x]]
    # Loop time
    i = 1
    while len(vortex) < i_mat.size:
        try:
            # Get new coords
            xnew = x + move_dic[movdir][1]
            ynew = y + move_dic[movdir][0]
            # Check if we hit an edge or a previous path
            if (movdir=='N') and (ynew < i//4): raise
            if (movdir=='E') and (xnew >= w-i//4): raise
            if (movdir=='S') and (ynew >= h-i//4): raise
            if (movdir=='W') and (xnew < i//4): raise
            # Append it to vortex list
            vortex.append(i_mat[ynew,xnew])
        except:
            # Turn clockwise
            movdir = turn_dic[movdir]
            i += 1
            continue
        # Set new coords
        x = xnew
        y = ynew
    # Permutate vortex
    if (nperm != 0) or (nperm != len(vortex)):
        perm = np.append(vortex[-nperm:], vortex[:-nperm])
    else:
        perm = vortex
    # Permutation dictionary
    perm_dic = { vortex[i] : int(perm[i]) for i in range(bytmat.size) }
    # Swirl the data
    swirled = np.zeros(bytmat.size, dtype=int)

    for i in range(bytmat.size):
        swirled[perm_dic[i]] = msg[i]

    return bytes(list(swirled))

enc = open(sys.argv[1], "rb").read().rstrip(b'abroly')

dec = unswirl(enc)
```

I then added some code at the bottom of the file to search for the flag:

```py
flg = str(dec).index('flag{')

print(str(dec)[flg:flg+100])
```

Running this on the original FlagFolder.txt I get:

```
flag{w0w_1m_g3tt1ng_s0_d1zzy_1_th1nk_1m_g0nn4_puk3}Lorem ipsum dolor sit amet, consectetur adipiscin
```

However the original file had an unintended solve where you could simply ctrl + f for { and see the flag so another file was released. Running our solve script on that file we get that the substring wasn't find so I decided to print out dec:

```
b'QRSTUVWf\n98765432}00t_n14rb_ym_d31rFGXl1ws_lr1ws4l}aEPONMLKJ{{w4nng_YDCBAzyxwvutsrIZaHqponmlkjihgfedcbabrolyabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{_}123456789\nabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{_}123456789\nabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{_}123456789\nabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{_}123456789\n'
```

We can see that there is another abroly in the output so now we need to consider what the program does. We know how to reverse a single swirl, but the program swirls every single directory starting from the farthest down. Encrypting the directory, moving it up one directory, encrypting again, until all of the subfolders have been encrypted into one single massive file. With the subfolders having been encrypted multiple times. 

Lucily for us, the hacker wasn't very smart and added abroly to the end of each swirl so we basically have a partition that we can look up. So what we need to do it split on that partition and run the unswirl function yet again:

```py
# Imports
import numpy as np
import sys

# Factor the length as square-like as possible
# for maximum swirlage
def factosize(msg):
    ln = len(msg)
    # Start at the square
    start = int(np.sqrt(ln))
    # Keep checking
    for i in range((start+2)//1):
        x = start + i
        if ln % x == 0:
            return ln // x, x
    return "Not square enough! :C"

    
def unswirl(msg):
    bytmat = np.array(list(msg)).reshape(factosize(msg))
    
    # Some standard amount of swirl
    nperm = bytmat.size//5
    
    # Move dictionaries
    move_dic = {'N':(-1,0),'E':(0,1),'S':(1,0),'W':(0,-1)}
    turn_dic = {'N':'E','E':'S','S':'W','W':'N'}
    
    # SWIRL TIME 
    i_mat = np.arange(bytmat.size).reshape(bytmat.shape)
    # Get matrix shape
    h, w = bytmat.shape
    # Initial conditions
    y,x = 0,0    # start at 0,0
    movdir = 'E' # start going East
    vortex = [i_mat[y,x]]
    # Loop time
    i = 1
    while len(vortex) < i_mat.size:
        try:
            # Get new coords
            xnew = x + move_dic[movdir][1]
            ynew = y + move_dic[movdir][0]
            # Check if we hit an edge or a previous path
            if (movdir=='N') and (ynew < i//4): raise
            if (movdir=='E') and (xnew >= w-i//4): raise
            if (movdir=='S') and (ynew >= h-i//4): raise
            if (movdir=='W') and (xnew < i//4): raise
            # Append it to vortex list
            vortex.append(i_mat[ynew,xnew])
        except:
            # Turn clockwise
            movdir = turn_dic[movdir]
            i += 1
            continue
        # Set new coords
        x = xnew
        y = ynew
    # Permutate vortex
    if (nperm != 0) or (nperm != len(vortex)):
        perm = np.append(vortex[-nperm:], vortex[:-nperm])
    else:
        perm = vortex
    # Permutation dictionary
    perm_dic = { vortex[i] : int(perm[i]) for i in range(bytmat.size) }
    # Swirl the data
    swirled = np.zeros(bytmat.size, dtype=int)

    for i in range(bytmat.size):
        swirled[perm_dic[i]] = msg[i]

    return bytes(list(swirled))

enc = open(sys.argv[1], "rb").read().rstrip(b'abroly')

dec = unswirl(enc)

print(unswirl(dec.split(b'abroly')[0]))
```

When run on the file the output is:

```
b'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{_}123456789\nflag{w4nn4sw1rl_sw1rl3d_my_br41n_t00}'
```