# CSAW ncore Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

We have a very safe core with a very safe enclave.

nc rev.chal.csaw.io 5002


## Write up:

The server.py file was not too important for this challenge as it only really set up the verilog.

The most important parts of the verilog were:

The opcodes and their values:

```verilog
  `define ADD  4'd0
  `define SUB  4'd1
  `define AND  4'd2
  `define OR   4'd3
  `define RES 4'd4
  `define MOVF 4'd5
  `define MOVT 4'd6
  `define ENT  4'd7
  `define EXT  4'd8 
  `define JGT  4'd9
  `define JEQ  4'd10
  `define JMP  4'd11
  `define INC  4'd12
  `define MOVFS 4'd13
```

How the flag was getting loaded into memory:

```verilog
  task load_safeROM();
    $display("loading safe rom, safely...");
    $readmemh("flag.hex",safe_rom);
  endtask
```

The key generation:

```verilog
  task set_key();
    int tmp;
    // key[0] = 0;
    read_file_descriptor=$fopen("/dev/urandom","rb");
    tmp = $fread(key, read_file_descriptor);
    $readmemh("/dev/urandom",key);
  endtask
```

What was printed at the end of the program:

```verilog
 task print_res();
    integer i;
    for( i=0; i<64; i = i + 1) begin
      $write("%h ",ram[255-i]);
    end
    $write("\n");
  endtask
```

The opcode that moved memory from the flag.hex file into memory we can print:

```verilog
     `MOVFS: begin
       if(emode) begin 
        regfile[ram[pc][5:4]] <= safe_rom[ram[pc+1]];
       end
       increment_pc();
```

And the condition and opcode we needed to meet in order for us to be able to call the previous opcode:

```verilog
      `ENT: begin
        // $display("%d | %d",regfile[0],key[0][13:0]);
       if(key[0][13:0]==regfile[0]) begin
         emode <= 1;
         regfile[3] <= 0;
          $display("ENT");
       end else begin
         regfile[3] <= 1;
       end
       increment_pc();
```

Based on this information we saw that we would need to do the following:

```
ENT
INC [0]
ENT
JGT [3] 0
MOVFS
```

We first call ENT in order to set regfile[3] to 1. We then increment regfile[0] since that is what is compared to the key. We then run ENT again to check if we got the key. Then we run JGT to see if regfile[3] is still 1 or 0, if it is 1 we return back to the INC, otherwise we move the safe_rom data to the ram.

The code for the first part of this was:

```
07 00 0c 00 07 00 79 02
```

We then had to add some padding to our "code" since we didn't want to write the flag to the last 64 bytes for them to then be parsed and thus changing the output. So we put 184 ff's so that our final move code would be in the last 64 printable bytes.

Since we only had 64 printable bytes we could only display 16 flag characters per connection since the code for each character would be:

```
0d [index of character in flag] 06 [ram position to write to]
```

I then wrote the following solve script:

```py
from pwn import *

flag = []

# connect twice since the flag is 32 long
for i in range(2):

    # connect to the server
	r = remote('rev.chal.csaw.io', 5002)

    # receive the welcome line
	r.recvline()

    # initialize the string
	exploit = ''

    # loop through 16 times to write the exploit
	for j in range(16):

        # get the character index in the flag 
		count = str(hex(j + (i * 16)))[2:]

        # if the hex is small (< 0x10) we need to prepend 0
		if len(count) == 1:
			count = '0'+ count

        # add the code to the exploit
		exploit += '0d ' + count + ' 06 ' + str(hex(0xc0 + j))[2:] + ' '

    # send the "code" we generate
	r.sendline('07 00 0c 00 07 00 79 02 ' + ('ff ' * 184) + exploit)
	
    # receive the data we just sent
    r.recvline()

    # get the output
	output = r.recvline()

    # get rid of the parts we don't care about
	output = str(output[output.find(b'\\nENT\\n') + 7:])[2:-8]

    # append the 16 bytes (and spaces between them) we care about to flag
	flag.append(output[-47:])

# intialize our output string
s = ""

# loop through all flag components
for i in flag:

    # intialize current string
	cur = ""

    # loop through the current component, increment by 3 (2 byte characters and space)
	for j in range(0, len(i), 3):

        # if the character isn't an x (this was from when we overshot the flag)
		if i[j] != 'x':

            # turn the byte into a character
			cur += chr(int(i[j]+i[j+1], 16))
    # append the current string in reverse order to the string        
	s += cur[::-1]

# print the flag
print(s)
```

When run I got:

```
[+] Opening connection to rev.chal.csaw.io on port 5002: Done
[+] Opening connection to rev.chal.csaw.io on port 5002: Done
flag{d0nT_mESs_wiTh_tHe_sChLAmi}
```
