# Zer0pts Syscall 777 Write Up

## Details:

Jeopardy style CTF

Category: Reversing

## Write up:

This challenge had a 64 bit binary that when run would ask for a flag before printing out wrong if it was incorrect. Pulling the file up in a decompiler I saw the below main function:

```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  __int64 v3; // rcx
  unsigned int v4; // er12
  int *v5; // rdi
  __int64 i; // rbx
  int v8[14]; // [rsp+0h] [rbp-68h] BYREF
  unsigned __int64 v9; // [rsp+38h] [rbp-30h]

  v3 = 14LL;
  v4 = 1;
  v9 = __readfsqword(0x28u);
  v5 = v8;
  while ( v3 )
  {
    *v5++ = 0;
    --v3;
  }
  __printf_chk(1LL, "FLAG: ", a3);
  if ( (unsigned int)__isoc99_scanf("%55s", v8) == 1 )
  {
    for ( i = 1LL; i != 15; ++i )
    {
      syscall(
        777LL,
        (unsigned int)v8[i - 1],
        (unsigned int)v8[(int)i % 14],
        (unsigned int)v8[((int)i + 1) % 14],
        (unsigned int)v8[((int)i + 2) % 14]);
      v4 = *__errno_location();
      if ( v4 == 1 )
      {
        puts("Wrong...");
        return v4;
      }
    }
    v4 = 0;
    puts("Correct!");
  }
  return v4;
}
```

The program was using the system call to check the flag, however syscall 777 was not a valid call. I then tried an strace and saw something interesting:

```
strace ./chall      

execve("./chall", ["./chall"], 0x7ffe48ddcbd0 /* 48 vars */) = 0
brk(NULL)                               = 0x55cd8c957000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=84692, ...}) = 0
mmap(NULL, 84692, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fd7bc273000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0@n\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=1839792, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fd7bc271000
mmap(NULL, 1852680, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fd7bc0ac000
mprotect(0x7fd7bc0d1000, 1662976, PROT_NONE) = 0
mmap(0x7fd7bc0d1000, 1355776, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x25000) = 0x7fd7bc0d1000
mmap(0x7fd7bc21c000, 303104, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x170000) = 0x7fd7bc21c000
mmap(0x7fd7bc267000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1ba000) = 0x7fd7bc267000
mmap(0x7fd7bc26d000, 13576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fd7bc26d000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7fd7bc272580) = 0
mprotect(0x7fd7bc267000, 12288, PROT_READ) = 0
mprotect(0x55cd8c82c000, 4096, PROT_READ) = 0
mprotect(0x7fd7bc2b2000, 4096, PROT_READ) = 0
munmap(0x7fd7bc273000, 84692)           = 0
prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)  = 0
prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, {len=205, filter=0x7ffd4924fb10}) = 0
fstat(1, {st_mode=S_IFCHR|0600, st_rdev=makedev(0x88, 0), ...}) = 0
brk(NULL)                               = 0x55cd8c957000
brk(0x55cd8c978000)                     = 0x55cd8c978000
fstat(0, {st_mode=S_IFCHR|0600, st_rdev=makedev(0x88, 0), ...}) = 0
write(1, "FLAG: ", 6FLAG: )                   = 6
read(0, 
```

We were calling something that was setting a filter value. Looking through the docs for prctl and SECCOMP I found out that this did indeed set what syscall's were or were not accepted and that the filter = pointed to the sock_filter structure. Upon more googling we realized that the structure was:

```c
struct sock_filter {	/* Filter block */
	__u16	code;   /* Actual filter code */
	__u8	jt;	/* Jump true */
	__u8	jf;	/* Jump false */
	__u32	k;      /* Generic multiuse field */
};
```

From here we then decided to do dynamic analysis, I found the prctl function and set a breakpoint, once the filter was set I went to the memory the pointer pointed at and copied the relevent bytes:

