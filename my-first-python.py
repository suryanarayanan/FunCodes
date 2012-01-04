#!/usr/bin/python
import sys
board = [[0,0,0],[0,0,0],[0,0,0]]
PLAYER = 1
COMP = 2
NONE = 0
PLAYER_WIN = 0
DRAW = 1
COMP_WIN = 2
INT_MAX = 2**30

def play(x, y, who):
    
    if(who == COMP):
        result = -1 
        if(who_won() == PLAYER):
            return PLAYER_WIN
        elif(game_over()):
            return DRAW
        for i in range(3):
            for j in range(3):
                if(board[i][j] == NONE):
                    board[i][j] = COMP
                    result = max(result,play(i,j,PLAYER))
                    board[i][j] = NONE
        return result
        
                
    elif(who == PLAYER):
        result = INT_MAX
        if(who_won() == COMP):
            return COMP_WIN
        elif(game_over()):
            return DRAW
        for i in range(3):
            for j in range(3):
                if(board[i][j] == NONE):
                    board[i][j] = PLAYER
                    result = min(result, play(i,j,COMP))
                    board[i][j] = NONE
        return result
    
                
def game_over():
    for i in range(3):
        for j in range(3):
            if(board[i][j] == 0):
                return False
    return True

def is_a_win(who):
    count_diag1 = count_diag2 = 0
    for i in range(3):
        count_row = count_col = 0
        for j in range(3):
            if(i == j and board[i][j] == who): count_diag1 += 1
            if(i + j == 2 and board[i][j] == who): count_diag2 += 1
            if(board[i][j] == who): count_row += 1
            if(board[j][i] == who): count_col += 1
        if(count_row == 3 or count_col == 3 or count_diag1 == 3 or count_diag2 == 3):
            return True
    return False

def who_won():
    #For Computer
    if(is_a_win(COMP)):
        return COMP
    #For Player
    elif(is_a_win(PLAYER)):
        return PLAYER


def print_board():
    for i in range(3):
        for j in range(3):
            print(board[i][j])," ",
        print "\n",

print_board()
while(game_over() != True):
    #get input
    x = y = 0
    print "Enter x and y co-ordinates(0 to 2 space seperated): ",
    input = raw_input()
    x,y = [int(i) for i in input.split()]
    while(x > 2 or x < 0 or y > 2 or y < 0 or board[x][y] != 0):
        print "Invalid Input"
        print "Enter x and y co-ordinates(0 to 2 space seperated): ",
        input = raw_input()
        x,y = [int(i) for i in input.split()]

    board[x][y] = PLAYER
 
    #print board
    print "\nYour move"
    print_board()

    if(who_won() == PLAYER):
        print "Wow! You Won :o and you have found a logical glitch too :p"
        sys.exit(0)

    #calculate comp move
    flag = False
    comx = comy = -99
    for i in range(3):
        for j in range(3):
            if(board[i][j] == 0):
                board[i][j] = COMP
                who = play(0,0,PLAYER)
                board[i][j] = NONE
                if(who == COMP):
                    comx = i
                    comy = j
                    flag = True
                    break
                elif(who == DRAW):
                    comx = i
                    comy = j      
        if(flag == True):
            break
    if(comx == -99 and comy == -99):
        print "I won't lose :p"
        sys.exit()
    board[comx][comy] = COMP
    
    #print board
    print "\n",
    print "Comp move"
    print_board()
    
    if(who_won() == COMP):
        print "You lost."
        sys.exit(0)
    

