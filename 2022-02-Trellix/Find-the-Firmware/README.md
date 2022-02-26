# Trellix Find the Firmware Write Up

## Details:

Jeopardy style CTF

Category: Network

Comments:

```
I've got a packet capture from the K9's network. I overheard them talk about pushing firmware updates for their internal IOT devices. See if you can sniff out the firmware on the 192.168.86.0/24 subnet. Maybe there are some secrets buried in the firmware file system! According to my sources, the MD5 checksum for the embedded download.bin file is 7aa6a7ebcbd98ce19539b668ff790655. I tried looking myself but I didn't find any files with that MD5. It was ruff. Maybe my tools aren't extracting correctly? See what you can do!
```

## Write up:

I started this challenge by using tshark in order to extract all of the files from the pcap since I had noticed a lot had the name downloaded?= and had an offset:

```
tshark -r firmware.pcap --export-objects http,firmware-files
```

All of these files contained base64 encoded data so I started working on a script that appended the files together after base64 decoding the file and then saving that as the binary:

```py
# import os and base64
import os, base64

# get all of the files in the path
files = [f for f in os.listdir('.') if os.path.isfile(f)]

# create a list
sorted_list = list()

# loop through all of the files that have download in their name
for i in files:
    if "download" in i:
        # append the offset number and the file name
        sorted_list.append((int(i.split('offset=')[1].split('&')[0]), i))

# sort the list
sorted_list.sort()

# instantiate data
data = b''

# loop through all of the files and decode the data and add them
for i in sorted_list:
    with open(i[1],'rb') as f:
        data += base64.b64decode(f.read())

# write the firmware file
f = open('./firmware.bin', 'wb')
f.write(data)
f.close()
```

However, after running the firmware did not have the right md5 hash:

```
md5sum firmware.bin 

ef3feeb1a5f33e211477eb945a0c03a8  firmware.bin
```

I then updated the python script to clean it up and in order to check if we were missing any values:

```py
# import os and base64
import os, base64

# get all of the files in the path
files = [f for f in os.listdir('.') if os.path.isfile(f)]

# create a list
sorted_list = list()

# loop through all of the files that have download in their name
for i in files:
    if "download" in i:
        # append the offset number and the file name
        sorted_list.append((int(i.split('offset=')[1].split('&')[0]), i))

# sort the list
sorted_list.sort()

# instantiate data
data = b''

# loop through all of the files and decode the data and add them
for i in sorted_list:
    if (len(data) != i[0]):
        print(len(data), i[0])
        break
    with open(i[1],'rb') as f:
            data += base64.b64decode(f.read())

# write the firmware file
f = open('./firmware.bin', 'wb')
f.write(data)
f.close()
```

I found that there were two missing values: `1904640` and `3505152`. I then went into wireshark and manually searched for those strings and found the data for those values. I then manually added these in the script:

```py
# import os and base64
import os, base64

# get all of the files in the path
files = [f for f in os.listdir('.') if os.path.isfile(f)]

# create a list
sorted_list = list()

# loop through all of the files that have download in their name
for i in files:
    if "download" in i:
        # append the offset number and the file name
        sorted_list.append((int(i.split('offset=')[1].split('&')[0]), i))

# sort the list
sorted_list.sort()

# instantiate data
data = b''

# loop through all of the files and decode the data and add them
for i in sorted_list:
    if len(data) == 1904640:
        print("added")
        data += base64.b64decode("w5yDSXo1DeR1y3qqAJkHwS8D+QGY/9qX6oGZyo8tSQ5XExR6Q5JqsG/D6blFctKFjI4JYwHd4wP3UCg0uIPBGMnNkvoJxJHQ78nUuixABMF375j9+E3hZOpF0aLFhvC49i+FNdJX2bFk+u2SlqCjbwtXU6d2BAmaU+uM7TLFchyt3aYGJIgKaDW+qiP7PF1JpedK9VMqvpgXoOUzVrikLVKtRnpEANjcaZdGJVMxwYHLbMtgIOu14+Oo+PUH+mfi93+T94EKxk1onh7JEthWV703Qgt3E65yN8hbSAUpGJHTYTu+HPYB0IBojuIL1pzkjpcoveOeD7aWm325xOz7LpDTjPRReQlvHCtNQGF9SmQaWi+NGXI0G8N0LUJHjF+aYbDVkVSkEj7QhqPOGeFzurTrt4bxsJnnhMBUavrjafAkO9LjjdZ2NCkJeHcsYQ2ER8eBGvV4Ej818ZltB18mji69GR8h2UY9sDY/ISiIQj2ReTXAoomgsEBrh6aH7Khs9qJALeQjowxbSCPIV2RAMbfVawINrkDU9V9+rhvHJ+VuON++qtB8g7voykPb2XUJ+Ce7d+KWeC6IZa7OYAQv949k0RCPv+nxX3Wz9bP8puHAqo8hdgtKZNBodojWWLzeMktce7v+BngPllyuZ8RSAi/rfouNW9DfbZG68Pa5OcuT/NUXW85g/NZiCZLKmQqZ3F+MTYb1rtdyy8YzewlMyF/8suHf6VLAmgknHwbBFKvSXeuI1KBSnM7a4CyUx4kWQcfcdRt+93dPdWq9C7r1AtamRswGdG7i8Nl6vc023OElY8IC4oybE4kFR3/nNzQpbqOG0oH70hXVz/w2GS/hKINY6NSbXUX7HJhbDMXSKcw5v7PavjB0GehSJVk04i0LeGSycYK3974shYgoXm3IfZ9pdLzmRL8PAvuYcbG8zDtZaEjlsXlkuXDWBLD4oJKfBaXqWiSVUC6uBGHR49KnXB9PMwz3EByEfL7RvokKYHHqBvZL6Y64dv1vcDx4PcYq7tdTG2Jl5oNZHcyOPsSg/+fLljdGMvry7GBwk6nIwlrizDZLFjwyHzAI2B94NSge5R8P9iKpQFj1Ss6SJzKfz6DIQOlvcdgbB3juDDRI3LaAtBiQyREYqhiQohFuHq6/N+we5d++HPkLwMEnWNVECAQsKlj8uMaxQKHIsLcEWt6c7Jh+FShLHp9SVkjrOGIBAYsmaR08jYIw/VQktLuDqgJlfc8FH/OFYI83eeEAzYNAo80BJeUi5k3C3T1CgdC36VlduwOtUyoO/3LOrNc8G7c1f+K2sYLXdX7fyQ0Pg3vexU1O3ZdXXhVYGowOq9+jNlrqQU3+siYz0xXF/OJ/jw==")

    if len(data) == 3505152:
        print("added")
        data += base64.b64decode("BUrRASUwfJyNIxrgBc+b1IocZJNfxdhO1kfdQTVevRy5ZpG99RFNcF7ai9AzYRqwMJC9HEH3SkJs3N0jf3YnqDfJyzJkF4+iC/eSASnbsfE8JyXbcZfxmetCEv2iG4woVbUJh0PvBFe3ZZIz+4lwoIFUm6O+Ll9Hv6Pt6bebCcOK0YIvWfMfSgSDkFVPQA/8Nrc2nZ+OuN7fO+wgMfMO6U8f5p60NUGW4U399+de0FVRiduNptm7t/85YTJAw4A08e2TKQkFZj5raAETBJtUPinr3Vx1j8MSJsvNddrIKX/Imkip58UckASYLHn9jQZoeBreIXR2xijb7JVzYp+412m4OkQs6S786iuDyV4CSiTSZH/8mdcdmHCYlf+Etb0p+SuAT7YasHyTitBh2qlDoOyOpnFhPBKShEbEVs8UPV4qB9imhYHaxu7wmBDYqqnFJqFVim5FByEPWy6W+JqWLjKc1JJ//OGeow0u4PebDOEBzUVnbmWA0M8o54SKo1/TTqDnRCbIh6++8SuRYjUreUcQ4QoaCb7LWvnkzU5cFkJDGSjJffs4TOr1AaLyoaV6CQ0+9x2o+pj74R9Au7jDka0VR7jxHaXCBHikq5aB4fQDxCfejvWR0T82wtDt1Q7YyejhqjVHt2koeMfXoOtGU6XLGPrQ6WurALS4ivYipFBGlsnQVNYkUfnzlzovq9YCQKmhVRqMVBBt+t0mR3/2HvKu8Hsda3+uG2Z0dbQYKV6w5tmF8/0rG6K+QE0aiJ8q7ls57idPQXQAmCW6qO1IjsXgVIwSL8F0wsquVz6dA+RlLQ3vjC+BfOjKXuPMZnbyTWLenqjwB/yXraIgY0y65FtQ13/bhd+k8LO/I6Wgy3okiKEBMcO8bZOIo6gSx+F61M1DGwfF2Ny2kN8JkGqmmrffDSVoT3g1KlPSquJymPTBKdW88ODr87W47QaMwK04DvnLK4V8C3ABRqSYtn2iGmBgkfoTmHz6J8aOO55ElfSiQVmtcDIkeLs0El5v1QsmsxQk8rVq5/QsealcWPqE/wEuKS5w+iC9DdwvKF4JTB5gLeQZ8vASeHmO5oarxZZ/V3S/ygt7V18pOhVRrFwGCPLk2RwXQBYHJlF5nyCnbm5VHULhLZH6zLAOFLL0m+r7hH9a3Aqamdk4d8MV8nQ25HPUYSvtUbTlai1w6xADh6ttgcjmMN0rr89JlGmEMjnQ6co1kSyhl/b8Q2tHyWRCduVAehJcsr15ddDQD5CHYf4dfv/o1dICMcHgeR0K0JJ3TqZ60Fmm4ORRiM5giRGwLRUHAzHLYLRmshfq4j+q5QbwZD7AG+JGcQHcbY8S2eAZGixrRTqyXXVHxNmjbcoW5g==")
    with open(i[1],'rb') as f:
            data += base64.b64decode(f.read())

# write the firmware file
f = open('./firmware.bin', 'wb')
f.write(data)
f.close()
```

Running this produced a firmware.bin that when I hashed had the correct hash:

```
md5sum firmware.bin 

7aa6a7ebcbd98ce19539b668ff790655  firmware.bin
```

I then used binwalk to extract the firmware, using the M flag to recursively unzip:

```
binwalk -Me firmware.bin
```

After that I went into the folder and ran grep for the flag:

```
grep -ri ATR

squashfs-root/root/.secret/flag:ATR[F1rMW4r315N750H4rD]
Binary file squashfs-root/bin/opkg matches
Binary file 60.7z matches
```