```
20 00 00 00 00 00 00 00  15 00 00 C8 09 03 00 00
20 00 00 00 10 00 00 00  54 00 00 00 FF 00 00 00
35 00 C7 00 80 00 00 00  20 00 00 00 10 00 00 00
74 00 00 00 08 00 00 00  54 00 00 00 FF 00 00 00
35 00 C3 00 80 00 00 00  20 00 00 00 10 00 00 00
74 00 00 00 10 00 00 00  54 00 00 00 FF 00 00 00
35 00 BF 00 80 00 00 00  20 00 00 00 10 00 00 00
74 00 00 00 18 00 00 00  54 00 00 00 FF 00 00 00
35 00 BB 00 80 00 00 00  20 00 00 00 10 00 00 00
02 00 00 00 00 00 00 00  20 00 00 00 18 00 00 00
61 00 00 00 00 00 00 00  AC 00 00 00 00 00 00 00
02 00 00 00 01 00 00 00  20 00 00 00 20 00 00 00
61 00 00 00 01 00 00 00  AC 00 00 00 00 00 00 00
02 00 00 00 02 00 00 00  20 00 00 00 28 00 00 00
61 00 00 00 02 00 00 00  AC 00 00 00 00 00 00 00
02 00 00 00 03 00 00 00  60 00 00 00 00 00 00 00
61 00 00 00 01 00 00 00  0C 00 00 00 00 00 00 00
61 00 00 00 02 00 00 00  0C 00 00 00 00 00 00 00
61 00 00 00 03 00 00 00  0C 00 00 00 00 00 00 00
02 00 00 00 04 00 00 00  60 00 00 00 00 00 00 00
61 00 00 00 01 00 00 00  1C 00 00 00 00 00 00 00
61 00 00 00 02 00 00 00  0C 00 00 00 00 00 00 00
61 00 00 00 03 00 00 00  1C 00 00 00 00 00 00 00
02 00 00 00 05 00 00 00  60 00 00 00 00 00 00 00
61 00 00 00 01 00 00 00  0C 00 00 00 00 00 00 00
61 00 00 00 02 00 00 00  1C 00 00 00 00 00 00 00
61 00 00 00 03 00 00 00  1C 00 00 00 00 00 00 00
02 00 00 00 06 00 00 00  60 00 00 00 00 00 00 00
61 00 00 00 01 00 00 00  1C 00 00 00 00 00 00 00
61 00 00 00 02 00 00 00  1C 00 00 00 00 00 00 00
61 00 00 00 03 00 00 00  0C 00 00 00 00 00 00 00
02 00 00 00 07 00 00 00  60 00 00 00 04 00 00 00
61 00 00 00 05 00 00 00  4C 00 00 00 00 00 00 00
02 00 00 00 08 00 00 00  60 00 00 00 06 00 00 00
61 00 00 00 07 00 00 00  5C 00 00 00 00 00 00 00
61 00 00 00 08 00 00 00  AC 00 00 00 00 00 00 00
02 00 00 00 08 00 00 00  60 00 00 00 05 00 00 00
61 00 00 00 06 00 00 00  4C 00 00 00 00 00 00 00
02 00 00 00 09 00 00 00  60 00 00 00 07 00 00 00
61 00 00 00 04 00 00 00  5C 00 00 00 00 00 00 00
61 00 00 00 09 00 00 00  AC 00 00 00 00 00 00 00
02 00 00 00 09 00 00 00  60 00 00 00 06 00 00 00
61 00 00 00 07 00 00 00  4C 00 00 00 00 00 00 00
02 00 00 00 0A 00 00 00  60 00 00 00 04 00 00 00
61 00 00 00 05 00 00 00  5C 00 00 00 00 00 00 00
61 00 00 00 0A 00 00 00  AC 00 00 00 00 00 00 00
02 00 00 00 0A 00 00 00  60 00 00 00 07 00 00 00
61 00 00 00 04 00 00 00  4C 00 00 00 00 00 00 00
02 00 00 00 0B 00 00 00  60 00 00 00 05 00 00 00
61 00 00 00 06 00 00 00  5C 00 00 00 00 00 00 00
61 00 00 00 0B 00 00 00  AC 00 00 00 00 00 00 00
02 00 00 00 0B 00 00 00  60 00 00 00 08 00 00 00
```

