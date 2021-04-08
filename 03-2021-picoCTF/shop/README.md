# picoCTF Shop Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: Reverse Engineering

Comments:

Best Stuff - Cheap Stuff, Buy Buy Buy... Store Instance: source. The shop is open for business at nc mercury.picoctf.net 34938.

## Write up:

When first running the program we see:

```
Welcome to the market!
=====================
You have 40 coins
        Item            Price   Count
(0) Quiet Quiches       10      12
(1) Average Apple       15      8
(2) Fruitful Flag       100     1
(3) Sell an Item
(4) Exit
Choose an option: 
```

I then checked the file and saw that it was a 32 bit ELF file so I opening it up in a decompiler. There were a lot of function but I found one called main_get_flag:

```c
void __noreturn main_get_flag()
{
...

  while ( (unsigned int)&retaddr <= *(_DWORD *)(*(_DWORD *)(__readgsdword(0) - 4) + 8) )
    runtime_morestack_noctxt();
  filename.str = (uint8 *)&str.buf.cap;
  filename.len = 8;
  io_ioutil_ReadFile(filename, v3, e);
  main_check(ea);
  v9 = v4;
  v10 = v6;
  a.cap = 0;
  v12 = 0;
  a.array = (interface_{} *)&::e;
  a.len = (__int32)&main_statictmp_14;
  filenamea.str = (uint8 *)&stru_80E2E20;
  filenamea.len = (__int32)&v9;
  runtime_convT2Eslice((runtime_eface_0)filenamea, v4, (runtime__type_0 *)v6.tab);
  a.cap = v5;
  v12 = v6.tab;
  filenameb.array = (interface_{} *)&a;
  *(_QWORD *)&filenameb.len = 0x200000002LL;
  fmt_Println(filenameb, v6, (__int32)ea.tab);
  os_Exit(0);
}
```

This seemed to read in a file on their system meaning that I would need to find a way to activate this function, looking at the xref's I found that it was called in the main_menu function, this is a fairly large function so I'm only going to include the important parts:

```c
...

fmt_Scanf(ak, (error_0)0x100000001LL, *((string *)&tc + 1), v53); 
    v9 = *_num; // number we chose to buy
    v10 = _choice; // item we chose
    if ( (unsigned int)*_choice >= inv.len ) // if the choice isn't in the inventory then exit
      runtime_panicindex();
    v11 = *_choice;
    v12 = inv.array[v11].count;
    if ( v9 <= v12 ) // check that number wanted is less than number they have
    {
      if ( wallet < inv.array[v11].price )
      {
        v58.array = (interface_{} *)&e;
        v58.len = (__int32)&main_statictmp_7;
        an.array = (interface_{} *)&v58;
        *(_QWORD *)&an.len = 0x100000001LL;
        fmt_Println(an, (error_0)tc, SDWORD2(tc));
        v15 = wallet;
      }
      else
      {
        inv.array[v11].count = v12 - v9;
        if ( (unsigned int)*v10 >= inv.len )
          runtime_panicindex();
        v14 = *v10;
        v15 = wallet - *_num * inv.array[v14].price; // vuln: doesn't check if the value is negative so we can buy a negative amount
        if ( (unsigned int)*v10 >= user.len )
          runtime_panicindex();
        user.array[v14].count += *_num;
        if ( inv.len <= 2u )
          runtime_panicindex();
        if ( inv.array[2].count != 1 )          // if fruitful flag is not 1 count
          main_get_flag();
      }
      v13 = v15;

...
```

So I then went into the shop and started buying negative amounts of items to get money. Once I had 100+ coins I bought the flag:

```
How many do you want to buy?
-10
You have 190 coins
        Item            Price   Count
(0) Quiet Quiches       10      27
(1) Average Apple       15      8
(2) Fruitful Flag       100     1
(3) Sell an Item
(4) Exit
Choose an option: 
2
How many do you want to buy?
1
Flag is:  [112 105 99 111 67 84 70 123 98 52 100 95 98 114 111 103 114 97 109 109 101 114 95 98 97 54 98 56 99 100 102 125]
```

These were decimal ascii values, once translated I got that the flag was:

```
picoCTF{b4d_brogrammer_ba6b8cdf}
```