# Tenable Is the King in Check? Write Up

## Details:
Points: 200

Jeopardy style CTF

Category: Code

Comment:

We have several chess matches on an 8x8 board needing to know if either king is in check or not. Below is some starter code that reads some chess board data from several chess matches.

``` python
'''
Takes '+' and ' ' delimited data of chess matches and parses into list of seperate matches
''' 
def ParseMatches(chess_matches):
    return [c.split('+') for c in chess_matches.split(' ')]

'''
:param chess_match: A list of chess pieces and their location on the board. ie: ['w,p,a2', 'w,q,a6','w,k,c1','b,r,h1','b,k,g3']
:return: returns True or False if a King is in check
'''
def IsKingInCheck(chess_match):
    # impliment code

# Parses chess matches from raw_input and calls "IsKingInCheck" for each match. Result is then printed
result = []
chess_matches = ParseMatches(raw_input())
for chess_match in chess_matches:
    result.append(IsKingInCheck(chess_match))
    
print result
```

Impliment "IsKingInCheck" function which takes a single matches' chess data and returns True if the one of the Kings are in check (or checkmate), otherwise False. The single matches' chess data will be a list of pieces and their location on the board in the format of ["color, rank, cordinates"].

Example ["w,p,a2", "w,q,b6", "w,r,h2", "w,k,c1", "w,p,c7", "w,p,g7", "w,p,e6", "b,k,d7", "b,p,e6", "b,r,a6",]

Color

    "b" = Black
    "w" = White

Rank

    "q" = Queen
    "k" = King
    "b" = Bishop
    "r" = Rook
    "n" = Knight
    "p" = Pawn

Cordinates

    This describes where the piece currently is on the board and will be in typical chess cordinates, such as "a6" or "b7", as seen on a labeled chess board.

In this chess game, White opponent starts at the lower numbers of the board cordinates and Black opponent at higher cordinates.

stdin example:

```
w,p,c6+w,q,c8+w,p,g7+w,k,e5+b,b,b2+b,p,f3+b,k,f1
w,p,c4+w,r,a6+w,p,e6+w,p,h6+w,p,g7+w,k,h5+b,r,b2+b,p,f3+b,k,c2
```

stdout example:

```
[True, False]
```

You can ignore the default code below and instead use code above. Once function is implemented, simply copy/paste into box and run. Ensure not to print anything else during your code execution!

## Write up:

The challenge was given for python2 code. 

For this challenge I decided to find the kings and then branch out each way until they hit a piece, then checked if that piece would check it. I checked horizontal, vertical, diagonal, and for knight check. The code is not very neat and has a lot of duplicate code rather than complex design.