My teammate then wrote the following go program to parse the bytes and using the built in BPF library in go to convert the bytes to the BPF assembly.

```go
package main;
import (
  "bufio"
  "encoding/binary"
  "encoding/hex"
  "encoding/json"
  "fmt"
  "golang.org/x/net/bpf"
  "os"
  "regexp"
)

func main() {

  arg := 1;
  file, _ := os.Open("./bpf.txt")
  raw := getInstructions(file);
  
  instructions, allDecoded := bpf.Disassemble(raw);
  
  for i := range instructions {
    if arg == 0 {
      xType := fmt.Sprintf("%T", instructions[i])
      print(xType);
      print(" | ");
      j, _ := json.Marshal(instructions[i]);
      fmt.Printf("%s\n", string(j));
    } else {
      fmt.Printf("%+v\n", instructions[i]);
    }
  }
  print("---------\n");
  print(allDecoded);
  print("\n")

}

func getInstructions(file *os.File) []bpf.RawInstruction {
  instructions := make([]bpf.RawInstruction, 0)
  scanner := bufio.NewScanner(file)
  for scanner.Scan() {
    line := scanner.Text()
    bytes := regexp.MustCompile("[^\\s]+").FindAllString(line, -1)
    instructions = append(instructions, parseInstruction(bytes[0:8]))
    instructions = append(instructions, parseInstruction(bytes[8:]))
  }

  return instructions
}

func parseInstruction(strings []string) bpf.RawInstruction {
  fmt.Println(strings)
  opStr := strings[1] + strings[0]
  opBytes, _ := hex.DecodeString(opStr)
  op := binary.BigEndian.Uint16(opBytes)

  jtStr := strings[3]
  jtBytes, _ := hex.DecodeString(jtStr)

  jfStr := strings[4]
  jfBytes, _ := hex.DecodeString(jfStr)

  kStr := strings[7] + strings[6] + strings[5] + strings[4]
  kBytes, _ := hex.DecodeString(kStr)
  k := binary.BigEndian.Uint32(kBytes)

  instruction := bpf.RawInstruction{Op: op, Jt: jtBytes[0], Jf: jfBytes[0], K: k}
  fmt.Printf("%+v\n", instruction)
  return instruction
}
```

When run the output was:

```
[20 00 00 00 00 00 00 00]
{Op:32 Jt:0 Jf:0 K:0}
...
ld [0]
jeq #777,200,9
ld [16]
and #255
jlt #128,128
ld [16]
rsh #8
and #255
jlt #128,128
ld [16]
rsh #16
and #255
jlt #128,128
ld [16]
rsh #24
and #255
jlt #128,128
ld [16]
st M[0]
ld [24]
ldx M[0]
xor x
st M[1]
ld [32]
ldx M[1]
xor x
st M[2]
ld [40]
ldx M[2]
xor x
st M[3]
ld M[0]
ldx M[1]
add x
ldx M[2]
add x
ldx M[3]
add x
st M[4]
ld M[0]
ldx M[1]
sub x
ldx M[2]
add x
ldx M[3]
sub x
st M[5]
ld M[0]
ldx M[1]
add x
ldx M[2]
sub x
ldx M[3]
sub x
st M[6]
ld M[0]
ldx M[1]
sub x
ldx M[2]
sub x
ldx M[3]
add x
st M[7]
ld M[4]
ldx M[5]
or x
st M[8]
ld M[6]
ldx M[7]
and x
ldx M[8]
xor x
st M[8]
ld M[5]
ldx M[6]
or x
st M[9]
ld M[7]
ldx M[4]
and x
ldx M[9]
xor x
st M[9]
ld M[6]
ldx M[7]
or x
st M[10]
ld M[4]
ldx M[5]
and x
ldx M[10]
xor x
st M[10]
ld M[7]
ldx M[4]
or x
st M[11]
ld M[5]
ldx M[6]
and x
ldx M[11]
xor x
st M[11]
ld M[8]
---------
true
```

