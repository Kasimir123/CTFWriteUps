# DiceCTF hyperlink Write Up

## Details:

Jeopardy style CTF

Category: Reverse Engineering

## Write up:

Looking at the python file I saw:

```py
import json


def test_chain(links, start, end):
    current = start
    for link in links:
        current = int(''.join(
            str(int(current & component != 0))
            for component in link
        ), 2)
    return end == current & end


def main():
    try:
        with open('hyperlink.json', 'r') as f:
            data = json.load(f)
    except IOError:
        print('Could not open hyperlink.json')
        return

    print('Welcome to the chain building game.')
    print('Enter a chain and see if it works:')

    chain = input()

    legal_chars = set('abcdefghijklmnopqrstuvwxyz{}_')
    if any(c not in legal_chars for c in chain):
        print('Chain contains illegal characters!')
        return

    try:
        links = [data['links'][c] for c in chain]
        result = test_chain(links, data['start'], data['target'])
    except Exception:
        print('Something went wrong!')
        return

    if result:
        print('Chain works! Congratulations.')
    else:
        print('Oh no! Chain does not reach the target.')


if __name__ == '__main__':
    main()
```

My first step for this was to see how the program acted when I entered some text so I decided to enter dice{ since I knew that was how the flag started. I added some code to print out the bits of the start, end, and each current value:


```
Welcome to the chain building game.
Enter a chain and see if it works:
dice{
start: 0b10000000000000000000000000000000000010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000
cur:    0b1000000000000000000000000000000000010001000100010001000100011001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000
cur:     0b100000000000000000000000000000000010001000100010001000100010101000110010001100100010001000100010001000110010001000100011001000100010001000100010001000100010001000
cur:      0b10000000000000000000000000000000010001000100010001000100010011000100010001010100010001000100010001000100010001000110010001000100010001000100010001000100010001000
cur:       0b1000000000000000000000000000000010001100100010001000100010011000100011001001100011001000110010001000100010001000101010001000100010001000100010001000100010001100
cur:        0b100000000000000000000000000000010001000100010001000100010011000100010001001100010001000101010001000100010001000100110001000100010001000100011001000100010001000
end:                                     0b1000010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001000100010001
```

This shifted some of the 1's to their correct position so I decided to dive into how the bit values get set. The code for this was in:

```py
    for link in links:
        current = int(''.join(
            str(int(current & component != 0))
            for component in link
        ), 2)
```

If any bit in both current and component were equal to 1 then we would get 1 for that position, otherwise we would get 0 so I decided to see what all the values in the last "link" was for each character:

```
j: 0b1
d: 0b1
r: 0b11
m: 0b1
y: 0b1
k: 0b1
v: 0b1
}: 0b1
c: 0b1
h: 0b1
u: 0b1
_: 0b1
l: 0b1
n: 0b1
s: 0b1
q: 0b1
e: 0b1
w: 0b1
b: 0b1
i: 0b1
t: 0b1
p: 0b1
g: 0b1
z: 0b1
f: 0b1
{: 0b1
a: 0b1
o: 0b1
x: 0b1
```

Looking at the values I noticed that the only value that would ever turn the last value into a 1 had to be 'r' and for that to work the second to last bit needed to equal 1 as well:

```
100 & 1  == 0
110 & 1  == 0
110 & 11 == 1
111 & 1  == 1
```

I then wrote a script to loop through all of the link values (0-163) and see which characters were different from the rest:

```
0 
1 
2 
3 
4 
5 
6 
7 
8 
9 
10 
11 
12 
13 
14 
15 
16 
17 
18 
19 
20 
21 
22 
23 
24 
25 
26 
27 
28 
29 
30 
31 
32 
33 
34 
35 
36 ['_']['g']['n']
37 ['n']
38 ['g']
39 ['_']
40 ['b']['r']['e']
41 ['e']
42 ['b']
43 ['r']
44 ['t']['y']['h']
45 ['y']
46 ['t']
47 ['h']
48 ['}']['a']['r']
49 ['r']
50 ['a']
51 ['}']
52 ['t']['y']['r']
53 ['r']
54 ['y']
55 ['t']
56 ['b']['a']['r']
57 ['b']
58 ['r']
59 ['a']
60 ['c']['i']['d']
61 ['d']
62 ['i']
63 ['c']
64 ['a']['e']['n']
65 ['n']
66 ['e']
67 ['a']
68 ['e']['i']['n']
69 ['i']
70 ['n']
71 ['e']
72 ['v']['e']
73 ['e']
74 ['v']
75 ['e']
76 ['c']['e']['i']
77 ['i']
78 ['c']
79 ['e']
80 ['_']['i']['s']
81 ['_']
82 ['i']
83 ['s']
84 ['y']['r']['e']
85 ['e']
86 ['r']
87 ['y']
88 ['_']['a']['r']
89 ['a']
90 ['r']
91 ['_']
92 ['e']['{']
93 ['e']
94 ['{']
95 ['e']
96 ['_']['g']['i']
97 ['g']
98 ['_']
99 ['i']
100 ['_']['a']['l']
101 ['_']
102 ['a']
103 ['l']
104 ['_']['i']['s']
105 ['i']
106 ['s']
107 ['_']
108 ['i']['l']['n']
109 ['l']
110 ['i']
111 ['n']
112 ['a']['g']['l']
113 ['a']
114 ['l']
115 ['g']
116 ['c']['e']['{']
117 ['c']
118 ['e']
119 ['{']
120 ['g']['i']['n']
121 ['i']
122 ['n']
123 ['g']
124 ['_']['l']['s']
125 ['s']
126 ['_']
127 ['l']
128 ['i']['n']['h']
129 ['h']
130 ['i']
131 ['n']
132 ['t']['i']['h']
133 ['t']
134 ['h']
135 ['i']
136 ['_']['i']['l']
137 ['_']
138 ['l']
139 ['i']
140 ['_']['a']['r']
141 ['r']
142 ['_']
143 ['a']
144 ['v']['e']['{']
145 ['{']
146 ['e']
147 ['v']
148 ['g']['e']['l']
149 ['l']
150 ['g']
151 ['e']
152 ['b']['g']['e']
153 ['g']
154 ['e']
155 ['b']
156 ['v']['r']['e']
157 ['v']
158 ['e']
159 ['r']
160 ['a']['r']['e']
161 ['e']
162 ['a']
163 ['r']
```

Looking through this a bit I saw that each set of 3 characters would shift and set one of the bits that we needed and saw that all possible sets of 3 were:

```
geb ear ver lge {ev r_a _li thi hing s_l ing ce{ alg lin is_ _al g_i e{e ar_ ery _is ice eve ine nea dic bra ryt ra} yth ebr ng_
```

Each link we have here started with what the last one ended with, some would be 2 characters while others were just 1:

dic + ice => dice

I then wrote a script that looped through and combined all the values for the flag:

```py
# import json
import json

# main function
def main():

    # open json data
    with open('hyperlink.json', 'r') as f:
        data = json.load(f)

    # all legal chars
    legal_chars = set('abcdefghijklmnopqrstuvwxyz{}_')
    
    # get start and end
    start = data['start']
    end = data['target']
    
    s = []
    st = ""

    # loop through and find all unique values
    for i in range(0, len(data['links']['a'])):
        arr = {}

        # make a dictionary with all values and characters
        for c in legal_chars:
            if  data['links'][c][i] not in arr:
                arr[data['links'][c][i]] = []
            arr[data['links'][c][i]].append(c)

        # find how many characters for each value
        max_val = 0
        for a in arr:
            if len(arr[a]) > max_val:
                max_val = len(arr[a])

        # remove the value with the most amount of characters
        not_unique = None
        for a in arr:
            if len(arr[a]) == max_val:
                not_unique = a
        arr.pop(not_unique, None)

        # create our array
        if len(arr) > 1:
            if len(st) > 0:
                s.append(st)
            st = ""
        else:
            st += ''.join([str(arr[a][0]) for a in arr])
    
    # start of flag
    flag = " d"
    
    # loop through and find which have overlap
    while len(s) > 1 and flag[-1] != '}':
        pos = []
        pos2 = []
        for i in s:
            if i[0] == flag[-1]:
                pos.append(i)
            elif i[:2] == flag[-2:]:
                pos2.append(i)
        if len(pos2) == 1:
            flag = flag[:-2] + pos2[0]
            s.remove(pos2[0])
        else:
            flag = flag[:-1] + pos[0]
            s.remove(pos[0])

    # print flag
    print(flag)
        


if __name__ == '__main__':
    main()
```

When run I got:

```
 dice{everything_is_linear_algebra}
```