``` python
board = []

# maps letters to numbers on the board i made
postDict = {'a':1, 'b':2,'c':3, 'd':4, 'e':5, 'f':6, 'g':7, 'h':8}

# resets my board set up
def resetBoard():
    global board
    board = []
    board.append(['0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'])
    for i in range(1, 9):
        lst = [i]
        lst += [None]*8
        board.append(lst)

# splits a word
def split(word): 
    return [char for char in word]  

# attaches the piece to the board
def pieceToBoard(piece):
    color = piece.split(',')[0]
    curPiece = piece.split(',')[1]
    pos = piece.split(',')[2]
    board[postDict[split(pos)[0]]][int(split(pos)[1])] = str(color)+str(curPiece)

'''
Takes '+' and ' ' delimited data of chess matches and parses into list of seperate matches
''' 
def ParseMatches(chess_matches):
    return [c.split('+') for c in chess_matches.split(' ')]

# Gets the white king
def getWhiteKing(chess_match):
    for x in chess_match:
        if 'k' in x and 'w' in x:
            return x

# gets the black king
def getBlackKing(chess_match):
    for x in chess_match:
        if 'k' in x and 'b' in x:
            return x

# checks the diagonal spaces
def checkDiagonal(chess_match):
    wkPos = getWhiteKing(chess_match).split(',')[2]

    x = postDict[split(wkPos)[0]]
    y = int(split(wkPos)[1])

    # check top right
    i = x + 1
    j = y - 1
    while i < 8 and j > 0:
        pos = board[i][j]
        if pos is not None:
            if pos == "bb" or pos == "bq":
                return True
            else:
                break
        i += 1
        j -= 1

    # check top left
    i = x - 1
    j = y - 1
    while i > 0 and j > 0:
        pos = board[i][j]
        if pos is not None:
            if pos == "bb" or pos == "bq":
                return True
            else:
                break
        i -= 1
        j -= 1
    # check bottom left

    i = x - 1
    j = y + 1
    while i > 0 and j < 8:
        pos = board[i][j]
        if pos is not None:
            if pos == "bb" or pos == "bq":
                return True
            elif pos == 'bp' and i == x-1 and j == y+1:
                return True
            else:
                break
        i -= 1
        j += 1
    # check bottom right

    i = x + 1
    j = y + 1
    while i < 8 and j < 8:
        pos = board[i][j]
        if pos is not None:
            if pos == "bb" or pos == "bq":
                return True
            elif pos == 'bp' and i == x+1 and j == y+1:
                return True
            else:
                break
        i += 1
        j += 1

    bkPos = getBlackKing(chess_match).split(',')[2]

    x = postDict[split(bkPos)[0]]
    y = int(split(bkPos)[1])

    # check top right
    i = x + 1
    j = y - 1
    while i < 8 and j > 0:
        pos = board[i][j]
        if pos is not None:
            if pos == "wb" or pos == "wq":
                return True
            elif pos == 'wp' and i == x+1 and j == y-1:
                return True
            else:
                break
        i += 1
        j -= 1

    # check top left
    i = x - 1
    j = y - 1
    while i > 0 and j > 0:
        pos = board[i][j]
        if pos is not None:
            if pos == "wb" or pos == "wq":
                return True
            elif pos == 'wp' and i == x-1 and j == y-1:
                return True
            else:
                break
        i -= 1
        j -= 1
    # check bottom left

    i = x - 1
    j = y + 1
    while i > 0 and j < 8:
        pos = board[i][j]
        if pos is not None:
            if pos == "wb" or pos == "wq":
                return True
            else:
                break
        i -= 1
        j += 1
    # check bottom right

    i = x + 1
    j = y + 1
    while i < 8 and j < 8:
        pos = board[i][j]
        if pos is not None:
            if pos == "wb" or pos == "wq":
                return True
            else:
                break
        i += 1
        j += 1


    return False

# checks the horizontal and vertical spaces
def checkHorizontal(chess_match):
    wkPos = getWhiteKing(chess_match).split(',')[2]

    x = postDict[split(wkPos)[0]]
    y = int(split(wkPos)[1])

    #top
    j = y-1
    while j > 0:
        pos = board[x][j]
        if pos is not None:
            if pos == "br" or pos == "bq":
                return True
            else:
                break
        j -= 1

    #left
    i = x-1
    while i > 0:
        pos = board[i][y]
        if pos is not None:
            if pos == "br" or pos == "bq":
                return True
            else:
                break
        i -= 1
    #right
    i = x+1
    while i < 8:
        pos = board[i][y]
        if pos is not None:
            if pos == "br" or pos == "bq":
                return True
            else:
                break
        i += 1
    #bottom
    j = y+1
    while j < 8:
        pos = board[x][j]
        if pos is not None:
            if pos == "br" or pos == "bq":
                return True
            else:
                break
        j += 1



    bkPos = getBlackKing(chess_match).split(',')[2]

    x = postDict[split(bkPos)[0]]
    y = int(split(bkPos)[1])


    #top
    j = y-1
    while j > 0:
        pos = board[x][j]
        if pos is not None:
            if pos == "wr" or pos == "wq":
                return True
            else:
                break
        j -= 1

    #left
    i = x-1
    while i > 0:
        pos = board[i][y]
        if pos is not None:
            if pos == "wr" or pos == "wq":
                return True
            else:
                break
        i -= 1
    #right
    i = x+1
    while i < 8:
        pos = board[i][y]
        if pos is not None:
            if pos == "wr" or pos == "wq":
                return True
            else:
                break
        i += 1
    #bottom
    j = y+1
    while j < 8:
        pos = board[x][j]
        if pos is not None:
            if pos == "wr" or pos == "wq":
                return True
            else:
                break
        j += 1

    return False

# checks for check by knight
def checkKnight(chess_match):
    wkPos = getWhiteKing(chess_match).split(',')[2]

    x = postDict[split(wkPos)[0]]
    y = int(split(wkPos)[1])

    # top right1
    if x+2 <= 8 and y-1 > 0:
        if board[x+2][y-1] == 'wn':
            return True
    # top right2
    if x+1 <= 8 and y-2 > 0:
        if board[x+1][y-2] == 'wn':
            return True
    # top left1
    if x-2 > 0 and y-1 > 0:
        if board[x-2][y-1] == 'wn':
            return True
    # top left2
    if x-1 > 0 and y-2 > 0:
        if board[x-1][y-2] == 'wn':
            return True
    #bottom left1
    if x-2 > 0 and y+1 <= 8:
        if board[x-2][y+1] == 'wn':
            return True
    #bottom left2
    if x-1 > 0 and y+2 <= 8:
        if board[x-1][y+2] == 'wn':
            return True
    #bottom right1
    if x+2 <= 8 and y+1 <= 8:
        if board[x+2][y+1] == 'wn':
            return True
    #bottom right2
    if x+1 <= 8 and y+2 <= 8:
        if board[x+1][y+2] == 'wn':
            return True

    bkPos = getBlackKing(chess_match).split(',')[2]

    x = postDict[split(bkPos)[0]]
    y = int(split(bkPos)[1])

    # top right1
    if x+2 <= 8 and y-1 > 0:
        if board[x+2][y-1] == 'wn':
            return True
    # top right2
    if x+1 <= 8 and y-2 > 0:
        if board[x+1][y-2] == 'wn':
            return True
    # top left1
    if x-2 > 0 and y-1 > 0:
        if board[x-2][y-1] == 'wn':
            return True
    # top left2
    if x-1 > 0 and y-2 > 0:
        if board[x-1][y-2] == 'wn':
            return True
    #bottom left1
    if x-2 > 0 and y+1 <= 8:
        if board[x-2][y+1] == 'wn':
            return True
    #bottom left2
    if x-1 > 0 and y+2 <= 8:
        if board[x-1][y+2] == 'wn':
            return True
    #bottom right1
    if x+2 <= 8 and y+1 <= 8:
        if board[x+2][y+1] == 'wn':
            return True
    #bottom right2
    if x+1 <= 8 and y+2 <= 8:
        if board[x+1][y+2] == 'wn':
            return True

    return False

'''
:param chess_match: A list of chess pieces and their location on the board. ie: ['w,p,a2', 'w,q,a6','w,k,c1','b,r,h1','b,k,g3']
:return: returns True or False if a King is in check
'''
def IsKingInCheck(chess_match):
    # impliment code
    ret = False

    for i in chess_match:
        pieceToBoard(i)

    ret |= checkDiagonal(chess_match)
    ret |= checkHorizontal(chess_match)
    ret |= checkKnight(chess_match)
    return ret
    

# Parses chess matches from raw_input and calls "IsKingInCheck" for each match. Result is then printed
result = []



chess_matches = ParseMatches(raw_input())
for chess_match in chess_matches:
    resetBoard()
    result.append(IsKingInCheck(chess_match))
    
print(result)
```