We then went through and commented the assembly to figure out what each component did:

```
ld [0]				
jneq #777,200	; Syscall # != 777 -> exit

ld [16]				
and #255		
jge #128,199	; if char 1 > 128 exit
ld [16]				
rsh #8				
and #255			
jge #128,195	; if char 2 > 128 exit
ld [16]		
rsh #16
and #255
jge #128,191  ; if char 3 > 128 exit
ld [16]				
rsh #24
and #255
jge #128,187  ; if char 4 > 128 exit

ld [16]				; A = arg[0]
st M[0]				; M[0] = arg[0]

ld [24]  			
ldx M[0]			
xor x				
st M[1]				; M[1] = M[0] xor arg[1]

ld [32]				
ldx M[1]			
xor x				
st M[2]				; M[2] = arg[2] xor M[1]

ld [40]				
ldx M[2]			
xor x				
st M[3]				; M[3] = arg[3] xor M[2]

ld M[0]				
ldx M[1]			
add x				
ldx M[2]			
add x				
ldx M[3]			
add x				
st M[4]				; M[4] = M[0] + M[1] + M[2] + M[3]

ld M[0]				
ldx M[1]			
sub x				
ldx M[2]			
add x				
ldx M[3]			
sub x				
st M[5]				; M[5] = M[0] - M[1] + M[2] - M[3]

ld M[0]				
ldx M[1]			
add x				
ldx M[2]			
sub x				
ldx M[3]			
sub x				
st M[6]				; M[6] = M[0] + M[1] - M[2] - M[3]

ld M[0]				
ldx M[1]
sub x
ldx M[2]
sub x
ldx M[3]
add x
st M[7]				; M[7] = M[0] - M[1] - M[2] + M[3]

ld M[4]				
ldx M[5]			
or x			
st M[8]				
ld M[6]
ldx M[7]
and x				
ldx M[8]			
xor x				
st M[8]				; M[8] = (M[4] OR M[5]) XOR (M[6] AND M[7])

ld M[5]
ldx M[6]
or x
st M[9]
ld M[7]
ldx M[4]
and x
ldx M[9]
xor x
st M[9]				; M[9] = (M[5] OR M[6]) XOR (M[7] AND M[4])

ld M[6]
ldx M[7]
or x
st M[10]
ld M[4]
ldx M[5]
and x
ldx M[10]
xor x
st M[10]			; M[10] = (M[6] OR M[7]) XOR (M[4] AND M[5])

ld M[7]
ldx M[4]
or x
st M[11]
ld M[5]
ldx M[6]
and x
ldx M[11]
xor x
st M[11]			; M[11] = (M[7] OR M[4]) XOR (M[5] AND M[6])

ld M[8]              ; Check M[8]
jeq #4127179254,37
jeq #1933881070,20
jeq #4255576062,31
jeq #1670347938,10
jeq #2720551936,15
jeq #2307981054,26
jeq #2673307092,29
jeq #4139379682,10
jeq #4192373742,15
jeq #530288564,20
jeq #4025255646,29
jeq #3747612986,14
jeq #3098492862,3
jeq #2130820044,14

ld M[9]               ; Check M[9]
jeq #4056898606,50,84
ld M[9]
jeq #3064954302,32,82
ld M[9]
jeq #3602496994,24,80
ld M[9]
jeq #1627051272,34,78
ld M[9]
jeq #2002783966,34,76
ld M[9]
jeq #4088827598,28,74
ld M[9]
jeq #1340672294,36,72
ld M[9]
jeq #2115580844,12,70
ld M[9]
jeq #530288564,36,68
ld M[9]
jeq #3415533530,28,66
ld M[9]
jeq #3116543486,10,64
ld M[9]
jeq #251771212,14,62
ld M[9]
jeq #4126139894,20,60
ld M[9]
jeq #2813168974,6,58

ld M[10]                ; Check M[10]
jeq #2130523044,46,56
ld M[10]
jeq #3606265306,36,54
ld M[10]
jeq #3151668710,38,52
ld M[10]
jeq #614968622,34,50
ld M[10]
jeq #3086875838,42,48
ld M[10]
jeq #251771212,16,46
ld M[10]
jeq #3015552726,18,44
ld M[10]
jeq #1627379644,34,42
ld M[10]
jeq #1601724370,12,40
ld M[10]
jeq #665780030,18,38
ld M[10]
jeq #3281895882,14,36
ld M[10]
jeq #1301225350,30,34
ld M[10]
jeq #2583645294,20,32
ld M[10]
jeq #3917315412,6,30

ld M[11]                ; Check M[11]
jeq #2673307092,27,28
ld M[11]
jeq #1532821474,25,26
ld M[11]
jeq #3119098870,23,24
ld M[11]
jeq #3917315412,21,22
ld M[11]
jeq #2174343406,19,20
ld M[11]
jeq #666819390,17,18
ld M[11]
jeq #4143147994,15,16
ld M[11]
jeq #1827055294,13,14
ld M[11]
jeq #4290701286,11,12
ld M[11]
jeq #197094626,9,10
ld M[11]
jeq #2145762244,7,8
ld M[11]
jeq #2720880308,5,6
ld M[11]
jeq #3120414398,3,4
ld M[11]
jeq #3708166042,1,2

ret #2147418112         ; Syscall# != 777
ret #327680             ; Success
ret #327681             ; Error
```

