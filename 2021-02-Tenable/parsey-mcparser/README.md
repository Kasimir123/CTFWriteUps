# Tenable Parsey McParser Write Up

## Details:
Points: 50

Jeopardy style CTF

Category: Code

Comment:

Attached is a blob of data with a never-before-seen format. We need user_names extracted from all entries based on the group they're in. Direcly below, is some starter code. All we need is the function "ParseNamesByGroup" implemented which takes a blob of data (string) and the group name (string). This function should return all user_names belonging to that group.

``` python
'''
:param blob: blob of data to parse through (string)
:param group_name: A single Group name ("Green", "Red", or "Yellow",etc...)

:return: A list of all user names that are part of a given Group
'''
def ParseNamesByGroup(blob, group_name):
    #impliment code
   
data = raw_input()
group_name = data.split('|')[0]
blob = data.split('|')[1]
result_names_list = ParseNamesByGroup(blob, group_name)
print result_names_list
```

stdin example:

```
Black|+++,Bellhomes LLC.,["age":39, "user_name":"Reid Jolley", "Group":"Black"],+++,Greek Ideas,["age":63, "user_name":"Lucius Chadwell", "Group":"Green"],["age":63, "user_name":"Cary Rizzuto", "Group":"Black"],["age":28, "user_name":"Shoshana Bickett", "Group":"Yellow"],["age":69, "user_name":"Madeleine Swallow", "Group":"Green"],["age":41, "user_name":"Buddy Etter", "Group":"Black"],+++,God fire,["age":26, "user_name":"Carlene Caulder", "Group":"Green"],["age":43, "user_name":"Napoleon Peay", "Group":"Purple"],["age":44, "user_name":"Noemi Constant", "Group":"Green"]
```

stdout example:

```
['Reid Jolley', 'Cary Rizzuto', 'Buddy Etter']
```

You can ignore the default code below and instead use code above. Once function is implemented, simply copy/paste into box and run. Ensure not to print anything else during your code execution!

## Write up:

This challenge was python2, pretty simple script so I'm just adding the commented scripts.

``` python
'''
:param blob: blob of data to parse through (string)
:param group_name: A single Group name ("Green", "Red", or "Yellow",etc...)

:return: A list of all user names that are part of a given Group
'''
def ParseNamesByGroup(blob, group_name):
    #impliment code

    # was a bit tired and restless when writing this so I chose random names, this one splits the input by comma
    sploot = blob.split(',')
    sploot2 = []
    printable = []

    # Looped through all the items and selected each item with Group or user_name in it
    for x in sploot:
        if 'Group' in x or 'user_name' in x:
            sploot2.append(x)

    i = 1
    # Loop through the selected and if the username is the one I was given add it to the printable list
    while i < len(sploot2):
        if group_name in sploot2[i]:
            printable.append(sploot2[i-1].replace('"user_name":', '').replace('"', '').strip())
        i += 2

    # Return list
    return printable
   
data = raw_input()
group_name = data.split('|')[0]
blob = data.split('|')[1]
result_names_list = ParseNamesByGroup(blob, group_name)
print(result_names_list)
```