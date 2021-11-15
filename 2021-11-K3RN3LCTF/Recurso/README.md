# K3RN3LCTF Recurso Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering


## Write up:

Opening the binary in a decompiler we can see that we are lucky and the binary wasn't stripped which is awesome. So we start by taking a look at the main function:

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  if ( argc > 1 && strstr(argv[1], ".recc") )
  {
    v7 = (unsigned int *)initBytecodeFileWithFile(argv[1]);
    runProgram(*((_QWORD *)v7 + 2), *((_QWORD *)v7 + 3), *v7);
  }
  else if ( argc > 1 && strstr(argv[1], ".rec") )
  {
    fileData = (void *)readFile(argv[1], &fileSize);
    v5 = fileData;
    program = initProgramNode();
    v6 = initFunctionNode("main");
    addElementToProgramNode(program, v6);
    curFunction = v6;
    parseFunction(program, &v5);
    if ( argc == 4 && !strcmp(argv[2], "-s") )
      v3 = argv[3];
    else
      v3 = (const char *)&unk_5008;
    compileBytecode(program, v3);
    free(fileData);
    freeProgramNode(program);
  }
  return 0;
}
```

The file we were given is a .recc file which means we can ignore the else if statement and dive straight into the if statement. Since we check if .recc is in argv[1] we can assume that that is our file name and we pass it into initBytecodeFileWithFile:

```c
int *__fastcall initBytecodeFileWithFile(const char *a1)
{
  v3 = (int *)malloc(0x20uLL);
  stream = fopen(a1, "rb");
  fread(v3 + 2, 4uLL, 1uLL, stream);
  *((_QWORD *)v3 + 2) = malloc(v3[2] + 1);
  fread(*((void **)v3 + 2), 1uLL, v3[2], stream);
  v3[1] = 1024;
  *((_QWORD *)v3 + 3) = malloc(v3[1] + 1);
  for ( i = fread(*((void **)v3 + 3), 1uLL, 0x400uLL, stream);
        i == 1024;
        i = fread((void *)(*((_QWORD *)v3 + 3) + *v3), 1uLL, 0x400uLL, stream) )
  {
    *v3 += 1024;
    v3[1] += 1024;
    *((_QWORD *)v3 + 3) = realloc(*((void **)v3 + 3), v3[1] + 1);
  }
  *v3 += i;
  fclose(stream);
  return v3;
}
```

The first thing that we do is malloc some space and then open the file and read 4 bytes, the size of an int. We then malloc the amount of memory that we just read from the first 4 bytes and read that amount of data. The rest of the function seems to start by mallocing some memory and then reading in the data, reallocating more memory if needed.

We then go into the next function:

```c
__int64 __fastcall runProgram(__int64 a1, __int64 a2)
{
  v76 = initializeFunctions(a1);
  v84 = 0;
  v83 = 0;
  v82 = 0;
  v81 = 0;
  v80 = 64;
  ptr = malloc(0x200uLL);
  for ( i = 0; i < v80; ++i )
    *((_QWORD *)ptr + i) = malloc(8uLL);
  while ( 1 )
  {
    result = *(unsigned __int8 *)(v84 + a2);
    if ( (_BYTE)result == 13 )
      return result;
    v2 = v84++;
    v75 = *(_BYTE *)(v2 + a2);
    switch ( v75 )
    {
      case 1:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v3 = v83++;
        *(_QWORD *)&v73[8 * v3 + 8] = *((_QWORD *)&v74 + 1) - v74;
        break;
      case 2:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v4 = v83++;
        *(_QWORD *)&v73[8 * v4 + 8] = v74 * *((_QWORD *)&v74 + 1);
        break;
      case 3:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v6 = v83++;
        *(_QWORD *)&v73[8 * v6 + 8] = *((_QWORD *)&v74 + 1) + v74;
        break;
      case 8:
        v10 = v84++;
        v73[0] = *(_BYTE *)(v10 + a2);
        v11 = v84++;
        v73[1] = *(_BYTE *)(v11 + a2);
        v12 = v84++;
        v73[2] = *(_BYTE *)(v12 + a2);
        v13 = v84++;
        v73[3] = *(_BYTE *)(v13 + a2);
        v14 = v84++;
        v73[4] = *(_BYTE *)(v14 + a2);
        v15 = v84++;
        v73[5] = *(_BYTE *)(v15 + a2);
        v16 = v84++;
        v73[6] = *(_BYTE *)(v16 + a2);
        v17 = v84++;
        v73[7] = *(_BYTE *)(v17 + a2);
        v18 = v83++;
        *(_QWORD *)&v73[8 * v18 + 8] = bytesToLongLong(v73);
        break;
      case 11:
        v19 = v84++;
        v69 = *(_BYTE *)(v19 + a2);
        v20 = v84++;
        v70 = *(_BYTE *)(v20 + a2);
        v21 = v84++;
        v71 = *(_BYTE *)(v21 + a2);
        v22 = v84++;
        v72 = *(_BYTE *)(v22 + a2);
        v23 = *(_QWORD *)&v73[8 * --v83 + 8];
        v24 = bytesToInt(&v69);
        setLocal(*(_QWORD *)(8LL * v81 + v76), v24, v23);
        break;
      case 12:
        v25 = v84++;
        v69 = *(_BYTE *)(v25 + a2);
        v26 = v84++;
        v70 = *(_BYTE *)(v26 + a2);
        v27 = v84++;
        v71 = *(_BYTE *)(v27 + a2);
        v28 = v84++;
        v72 = *(_BYTE *)(v28 + a2);
        v29 = bytesToInt(&v69);
        v30 = *(_QWORD *)(8LL * v81 + v76);
        v31 = v83++;
        *(_QWORD *)&v73[8 * v31 + 8] = getLocal(v30, v29);
        break;
      case 14:
        decrementFunction(*(_QWORD *)(8LL * v81 + v76));
        v84 = **((_DWORD **)ptr + --v82);
        v81 = *(_DWORD *)(*((_QWORD *)ptr + v82) + 4LL);
        break;
      case 15:
        printf("%lld\n", *(_QWORD *)&v73[8 * --v83 + 8]);
        break;
      case 16:
        --v83;
        break;
      case 19:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v5 = v83++;
        *(_QWORD *)&v73[8 * v5 + 8] = *((_QWORD *)&v74 + 1) / (__int64)v74;
        break;
      case 21:
        v32 = v84++;
        v69 = *(_BYTE *)(v32 + a2);
        v33 = v84++;
        v70 = *(_BYTE *)(v33 + a2);
        v34 = v84++;
        v71 = *(_BYTE *)(v34 + a2);
        v35 = v84++;
        v72 = *(_BYTE *)(v35 + a2);
        if ( v82 >= v80 )
        {
          v80 *= 2;
          ptr = realloc(ptr, 8LL * v80);
          for ( j = v82; j < v80; ++j )
          {
            v36 = (void **)((char *)ptr + 8 * j);
            *v36 = malloc(8uLL);
          }
        }
        *(_DWORD *)(*((_QWORD *)ptr + v82) + 4LL) = v81;
        **((_DWORD **)ptr + v82++) = v84;
        v81 = bytesToInt(&v69);
        v84 = **(_DWORD **)(8LL * v81 + v76);
        incrementFunction(*(_QWORD *)(8LL * v81 + v76));
        break;
      case 22:
        __isoc99_scanf("%lld", &v68);
        v37 = v83++;
        *(_QWORD *)&v73[8 * v37 + 8] = v68;
        break;
      case 23:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        if ( *((_QWORD *)&v74 + 1) != (_QWORD)v74 )
        {
          do
          {
            v38 = v84++;
            v75 = *(_BYTE *)(v38 + a2);
          }
          while ( v75 != 14 );
        }
        break;
      case 24:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v7 = v83++;
        *(_QWORD *)&v73[8 * v7 + 8] = v74 | *((_QWORD *)&v74 + 1);
        break;
      case 25:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v8 = v83++;
        *(_QWORD *)&v73[8 * v8 + 8] = v74 & *((_QWORD *)&v74 + 1);
        break;
      case 26:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v9 = v83++;
        *(_QWORD *)&v73[8 * v9 + 8] = v74 ^ *((_QWORD *)&v74 + 1);
        break;
      case 27:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        if ( *((_QWORD *)&v74 + 1) == (_QWORD)v74 )
        {
          do
          {
            v39 = v84++;
            v75 = *(_BYTE *)(v39 + a2);
          }
          while ( v75 != 14 );
        }
        break;
      case 28:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        if ( *((__int64 *)&v74 + 1) < (__int64)v74 )
        {
          do
          {
            v40 = v84++;
            v75 = *(_BYTE *)(v40 + a2);
          }
          while ( v75 != 14 );
        }
        break;
      case 29:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        if ( *((__int64 *)&v74 + 1) > (__int64)v74 )
        {
          do
          {
            v41 = v84++;
            v75 = *(_BYTE *)(v41 + a2);
          }
          while ( v75 != 14 );
        }
        break;
      case 30:
        v42 = v84++;
        v69 = *(_BYTE *)(v42 + a2);
        v43 = v84++;
        v70 = *(_BYTE *)(v43 + a2);
        v44 = v84++;
        v71 = *(_BYTE *)(v44 + a2);
        v45 = v84++;
        v72 = *(_BYTE *)(v45 + a2);
        v84 = bytesToInt(&v69);
        break;
      case 31:
        v84 = *(_QWORD *)&v73[8 * --v83 + 8];
        break;
      case 32:
        v46 = v84++;
        v69 = *(_BYTE *)(v46 + a2);
        v47 = v84++;
        v70 = *(_BYTE *)(v47 + a2);
        v48 = v84++;
        v71 = *(_BYTE *)(v48 + a2);
        v49 = v84++;
        v72 = *(_BYTE *)(v49 + a2);
        v50 = (int)bytesToInt(&v69);
        ++*(_BYTE *)(v50 + a2);
        break;
      case 33:
        v51 = v84++;
        v69 = *(_BYTE *)(v51 + a2);
        v52 = v84++;
        v70 = *(_BYTE *)(v52 + a2);
        v53 = v84++;
        v71 = *(_BYTE *)(v53 + a2);
        v54 = v84++;
        v72 = *(_BYTE *)(v54 + a2);
        v55 = (int)bytesToInt(&v69);
        --*(_BYTE *)(v55 + a2);
        break;
      case 34:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_BYTE *)(v74 + a2) = *(_BYTE *)(*((_QWORD *)&v74 + 1) + a2);
        break;
      case 35:
        v56 = v84++;
        v69 = *(_BYTE *)(v56 + a2);
        v57 = v84++;
        v70 = *(_BYTE *)(v57 + a2);
        v58 = v84++;
        v71 = *(_BYTE *)(v58 + a2);
        v59 = v84++;
        v72 = *(_BYTE *)(v59 + a2);
        v60 = *(unsigned __int8 *)((int)bytesToInt(&v69) + a2) << 8;
        v61 = v60 + *(unsigned __int8 *)((int)bytesToInt(&v69) + 1LL + a2);
        v62 = v83++;
        *(_QWORD *)&v73[8 * v62 + 8] = v61;
        break;
      case 36:
        v63 = v84++;
        v69 = *(_BYTE *)(v63 + a2);
        v64 = v84++;
        v70 = *(_BYTE *)(v64 + a2);
        v65 = v84++;
        v71 = *(_BYTE *)(v65 + a2);
        v66 = v84++;
        v72 = *(_BYTE *)(v66 + a2);
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        if ( *((_QWORD *)&v74 + 1) == (_QWORD)v74 )
          v84 = bytesToInt(&v69);
        break;
      default:
        continue;
    }
  }
}
```

This function is definitly the meat of our program, this is the interpreter that runs the recurso bytecode and executes our program. The first thing I wanted to look into was checking what intializeFunctions does:

```c
void *__fastcall initializeFunctions(char *a1)
{
  v25 = 2;
  v24 = 1;
  ptr = malloc(0x10uLL);
  v22 = 1;
  for ( i = *a1; i == 17; i = a1[v16] )
  {
    v1 = v22++;
    for ( i = a1[v1]; i != 18; i = a1[v2] )
      v2 = v22++;
    v3 = v22++;
    v19[0] = a1[v3];
    v4 = v22++;
    v19[1] = a1[v4];
    v5 = v22++;
    v19[2] = a1[v5];
    v6 = v22++;
    v19[3] = a1[v6];
    v7 = v22++;
    v18[0] = a1[v7];
    v8 = v22++;
    v18[1] = a1[v8];
    v9 = v22++;
    v18[2] = a1[v9];
    v10 = v22++;
    v18[3] = a1[v10];
    if ( v24 >= v25 )
    {
      v25 *= 2;
      ptr = realloc(ptr, 8LL * v25);
    }
    v11 = (void **)((char *)ptr + 8 * v24 - 8);
    *v11 = malloc(0x20uLL);
    v12 = (_DWORD *)*((_QWORD *)ptr + v24 - 1);
    *v12 = bytesToInt(v18);
    *(_DWORD *)(*((_QWORD *)ptr + v24 - 1) + 20LL) = 0;
    v13 = *((_QWORD *)ptr + v24 - 1);
    *(_DWORD *)(v13 + 16) = bytesToInt(v19);
    *(_DWORD *)(*((_QWORD *)ptr + v24 - 1) + 24LL) = v24 == 1;
    v14 = *((_QWORD *)ptr + v24 - 1);
    *(_QWORD *)(v14 + 8) = malloc(8LL * *(int *)(v14 + 16));
    for ( j = 0; j < *(_DWORD *)(*((_QWORD *)ptr + v24 - 1) + 16LL); ++j )
    {
      v15 = (void **)(*(_QWORD *)(*((_QWORD *)ptr + v24 - 1) + 8LL) + 8LL * j);
      *v15 = malloc(0xA0uLL);
    }
    ++v24;
    v16 = v22++;
  }
  return ptr;
}
```

This function starts by mallocing some space and then it checks if the first value in a1 is equal to 17. We then loop and increment a1 while a1 is not equal to 18. After that we fill in two different byte arrays of size 4 each while incrementing a1. We then check if one size is greater than another, if it is then we reallocate the memory we malloced at the start.

Then we malloc an object and fill in several of its parts, calling bytesToInt on the two arrays we populated earlier. After that we malloc some more space and then increment a1 yet again.

Back in the runProgram function we now want to look at the next section:

```c
  ptr = malloc(0x200uLL);
  for ( i = 0; i < v80; ++i )
    *((_QWORD *)ptr + i) = malloc(8uLL);