after a bit we found out that:

```
M[0] = arg[0]
M[1] = M[0] XOR arg[1]
M[2] = arg[2] XOR M[1]
M[3] = arg[3] XOR M[2]
M[4] = M[0] + M[1] + M[2] + M[3]
M[5] = M[0] - M[1] + M[2] - M[3]
M[6] = M[0] + M[1] - M[2] - M[3]
M[7] = M[0] - M[1] - M[2] + M[3]
M[8] = (M[4] OR M[5]) XOR (M[6] AND M[7])
M[9] = (M[5] OR M[6]) XOR (M[7] AND M[4])
M[10] = (M[6] OR M[7]) XOR (M[4] AND M[5])
M[11] = (M[7] OR M[4]) XOR (M[5] AND M[6])
```

and that these values had to have the same values for each line

```
M[8]         M[9]         M[10]        M[11]
---------------------------------------------------------
4127179254   4126139894   3086875838   3120414398
1933881070   2002783966   1601724370   1532821474
4255576062   3116543486   3151668710   4290701286
1670347938   4056898606   2583645294   197094626
2720551936   1627051272   1627379644   2720880308
2307981054   3415533530   3281895882   2174343406
2673307092   251771212    251771212    2673307092
4139379682   3602496994   3606265306   4143147994
4192373742   4088827598   3015552726   3119098870
530288564    530288564    3917315412   3917315412
4025255646   2813168974   614968622    1827055294
3747612986   1340672294   1301225350   3708166042
3098492862   3064954302   3086875838   3120414398
2130820044   2115580844   2130523044   2145762244
```


We then decided to make a python script that would use z3 to solve for the different chunks. From the main function we had found earlier we knew that each input (arg[x]) would be equal to 4 characters. After adding all the constraints the python function then printed out the results in ascii format rather than as integers:

``` python
from z3 import *

# instantiate solver
s = Solver()

# create arguments
args = [BitVec(f'args[{i}]', 32) for i in range(0,14)]

# Add char constraints
for i in range(0, len(args)):
      
  # last character
  s.add(((args[i]) & 0xff) <= 126)

  # second to last character
  s.add(((args[i] >> 8) & 0xff) <= 126)

  # third from last character
  s.add(((args[i] >> 16) & 0xff) <= 126)

  # fourth from last character
  s.add(((args[i] >> 24) & 0xff) <= 126)

# add constraints
def solve(j):
  
  M = [BitVec(f"M[{j}][{i}]", 32) for i in range(0, 12)]
  s.add(M[0] == args[j])
  s.add(M[1] == M[0] ^ args[j+1])
  s.add(M[2] == M[1] ^ args[j+2])
  s.add(M[3] == M[2] ^ args[j+3])
  s.add(M[4] == M[0] + M[1] + M[2] + M[3])
  s.add(M[5] == M[0] - M[1] + M[2] - M[3])
  s.add(M[6] == M[0] + M[1] - M[2] - M[3])
  s.add(M[7] == M[0] - M[1] - M[2] + M[3])
  s.add(M[8] == (M[4] | M[5]) ^ (M[6] & M[7]))
  s.add(M[9] == (M[5] | M[6]) ^ (M[7] & M[4]))
  s.add(M[10] == (M[6] | M[7]) ^ (M[4] & M[5]))
  s.add(M[11] == (M[7] | M[4]) ^ (M[5] & M[6]))

  # adds row checks
  s.add(Or(
    And(M[8] == 4127179254, M[9] == 4126139894, M[10] == 3086875838, M[11] == 3120414398), 
    And(M[8] == 1933881070, M[9] == 2002783966, M[10] == 1601724370, M[11] == 1532821474), 
    And(M[8] == 4255576062, M[9] == 3116543486, M[10] == 3151668710, M[11] == 4290701286), 
    And(M[8] == 1670347938, M[9] == 4056898606, M[10] == 2583645294, M[11] == 197094626), 
    And(M[8] == 2720551936, M[9] == 1627051272, M[10] == 1627379644, M[11] == 2720880308), 
    And(M[8] == 2307981054, M[9] == 3415533530, M[10] == 3281895882, M[11] == 2174343406), 
    And(M[8] == 2673307092, M[9] == 251771212, M[10] == 251771212, M[11] == 2673307092), 
    And(M[8] == 4139379682, M[9] == 3602496994, M[10] == 3606265306, M[11] == 4143147994), 
    And(M[8] == 4192373742, M[9] == 4088827598, M[10] == 3015552726, M[11] == 3119098870), 
    And(M[8] == 530288564, M[9] == 530288564, M[10] == 3917315412, M[11] == 3917315412), 
    And(M[8] == 4025255646, M[9] == 2813168974, M[10] == 614968622, M[11] == 1827055294), 
    And(M[8] == 3747612986, M[9] == 1340672294, M[10] == 1301225350, M[11] == 3708166042), 
    And(M[8] == 3098492862, M[9] == 3064954302, M[10] == 3086875838, M[11] == 3120414398), 
    And(M[8] == 2130820044, M[9] == 2115580844, M[10] == 2130523044, M[11] == 2145762244)
    ))
      
# swaps characters around
def swap32(x):
    return (((x << 24) & 0xFF000000) |
            ((x <<  8) & 0x00FF0000) |
            ((x >>  8) & 0x0000FF00) |
            ((x >> 24) & 0x000000FF))

# prints the solution as ascii
def printAsAscii(num):
  num = swap32(num)
  hexStr = '{:08x}'.format(num)
  bytes_object = bytes.fromhex(hexStr)
  ascii_string = bytes_object.decode("ASCII")
  print(ascii_string)

# calls solve function for each argument
for i in range(0, len(args)-3):
  solve(i)

# solves the equation
print(s.check())
print(s.assertions())
m = s.model()

for d in m.decls():
    if "args" in d.name():
        print("%s = %s" % (d.name(), m[d]))
        printAsAscii(m[d].as_long())
```

After running the program we got:

```
args[13] = 1832088435
ss3m
args[3] = 2071164016
pts{
args[2] = 812803450
zer0
args[7] = 1601450859
k3t_
args[10] = 878670899
3t_4
args[11] = 1752445038
n0th
args[4] = 1802646338
B3rk
args[5] = 2033413171
3l3y
args[6] = 1664372831
_P4c
args[9] = 1497002547
3r:Y
args[8] = 1953247558
F1lt
args[12] = 878670387
3r_4
args[1] = 0

args[0] = 2105109602
bly}
```

After assembling this we got that the flag was:

```
zer0pts{B3rk3l3y_P4ck3t_F1lt3r:Y3t_4n0th3r_4ss3mbly}
```

Lots of thanks to NodeDigital for working with me on the go file and commenting of the assembly.