```

We malloc an original pointer and then look through and malloc again which means that this is probably a pointer to an array of pointers. 

We now move on to the first part of the while loop:

```c
  while ( 1 )
  {
    result = *(unsigned __int8 *)(v84 + a2);
    if ( (_BYTE)result == 13 )
      return result;
    v2 = v84++;
    v75 = *(_BYTE *)(v2 + a2);
```

From the looks of this program and the fact that a2 is the program data we can assume that v84 is most likely the instruction pointer. We get the current opcode at the ip and if it is 13 then we return that value, if not, we increment the ip and set it to v2, then get the next opcode and save it to v75.

We now see a massive switch statement that switches on v75:

```c
    switch ( v75 )
    {
      case 1:
        *((_QWORD *)&v74 + 1) = *(_QWORD *)&v73[8 * --v83 + 8];
        *(_QWORD *)&v74 = *(_QWORD *)&v73[8 * --v83 + 8];
        v3 = v83++;
        *(_QWORD *)&v73[8 * v3 + 8] = *((_QWORD *)&v74 + 1) - v74;
        break;
```

Since we know that v75 is the current opcode we can assume that the switch is for various different operations, so lets start with the first one since it looks fairly simple. We see that we decrement a value (v83) twice and use that to index from the array v73. We then increment v83 and then subtract the two values from each other and put them back into the array.

From this we can assume several things, the first is that this case is some sort of subtraction operation, the next thing we can assume is that v73 is the stack since we seem to be popping values off the stack and then pushing values back onto the stack. And the last thing we can assume is that v83 is the stack pointer. With this information we can start deciphering what the rest of the opcodes do.

For the sake of brevity I am not going to go super in depth into how to figure out each one since you know you have the return stack and the actual stack so the rest can be figured out quite easily, however, I will add a quick summary:

```c
ISUB        '\x01'
IMUL        '\x02'
IADD        '\x03' 
FADD        '\x04'
FSUB        '\x05'
ITOF        '\x06'
CCONST      '\x07'
ICONST      '\x08'
SCONST      '\x09'
FCONST      '\x0A'
STORE       '\x0B'
LOAD        '\x0C'
HALT        '\x0D'
RET         '\x0E'
PRINT       '\x0F'
POP         '\x10'
FUNC        '\x11'
FUNCINFO    '\x12'
IDIV        '\x13'
FDIV        '\x14'
CALL        '\x15'
INPUT       '\x16'
CMP         '\x17'
OR          '\x18'
AND         '\x19'
XOR         '\x1A'
NCMP        '\x1B'
LTCMP       '\x1C'
GTCMP       '\x1D'
JMP         '\x1E'
IJMP        '\x1F'
INC         '\x20'
DEC         '\x21'
MOV         '\x22'
MEMP        '\x23'
CMPJMP      '\x24'
```

```
ISUB
Pops the stack twice, subtracts 2nd item on stack from the 1st item on the stack, then pushes them back onto the stack.

IMUL
Pops the stack twice, multiplies the two values and pushes them onto the stack.

IDIV
Pops the stack twice, divides 2nd item on stack from the 1st item on the stack, then pushes them back onto the stack.

IADD
Pops the stack twice, adds the two values and pushes them onto the stack.

OR
Pops the stack twice, ors the two values and pushes them onto the stack.

AND
Pops the stack twice, ands the two values and pushes them onto the stack.

XOR
Pops the stack twice, xors the two values and pushes them onto the stack.

ICONST
Reads the next 8 bytes, converts them to long long, and pushes them onto the stack.

STORE
Reads the next 4 bytes, converts them to an int, then pops the stack and stores the value from the stack into the corresponding local variable.

LOAD
Reads the next 4 bytes, converts them to an int, then uses that int to get the local variable and pushes to the stack.

RET
Calls decrement function to get the last function on the call stack, sets ip to the address and current function to the function.

PRINT
Pops the stack and prints out the number.

POP
Pops a value from the stack and discards it.

CALL
Reads the next 4 bytes, converts to an int, and uses that to call the selected function. Sets the address of the return stack to the current ip.

INPUT
Reads in a long long and pushes to the stack.

CMP
Pops 2 values off the stack, and checks if they are equal to each other, if they are not equal then continue till we find a return.

NCMP
Pops 2 values off the stack, and checks if they are equal to each other, if they are equal then continue till we find a return.

LTCMP
Pops 2 values off the stack, and compares them to each other, if the 1st value is less than the 2nd value then continue till we find a return.

GTCMP
Pops 2 values off the stack, and compares them to each other, if the 1st value is greater than the 2nd value then continue till we find a return.

HALT
Ends the program.

JMP
Reads next 4 bytes, converts to int, and sets ip to that value.

IJMP
Pops stack and sets ip to that value.

INC
Reads next 4 bytes, converts to int, and increases the value of the opcode at that location.

DEC
Reads next 4 bytes, converts to int, and decreases the value of the opcode at that location.

MOV
Pops 2 values from the stack and sets the opcode at the 2nd location with the one from the 1st location.

MEMP
Reads next 4 bytes, then reads the opcode at that location and the next opcode, converts them to an integer, and pushes to the stack.

CMPJMP
Reads next 4 bytes, pops 2 values from the stack, if they are equal then set ip to the value of the number we read.
```

From this point I needed to make a disassembler, I decided to make a plugin for Binary Ninja to do this step since that way I would be able to have a nice view.

I first made a disassembly file named `recwrapper.py`:

```py
from binaryninja.function import RegisterInfo, InstructionInfo, InstructionTextToken
from binaryninja.enums import InstructionTextTokenType, BranchType
from binaryninjaui import UIContext

ISUB = int('01', 16)
IMUL = int('02', 16)
IADD = int('03', 16) 
FADD = int('04', 16)
FSUB = int('05', 16)
ITOF = int('06', 16)
CCONST = int('07', 16)
ICONST = int('08', 16)
SCONST = int('09', 16)
FCONST = int('0A', 16)
STORE = int('0B', 16)
LOAD = int('0C', 16)
HALT = int('0D', 16)
RET = int('0E', 16)
PRINT = int('0F', 16)
POP = int('10', 16)
FUNC = int('11', 16)
FUNCINFO = int('12', 16)
IDIV = int('13', 16)
FDIV = int('14', 16)
CALL = int('15', 16)
INPUT = int('16', 16)
CMP = int('17', 16)
OR = int('18', 16)
AND = int('19', 16)
XOR = int('1A', 16)
NCMP = int('1B', 16)
LTCMP = int('1C', 16)
GTCMP = int('1D', 16)
JMP = int('1E', 16)
IJMP = int('1F', 16)
INC = int('20', 16)
DEC = int('21', 16)
MOV = int('22', 16)
MEMP = int('23', 16)
CMPJMP = int('24', 16)

def bytesToInt(data, addr):
    val = 0
    for i in range(4):
        val += (data[addr + i] << (24 - (8 * i)))
    return val

def bytesToLongLong(data, addr):
    val = 0
    for i in range(8):
        val += (data[addr + i] << (56 - (8 * i)))
    return val

class Disassembler():

    def __init__(self):
        self.bv = None

    def getBV(self):
        if self.bv == None:
            ac = UIContext.activeContext()
            if ac != None:
                cv = ac.getCurrentViewFrame()
                if cv != None:
                    self.bv = cv.getCurrentBinaryView()
                    return True
                return False
            return False
        return True

    def disasm(self, data, addr):

        while not self.getBV():
            pass

        op = data[0]

        if op == ISUB:
            return ([ "ISUB"], 1)
        elif op == IMUL:
            return ([ "IMUL"], 1)
        elif op == IDIV:
            return ([ "IDIV"], 1)
        elif op == IADD:
            return ([ "IADD"], 1)
        elif op == ICONST:
            return ([ "ICONST",  " ",  str(bytesToLongLong(data, 1))], 9)
        elif op == STORE:
            return ([ "STORE",  " ",  str(bytesToInt(data, 1))], 5)
        elif op == LOAD:
            return ([ "LOAD",  " ",  str(bytesToInt(data, 1))], 5)
        elif op == HALT:
            return ([ "HALT"], 1)
        elif op == RET:
            return ([ "RET"], 1)
        elif op == PRINT:
            return ([ "PRINT"], 1)
        elif op == POP:
            return ([ "POP"], 1)
        elif op == CALL:
            return ([ "CALL",  " ",  str(bytesToInt(data, 1))], 5)
        elif op == INPUT:
            return ([ "INPUT"], 1)
        elif op == CMP:
            return ([ "CMP"], 1)
        elif op == NCMP:
            return ([ "NCMP"], 1)
        elif op == LTCMP:
            return ([ "LTCMP"], 1)
        elif op == GTCMP:
            return ([ "GTCMP"], 1)
        elif op == OR:
            return ([ "OR"], 1)
        elif op == AND:
            return ([ "AND"], 1)
        elif op == XOR:
            return ([ "XOR"], 1)
        elif op == JMP:
            return ([ "JMP",  " ",  str(bytesToInt(data, 1))], 5)
        elif op == IJMP:
            return ([ "IJMP"], 1)
        elif op == INC:
            return ([ "INC",  " ",  str(bytesToInt(data, 1))], 5)
        elif op == DEC:
            return ([ "DEC",  " ",  str(bytesToInt(data, 1))], 5)
        elif op == MOV:
            return ([ "MOV"], 1)
        elif op == MEMP:
            return ([ "MEMP",  " ",  str(bytesToInt(data, 1))], 5)
        elif op == CMPJMP:
            return ([ "CMPJMP",  " ",  str(bytesToInt(data, 1))], 5)

        return ([ "empty"], 1)
```

What this code does is it takes in a few bytes and figures out what instructions they are supposed to be. I then tied this into a `__init__.py` which contained a custom view so that I could see the code:

```py
from binaryninja.architecture import Architecture
from binaryninja.binaryview import *
from binaryninja.function import RegisterInfo, InstructionInfo, InstructionTextToken
from binaryninja.enums import InstructionTextTokenType, BranchType, SegmentFlag, SymbolType
from binaryninja.types import *
import recwrapper
from binaryninjaui import UIContext

use_default_loader_settings = True

disasm = recwrapper.Disassembler()

class Recurso(Architecture):
    name = 'Recurso'

    address_size = 4
    default_int_size = 8
    max_instr_length = 23

    stack_pointer = RegisterInfo("SP", 4)

    def get_instruction_info(self, data, addr):

        (instrTxt, instrLen) = disasm.disasm(data, addr)
        if instrLen == 0:
            return None
        result = InstructionInfo()

        print()

        for txt in instrTxt:
            if txt == "CMP" or txt == "NCMP" or txt == "GTCMP" or txt == "LTCMP":
                endLen = instrLen
                result.add_branch(BranchType.TrueBranch, endLen + addr)
                
                (instructs, length) = disasm.disasm(disasm.bv.read(endLen + addr, 23), addr)
                while (instructs[0] != "RET"):
                    endLen += length
                    (instructs, length) = disasm.disasm(disasm.bv.read(endLen + addr, 23), addr)
                    
                result.add_branch(BranchType.FalseBranch, endLen + addr + length)



            elif instrTxt[0] == "CALL" and txt.isdigit():
                result.add_branch(BranchType.CallDestination, disasm.bv.functions[int(txt)].start)
            elif txt == "RET" or txt == "HALT":
                result.add_branch(BranchType.FunctionReturn)

        result.length = instrLen
        return result 

    def get_instruction_text(self, data, addr):
        (instrTxt, instrLen) = disasm.disasm(data, addr)

        result = []

        for txt in instrTxt:
            if txt.isdigit() and (instrTxt[0] == "STORE" or instrTxt[0] == "LOAD"):
                result.append(InstructionTextToken(InstructionTextTokenType.RegisterToken, txt))
            elif txt.isdigit() and instrTxt[0] == "CALL" :
                result.append(InstructionTextToken(InstructionTextTokenType.CodeSymbolToken, disasm.bv.functions[int(txt)].name, disasm.bv.functions[int(txt)].start))
            elif txt.isdigit() and (instrTxt[0] == "JMP" or instrTxt[0] == "INC" or instrTxt[0] == "DEC"):
                result.append(InstructionTextToken(InstructionTextTokenType.IntegerToken, hex(int(txt) + disasm.bv.functions[0].start), int(txt) + disasm.bv.functions[0].start))
            elif txt.isdigit():
                result.append(InstructionTextToken(InstructionTextTokenType.IntegerToken, txt, int(txt)))
            else:
                result.append(InstructionTextToken(InstructionTextTokenType.TextToken, txt))

        return result, instrLen

    def get_instruction_low_level_il(self, data, addr, il):
            return None

# Define our view
class RecursoView(BinaryView):
    name = 'RecursoView'
    long_name = 'Recurso View'

    # check if this is an MSDOS file
    @classmethod
    def is_valid_for_data(cls,data):
        if data.file.original_filename.endswith(".recc"):
            return True
        return False

    # intialize the binary view
    def __init__(self, data):
        BinaryView.__init__(self, parent_view = data, file_metadata = data.file)
        self.platform = Architecture['Recurso'].standalone_platform
        self.data = data

    def defineFunctions(self, data, start, end):
        while start < end:
            op = data[start]
            curName = ""

            if op == b'\x11':
                start += 1
                op = data[start]

                while op != b'\x12':
                    curName += op.decode()
                    start += 1
                    op = data[start]

                start += 1

            localsCount = int.from_bytes(data.read(start, 4), "big")
            start += 4
            startAddress = int.from_bytes(data.read(start, 4), "big")
            start += 4

            self.add_function(startAddress + end)
            self.define_auto_symbol(Symbol(SymbolType.FunctionSymbol,startAddress + end, curName))

                

    # initialize our view
    def init(self):

        start = int.from_bytes(self.data.read(0, 4), "little") + 4

        self.add_auto_segment(4, start - 1, 4, start - 1, SegmentFlag.SegmentReadable|SegmentFlag.SegmentContainsData)
        self.add_auto_segment(start, len(self.data) - start, start, len(self.data) - start, SegmentFlag.SegmentReadable|SegmentFlag.SegmentExecutable|SegmentFlag.SegmentContainsCode)
        self.add_entry_point(start)

        self.defineFunctions(self.data, 4, start)

        return True
    
    def perform_is_executable(self):
        return True

Recurso.register()
RecursoView.register()
```

The view basically parsed out what components of the data were function data and what was opcodes, remember that the first 4 bytes tell us the length of the function data and then rest is opcodes. It then calls the diassembler and creates branches so that we can get a nicer view of the program. When we run the disassembler we get:

```
0000018e  int32_t main()

0000018e  0c00000000         LOAD 0
00000193  16                 INPUT
00000194  0b00000000         STORE 0
00000199  0c00000000         LOAD 0
0000019e  1500000013         CALL check
000001a3  0b00000001         STORE 1
000001a8  0c00000001         LOAD 1
000001ad  0f                 PRINT
000001ae  0d                 HALT


000001af  int32_t antidebug()

000001af  0b00000000         STORE 0
000001b4  0c00000000         LOAD 0
000001b9  0800000000000000…ICONST 0
000001c2  17                 CMP

000001c3  0800000000000000…ICONST 0
000001cc  0e                 RET

000001cd  0800000000000000…ICONST 1
000001d6  0c00000000         LOAD 0
000001db  01                 ISUB
000001dc  0b00000001         STORE 1
000001e1  0800000000000000…ICONST 1
000001ea  0c00000000         LOAD 0
000001ef  03                 IADD
000001f0  0b00000000         STORE 0
000001f5  0800000000000000…ICONST 1
000001fe  0c00000000         LOAD 0
00000203  01                 ISUB
00000204  0b00000000         STORE 0
00000209  0800000000000000…ICONST 1
00000212  0c00000000         LOAD 0
00000217  02                 IMUL
00000218  0b00000000         STORE 0
0000021d  0800000000000000…ICONST 1
00000226  0c00000000         LOAD 0
0000022b  13                 IDIV
0000022c  0b00000000         STORE 0
00000231  0800000000000000…ICONST 1
0000023a  0c00000000         LOAD 0
0000023f  19                 AND
00000240  0b00000000         STORE 0
00000245  0800000000000000…ICONST 1
0000024e  0c00000000         LOAD 0
00000253  18                 OR
00000254  0b00000000         STORE 0
00000259  0800000000000000…ICONST 0
00000262  0c00000000         LOAD 0
00000267  1a                 XOR
00000268  0b00000000         STORE 0
0000026d  0c00000001         LOAD 1
00000272  1500000001         CALL antidebug
00000277  0800000000000000…ICONST 0
00000280  0e                 RET


00000281  int32_t itsATwap()

00000281  0b00000000         STORE 0
00000286  0c00000000         LOAD 0
0000028b  080000000000000e…ICONST 3713
00000294  17                 CMP

00000295  0800000000000000…ICONST 1
0000029e  0e                 RET

0000029f  0c00000001         LOAD 1
000002a4  16                 INPUT
000002a5  0b00000001         STORE 1
000002aa  0c00000001         LOAD 1
000002af  1500000001         CALL antidebug
000002b4  0c00000000         LOAD 0
000002b9  1500000002         CALL itsATwap
000002be  0800000000000000…ICONST 0
000002c7  0e                 RET


000002c8  int32_t oddCheck()

000002c8  0b00000003         STORE 3
000002cd  0b00000002         STORE 2
000002d2  0b00000001         STORE 1
000002d7  0b00000000         STORE 0
000002dc  0c00000000         LOAD 0
000002e1  0800000000000075…ICONST 30001
000002ea  17                 CMP

000002eb  0c00000001         LOAD 1
000002f0  0800000000000067…ICONST 26419
000002f9  17                 CMP

000002fa  0c00000002         LOAD 2
000002ff  08000038645f3731…ICONST 62003745337707
00000308  17                 CMP

00000309  0c00000003         LOAD 3
0000030e  080000000000006d…ICONST 27955
00000317  17                 CMP

00000318  0800000000000000…ICONST 1
00000321  0e                 RET

00000322  0800000000000000…ICONST 0
0000032b  0e                 RET


0000032c  int32_t evenCheck()

0000032c  0b00000003         STORE 3
00000331  0b00000002         STORE 2
00000336  0b00000001         STORE 1
0000033b  0b00000000         STORE 0
00000340  0c00000000         LOAD 0
00000345  0800000000000064…ICONST 25695
0000034e  1b                 NCMP

0000034f  0c00000001         LOAD 1
00000354  0800000000375f67…ICONST 928999216
0000035d  1b                 NCMP

0000035e  0c00000002         LOAD 2
00000363  0800000000000033…ICONST 13151
0000036c  1b                 NCMP

0000036d  0c00000003         LOAD 3
00000372  0800000000000000…ICONST 125
0000037b  1b                 NCMP

0000037c  0800000000000000…ICONST 0
00000385  0e                 RET

00000386  0800000000000000…ICONST 1
0000038f  0e                 RET


00000390  int32_t finalCheck()

00000390  0c00000000         LOAD 0
00000395  0c00000001         LOAD 1
0000039a  0c00000002         LOAD 2
0000039f  0c00000003         LOAD 3
000003a4  0c00000004         LOAD 4
000003a9  0c00000005         LOAD 5
000003ae  0c00000006         LOAD 6
000003b3  0c00000007         LOAD 7
000003b8  16                 INPUT
000003b9  0b00000000         STORE 0
000003be  16                 INPUT
000003bf  0b00000001         STORE 1
000003c4  16                 INPUT
000003c5  0b00000002         STORE 2
000003ca  16                 INPUT
000003cb  0b00000003         STORE 3
000003d0  16                 INPUT
000003d1  0b00000004         STORE 4
000003d6  16                 INPUT
000003d7  0b00000005         STORE 5
000003dc  16                 INPUT
000003dd  0b00000006         STORE 6
000003e2  16                 INPUT
000003e3  0b00000007         STORE 7
000003e8  0c00000000         LOAD 0
000003ed  0c00000002         LOAD 2
000003f2  0c00000004         LOAD 4
000003f7  0c00000006         LOAD 6
000003fc  1500000003         CALL oddCheck
00000401  0b00000008         STORE 8
00000406  0c00000001         LOAD 1
0000040b  0c00000003         LOAD 3
00000410  0c00000005         LOAD 5
00000415  0c00000007         LOAD 7
0000041a  1500000004         CALL evenCheck
0000041f  0b00000009         STORE 9
00000424  0c00000009         LOAD 9
00000429  0c00000008         LOAD 8
0000042e  19                 AND
0000042f  0b0000000a         STORE 10
00000434  0c0000000a         LOAD 10
00000439  0e                 RET


0000043a  int32_t checkThemAll()

0000043a  0b00000005         STORE 5
0000043f  0b00000004         STORE 4
00000444  0b00000003         STORE 3
00000449  0b00000002         STORE 2
0000044e  0b00000001         STORE 1
00000453  0b00000000         STORE 0
00000458  0c00000000         LOAD 0
0000045d  08ffffffd42fb64c…ICONST 18446743885531466769
00000466  17                 CMP

00000467  0c00000001         LOAD 1
0000046c  08000009a2991485…ICONST 10593957610752
00000475  17                 CMP

00000476  0c00000002         LOAD 2
0000047b  080000001ba4e9bf…ICONST 118730899270
00000484  17                 CMP

00000485  0c00000003         LOAD 3
0000048a  0800000139813d75…ICONST 1346493052268
00000493  17                 CMP

00000494  0c00000004         LOAD 4
00000499  080000005f755f73…ICONST 409991082872
000004a2  17                 CMP

000004a3  0c00000005         LOAD 5
000004a8  0800000018002c00…ICONST 103082098739
000004b1  17                 CMP

000004b2  0800000000000000…ICONST 1
000004bb  0e                 RET

000004bc  0800000000000000…ICONST 0
000004c5  0e                 RET


000004c6  int32_t nextNextNextCheck()

000004c6  0c00000000         LOAD 0
000004cb  0c00000001         LOAD 1
000004d0  0c00000002         LOAD 2
000004d5  0c00000003         LOAD 3
000004da  0c00000004         LOAD 4
000004df  16                 INPUT
000004e0  0b00000000         STORE 0
000004e5  16                 INPUT
000004e6  0b00000001         STORE 1
000004eb  16                 INPUT
000004ec  0b00000002         STORE 2
000004f1  16                 INPUT
000004f2  0b00000003         STORE 3
000004f7  16                 INPUT
000004f8  0b00000004         STORE 4
000004fd  0c00000001         LOAD 1
00000502  0c00000000         LOAD 0
00000507  01                 ISUB
00000508  0b00000005         STORE 5
0000050d  0c00000004         LOAD 4
00000512  0c00000000         LOAD 0
00000517  02                 IMUL
00000518  0b00000006         STORE 6
0000051d  0c00000002         LOAD 2
00000522  0c00000005         LOAD 5
00000527  03                 IADD
00000528  0b00000007         STORE 7
0000052d  0c00000003         LOAD 3
00000532  0c00000002         LOAD 2
00000537  03                 IADD
00000538  0c00000001         LOAD 1
0000053d  03                 IADD
0000053e  0c00000000         LOAD 0
00000543  03                 IADD
00000544  0b00000008         STORE 8
00000549  0c00000004         LOAD 4
0000054e  0c00000003         LOAD 3
00000553  18                 OR
00000554  0b00000009         STORE 9
00000559  0c00000002         LOAD 2
0000055e  0c00000003         LOAD 3
00000563  01                 ISUB
00000564  0b0000000a         STORE 10
00000569  0c00000005         LOAD 5
0000056e  0c00000006         LOAD 6
00000573  0c00000007         LOAD 7
00000578  0c00000008         LOAD 8
0000057d  0c00000009         LOAD 9
00000582  0c0000000a         LOAD 10
00000587  1500000006         CALL checkThemAll
0000058c  0b0000000b         STORE 11
00000591  1500000005         CALL finalCheck
00000596  0b0000000c         STORE 12
0000059b  0c0000000c         LOAD 12
000005a0  0c0000000b         LOAD 11
000005a5  19                 AND
000005a6  0b0000000d         STORE 13
000005ab  0c0000000d         LOAD 13
000005b0  0e                 RET


000005b1  int32_t checkity()

000005b1  0b00000004         STORE 4
000005b6  0b00000003         STORE 3
000005bb  0b00000002         STORE 2
000005c0  0b00000001         STORE 1
000005c5  0b00000000         STORE 0
000005ca  0c00000000         LOAD 0
000005cf  0800000071e2fcfb…ICONST 489139534831
000005d8  17                 CMP

000005d9  0c00000001         LOAD 1
000005de  0800000000000000…ICONST 1
000005e7  17                 CMP

000005e8  0c00000002         LOAD 2
000005ed  0800000000000000…ICONST 2
000005f6  17                 CMP

000005f7  0c00000003         LOAD 3
000005fc  0800000000000000…ICONST 3
00000605  17                 CMP

00000606  0c00000004         LOAD 4
0000060b  0800000000000000…ICONST 4
00000614  17                 CMP

00000615  0800000000000000…ICONST 1
0000061e  0e                 RET

0000061f  080000000000001c…ICONST 7331
00000628  1500000001         CALL antidebug
0000062d  0800000000000000…ICONST 0
00000636  0e                 RET


00000637  int32_t midThree()

00000637  0b00000000         STORE 0
0000063c  0c00000000         LOAD 0
00000641  080000000035305f…ICONST 892362496
0000064a  1b                 NCMP

0000064b  0800000000000000…ICONST 0
00000654  0e                 RET

00000655  0800000000000000…ICONST 1
0000065e  0e                 RET


0000065f  int32_t topAndBottom()

0000065f  0b00000000         STORE 0
00000664  0c00000000         LOAD 0
00000669  0800000000000000…ICONST 7
00000672  17                 CMP

00000673  0800000000000000…ICONST 1
0000067c  0e                 RET

0000067d  0800000000000000…ICONST 0
00000686  0e                 RET


00000687  int32_t checkTheBigs()

00000687  0b00000001         STORE 1
0000068c  0b00000000         STORE 0
00000691  0c00000000         LOAD 0
00000696  0c00000001         LOAD 1
0000069b  1d                 GTCMP

0000069c  0c00000001         LOAD 1
000006a1  0c00000001         LOAD 1
000006a6  1b                 NCMP

000006a7  0800000000000000…ICONST 0
000006b0  0e                 RET

000006b1  0c00000000         LOAD 0
000006b6  0c00000001         LOAD 1
000006bb  01                 ISUB
000006bc  0b00000002         STORE 2
000006c1  0c00000002         LOAD 2
000006c6  0800000000000000…ICONST 1
000006cf  0800000000000000…ICONST 2
000006d8  0800000000000000…ICONST 3
000006e1  0800000000000000…ICONST 4
000006ea  1500000008         CALL checkity
000006ef  0b00000003         STORE 3
000006f4  0800000000ffffff…ICONST 4294967040
000006fd  0c00000001         LOAD 1
00000702  19                 AND
00000703  0b00000004         STORE 4
00000708  0c00000004         LOAD 4
0000070d  1500000009         CALL midThree
00000712  0b00000005         STORE 5
00000717  0800000000000000…ICONST 255
00000720  0c00000000         LOAD 0
00000725  19                 AND
00000726  0b00000006         STORE 6
0000072b  08000000ff000000…ICONST 1095216660480
00000734  0c00000001         LOAD 1
00000739  19                 AND
0000073a  0b00000007         STORE 7
0000073f  0c00000007         LOAD 7
00000744  0c00000006         LOAD 6
00000749  1a                 XOR
0000074a  0b00000008         STORE 8
0000074f  0800000000000000…ICONST 1
00000758  0b00000009         STORE 9
0000075d  1500000007         CALL nextNextNextCheck
00000762  0b0000000a         STORE 10
00000767  0c0000000a         LOAD 10
0000076c  0c00000009         LOAD 9
00000771  19                 AND
00000772  0c00000005         LOAD 5
00000777  19                 AND
00000778  0c00000003         LOAD 3
0000077d  19                 AND
0000077e  0b0000000b         STORE 11
00000783  0c0000000b         LOAD 11
00000788  0e                 RET


00000789  int32_t isItOr()

00000789  0b00000000         STORE 0
0000078e  0c00000000         LOAD 0
00000793  0800000000000000…ICONST 99
0000079c  17                 CMP

0000079d  0800000000000000…ICONST 1
000007a6  0e                 RET

000007a7  0800000000000000…ICONST 0
000007b0  0e                 RET


000007b1  int32_t isItXor()

000007b1  0b00000000         STORE 0
000007b6  0c00000000         LOAD 0
000007bb  0800000000000000…ICONST 28
000007c4  1b                 NCMP

000007c5  0800000000000000…ICONST 0
000007ce  0e                 RET

000007cf  0800000000000000…ICONST 1
000007d8  0e                 RET


000007d9  int32_t keepGoing()

000007d9  080000000000001c…ICONST 7331
000007e2  1500000001         CALL antidebug
000007e7  0c00000000         LOAD 0
000007ec  0c00000001         LOAD 1
000007f1  16                 INPUT
000007f2  0b00000000         STORE 0
000007f7  16                 INPUT
000007f8  0b00000001         STORE 1
000007fd  0c00000001         LOAD 1
00000802  0c00000000         LOAD 0
00000807  1a                 XOR
00000808  0b00000002         STORE 2
0000080d  0c00000002         LOAD 2
00000812  150000000d         CALL isItXor
00000817  0b00000003         STORE 3
0000081c  0c00000001         LOAD 1
00000821  0c00000000         LOAD 0
00000826  19                 AND
00000827  0b00000002         STORE 2
0000082c  0c00000002         LOAD 2
00000831  150000000c         CALL isItOr
00000836  0b00000004         STORE 4
0000083b  0c00000005         LOAD 5
00000840  16                 INPUT
00000841  0b00000005         STORE 5
00000846  0c00000005         LOAD 5
0000084b  1500000002         CALL itsATwap
00000850  0c00000006         LOAD 6
00000855  0c00000007         LOAD 7
0000085a  16                 INPUT
0000085b  0b00000006         STORE 6
00000860  16                 INPUT
00000861  0b00000007         STORE 7
00000866  0c00000006         LOAD 6
0000086b  0c00000007         LOAD 7
00000870  150000000b         CALL checkTheBigs
00000875  0b00000008         STORE 8
0000087a  0c00000008         LOAD 8
0000087f  0c00000004         LOAD 4
00000884  19                 AND
00000885  0c00000003         LOAD 3
0000088a  19                 AND
0000088b  0b00000009         STORE 9
00000890  0c00000009         LOAD 9
00000895  0e                 RET


00000896  int32_t wowzaACheck2()

00000896  0b00000000         STORE 0
0000089b  0c00000000         LOAD 0
000008a0  0800000000000069…ICONST 26960
000008a9  1d                 GTCMP

000008aa  0800000000000000…ICONST 1
000008b3  0e                 RET

000008b4  0800000000000000…ICONST 0
000008bd  0e                 RET



000008be  int32_t wowzaACheck()

000008be  0b00000000         STORE 0
000008c3  0c00000000         LOAD 0
000008c8  080000000000041d…ICONST 269700
000008d1  1c                 LTCMP

000008d2  0800000000000000…ICONST 1
000008db  0e                 RET

000008dc  0800000000000000…ICONST 0
000008e5  0e                 RET


000008e6  int32_t yetAnotherCheck()

000008e6  0b00000000         STORE 0
000008eb  0c00000000         LOAD 0
000008f0  080000000000006d…ICONST 28025
000008f9  1b                 NCMP

000008fa  0800000000000000…ICONST 0
00000903  0e                 RET

00000904  0800000000000000…ICONST 1
0000090d  0e                 RET


0000090e  int32_t anotherCheck()

0000090e  0b00000000         STORE 0
00000913  0c00000000         LOAD 0
00000918  0800000000000004…ICONST 1057
00000921  17                 CMP

00000922  0800000000000000…ICONST 1
0000092b  0e                 RET

0000092c  0800000000000000…ICONST 0
00000935  0e                 RET


00000936  int32_t check()

00000936  0b00000000         STORE 0
0000093b  0c00000000         LOAD 0
00000940  0800000000000000…ICONST 102
00000949  1b                 NCMP

0000094a  0800000000000000…ICONST 0
00000953  0e                 RET

00000954  0c00000000         LOAD 0
00000959  16                 INPUT
0000095a  0b00000000         STORE 0
0000095f  0800000000000005…ICONST 1337
00000968  0c00000000         LOAD 0
0000096d  19                 AND
0000096e  0b00000002         STORE 2
00000973  0c00000002         LOAD 2
00000978  1500000012         CALL anotherCheck
0000097d  0b00000003         STORE 3
00000982  0800000000000005…ICONST 1337
0000098b  0c00000000         LOAD 0
00000990  18                 OR
00000991  0b00000004         STORE 4
00000996  0c00000004         LOAD 4
0000099b  1500000011         CALL yetAnotherCheck
000009a0  0b00000005         STORE 5
000009a5  0800000000000005…ICONST 1337
000009ae  0c00000000         LOAD 0
000009b3  1a                 XOR
000009b4  0b00000006         STORE 6
000009b9  0c00000006         LOAD 6
000009be  1500000010         CALL wowzaACheck
000009c3  0b00000007         STORE 7
000009c8  0c00000006         LOAD 6
000009cd  1500000010         CALL wowzaACheck
000009d2  0b00000008         STORE 8
000009d7  150000000e         CALL keepGoing
000009dc  0b00000009         STORE 9
000009e1  0c00000009         LOAD 9
000009e6  0c00000008         LOAD 8
000009eb  19                 AND
000009ec  0c00000007         LOAD 7
000009f1  19                 AND
000009f2  0c00000005         LOAD 5
000009f7  19                 AND
000009f8  0c00000003         LOAD 3
000009fd  19                 AND
000009fe  0b0000000a         STORE 10
00000a03  0c0000000a         LOAD 10
00000a08  0e                 RET
```

Luckily Recurso bytecode translate pretty easily straight into its high level representation so from here we can simply turn it back into its source code:

```c
int antidebug(int a)
| 0, 0 |
{
    int b = a - 1;

    a = a + 1;
    a = a - 1;
    a = a * 1;
    a = a / 1;
    a = a & 1;
    a = a | 1;
    a = a ^ 0;

    antidebug(b);
    return 0;
}

int itsATwap(int a)
| 3713, 1 |
{
    int b;

    input b;

    antidebug(b);

    itsATwap(a);

    return 0;
}

int oddCheck(int a, int b, int c, int d)
| 30001, 26419, 62003745337707, 27955, 1 |
{
    return 0;
}

int evenCheck(int a, int b, int c, int d)
| !25695, !928999216, !13151, !125, 0 |
{
    return 1;
}

int finalCheck()
{
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;

    input a;
    input b;
    input c;
    input d;
    input e;
    input f;
    input g;
    input h;

    int resp = oddCheck(a,c,e,g);
    int resp2 = evenCheck(b,d,f,h);

    int res = resp & resp2;

    return res;
}

int checkThemAll(int a, int b, int c, int d, int e, int f)
| -188178084847, 10593957610752, 118730899270, 1346493052268, 409991082872, 103082098739, 1 |
{
    return 0;
}

int nextNextNextCheck()
{
    int a;
    int b;
    int c;
    int d;
    int e;

    input a;
    input b;
    input c;
    input d;
    input e;

    int f = a - b;
    int g = a * e;
    int h = f + c;
    int i = a + b + c + d;
    int j = d | e;
    int k = d - c;

    int resp = checkThemAll(f,g,h,i,j,k);

    int resp2 = finalCheck();

    int res = resp & resp2;

    return res;

}

int checkity(int a, int b, int c, int d, int e)
| 489139534831, 1, 2, 3, 4, 1 |
{
    antidebug(7331);
    return 0;
}

int midThree(int a)
| !892362496, 0 |
{
    return 1;
}

int topAndBottom(int a)
| 7, 1 |
{
    return 0;
}

int checkTheBigs(int a, int b)
| >b, !b, 0 |
{

    int c = b - a;

    int resp = checkity(c, 1, 2, 3, 4);

    int d = b & 4294967040;

    int resp2 = midThree(d);

    int e = a & 255;

    int f = b & 1095216660480;

    int g = e ^ f;

    int resp3 = 1;

    int resp4 = nextNextNextCheck();

    int res = resp & resp2 & resp3 & resp4;

    return res;
}

int isItOr(int a)
| 99, 1 |
{
    return 0;
}

int isItXor(int a)
| !28, 0 |
{
    return 1;
}

int keepGoing()
{
    antidebug(7331);

    int a;

    int b;

    input a;

    input b;

    int c = a ^ b;

    int resp = isItXor(c);

    c = a & b;

    int resp2 = isItOr(c);

    int twap;

    input twap;

    itsATwap(twap);

    int d;

    int e;

    input d;

    input e;

    int resp3 = checkTheBigs(d, e);

    int res = resp & resp2 & resp3;

    return res;
}

int wowzaACheck2(int a)
| >26960, 1 |
{
    return 0;
}

int wowzaACheck(int a)
| <269700, 1 |
{
    return 0;
}

int yetAnotherCheck(int a)
| !28025, 0 |
{
    return 1;
}

int anotherCheck(int a)
| 1057, 1 |
{
    return 0;
}

int check(int a)
| !102, 0 |
{
    int a;

    input a;

    int b = a & 1337;

    int resp = anotherCheck(b);

    int c = a | 1337;

    int resp2 = yetAnotherCheck(c);

    int d = a ^ 1337;

    int resp3 = wowzaACheck(d);

    int resp4 = wowzaACheck(d);

    int resp5 = keepGoing();

    int res = resp & resp2 & resp3 & resp4 & resp5;

    return res;
}

int a;

input a;

int b = check(a);

print(b);
```

From here we can see that we have a bunch of conditions which means that we can simply write a z3py script.

pure constriants (no guessing):

```py
from z3 import *

maina = BitVec('maina', 64)

checka = BitVec('checka', 64)

keepgoinga = BitVec('keepgoinga', 64)
keepgoingb = BitVec('keepgoingb', 64)

checkthebigsa = BitVec('checkthebigsa', 40)
checkthebigsb = BitVec('checkthebigsb', 40)

nextNextNextChecka = BitVec('nextNextNextChecka', 64)
nextNextNextCheckb = BitVec('nextNextNextCheckb', 64)
nextNextNextCheckc = BitVec('nextNextNextCheckc', 64)
nextNextNextCheckd = BitVec('nextNextNextCheckd', 64)
nextNextNextChecke = BitVec('nextNextNextChecke', 64)

finalChecka = BitVec('finalChecka', 64)
finalCheckb = BitVec('finalCheckb', 64)
finalCheckc = BitVec('finalCheckc', 64)
finalCheckd = BitVec('finalCheckd', 64)
finalChecke = BitVec('finalChecke', 64)
finalCheckf = BitVec('finalCheckf', 64)
finalCheckg = BitVec('finalCheckg', 64)
finalCheckh = BitVec('finalCheckh', 64)

vals = [maina, checka, keepgoinga, keepgoingb, checkthebigsa, checkthebigsb, nextNextNextChecka, nextNextNextCheckb,
	nextNextNextCheckc, nextNextNextCheckd, nextNextNextChecke, finalChecka, finalCheckb, finalCheckc,
	finalCheckd, finalChecke, finalCheckf, finalCheckg, finalCheckh]

s = Solver()

s.add(maina == 102)

s.add(checka & 1337 == 1057)
s.add(checka | 1337 == 28025)
s.add(checka ^ 1337 < 269700)

s.add(keepgoinga ^ keepgoingb == 28)
s.add(keepgoinga & keepgoingb == 99)

s.add(checkthebigsb - checkthebigsa == 489139534831)
s.add(checkthebigsb & 4294967040 == 892362496)
s.add(checkthebigsb < checkthebigsa)

s.add(nextNextNextChecka - nextNextNextCheckb == -188178084847)
s.add(nextNextNextChecka * nextNextNextChecke == 10593957610752)
s.add((nextNextNextChecka - nextNextNextCheckb) + nextNextNextCheckc == 118730899270)
s.add(nextNextNextChecka + nextNextNextCheckb + nextNextNextCheckc + nextNextNextCheckd == 1346493052268)
s.add(nextNextNextCheckd | nextNextNextChecke == 409991082872)
s.add(nextNextNextCheckd - nextNextNextCheckc == 103082098739)

s.add(finalChecka == 30001)
s.add(finalCheckb == 25695)
s.add(finalCheckc == 26419)
s.add(finalCheckd == 928999216)
s.add(finalChecke == 62003745337707)
s.add(finalCheckf == 13151)
s.add(finalCheckg == 27955)
s.add(finalCheckh == 125)


s.check()
m = s.model()

print(m)
total_str = ""
for i in vals:
	hex_str = hex(int(str(m[i])))[2:].replace("00", '')
	cur_str = ""
	for i in range(0, len(hex_str), 2):
		try:
			cur_str += bytes.fromhex(hex_str[i:i+2]).decode("ASCII")
		except:
			cur_str += "."
        print(cur_str)
	total_str += cur_str

print(total_str)
```

Which gives you:

```
f
la
c

`R3c%
.50_
3c0mp
_3z!_
Gu3s5
_u_sh
0
u1
d_
g3
7_g0
8d_71k
3_
m3
}
flac`R3c%.50_3c0mp_3z!_Gu3s5_u_sh0u1d_g37_g08d_71k3_m3}
```
R3c..50 makes it fairly obvious that it is Recurso in some form. and that we don't need that initial value in front so it is 4 bytes.

and you know it should be flag{ so you can add some more constraints:

```py
from z3 import *

maina = BitVec('maina', 64)

checka = BitVec('checka', 64)

keepgoinga = BitVec('keepgoinga', 64)
keepgoingb = BitVec('keepgoingb', 64)

checkthebigsa = BitVec('checkthebigsa', 40)
checkthebigsb = BitVec('checkthebigsb', 40)

nextNextNextChecka = BitVec('nextNextNextChecka', 64)
nextNextNextCheckb = BitVec('nextNextNextCheckb', 64)
nextNextNextCheckc = BitVec('nextNextNextCheckc', 64)
nextNextNextCheckd = BitVec('nextNextNextCheckd', 64)
nextNextNextChecke = BitVec('nextNextNextChecke', 64)

finalChecka = BitVec('finalChecka', 64)
finalCheckb = BitVec('finalCheckb', 64)
finalCheckc = BitVec('finalCheckc', 64)
finalCheckd = BitVec('finalCheckd', 64)
finalChecke = BitVec('finalChecke', 64)
finalCheckf = BitVec('finalCheckf', 64)
finalCheckg = BitVec('finalCheckg', 64)
finalCheckh = BitVec('finalCheckh', 64)

vals = [maina, checka, keepgoinga, keepgoingb, checkthebigsa, checkthebigsb, nextNextNextChecka, nextNextNextCheckb,
	nextNextNextCheckc, nextNextNextCheckd, nextNextNextChecke, finalChecka, finalCheckb, finalCheckc,
	finalCheckd, finalChecke, finalCheckf, finalCheckg, finalCheckh]

s = Solver()

s.add(maina == 102)

s.add(checka & 1337 == 1057)
s.add(checka | 1337 == 28025)
s.add(checka ^ 1337 < 269700)

s.add(keepgoinga ^ keepgoingb == 28)
s.add(keepgoinga & keepgoingb == 99)
s.add(keepgoinga == 0x67)
s.add(keepgoingb == 0x7b)

s.add(checkthebigsb - checkthebigsa == 489139534831)
s.add(checkthebigsb & 4294967040 == 892362496)
s.add(checkthebigsb < checkthebigsa)
s.add((checkthebigsa & 0xFFFFFF00)  == 0x52336300)
s.add(Or(checkthebigsa & 0xFF == ord('u'), checkthebigsa & 0xFF == ord('U')))

s.add(nextNextNextChecka - nextNextNextCheckb == -188178084847)
s.add(nextNextNextChecka * nextNextNextChecke == 10593957610752)
s.add((nextNextNextChecka - nextNextNextCheckb) + nextNextNextCheckc == 118730899270)
s.add(nextNextNextChecka + nextNextNextCheckb + nextNextNextCheckc + nextNextNextCheckd == 1346493052268)
s.add(nextNextNextCheckd | nextNextNextChecke == 409991082872)
s.add(nextNextNextCheckd - nextNextNextCheckc == 103082098739)

s.add(finalChecka == 30001)
s.add(finalCheckb == 25695)
s.add(finalCheckc == 26419)
s.add(finalCheckd == 928999216)
s.add(finalChecke == 62003745337707)
s.add(finalCheckf == 13151)
s.add(finalCheckg == 27955)
s.add(finalCheckh == 125)


s.check()
m = s.model()

print(m)
total_str = ""
for i in vals:
	hex_str = hex(int(str(m[i])))[2:].replace("00", '')
	cur_str = ""
	for i in range(0, len(hex_str), 2):
		try:
			cur_str += bytes.fromhex(hex_str[i:i+2]).decode("ASCII")
		except:
			cur_str += "."
	print(cur_str)
	total_str += cur_str

print(total_str)
```

And with that you get:

```
f
la
g
{
~R3cU
.50_D
.3c0mp
._3z!_
Gu3s5
_u_sh
0
u1
d_
g3
7_g0
8d_71k
3_
m3
}
flag{~R3cU.50_D.3c0mp._3z!_Gu3s5_u_sh0u1d_g37_g08d_71k3_m3}
```

You know you may have sizes slightly off but if you constrain to only allow sizes you know for sure you end up with:

```
flag{R3cU.50_.3c0mp._3z!_Gu3s5_u_sh0u1d_g37_g08d_71k3_m3}
```

However, based on the challenge description you can see that they are either r/R or d/D for recurso and decompilation. Making the flag:

`flag{R3cUr50_D3c0mp_3z!_Gu3s5_u_sh0u1d_g37_g08d_71k3_m3}`