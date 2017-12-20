//
//  main.cpp
//  hw6
//
//  Created by mike Levine on 3/11/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Grid.h"
#include <list>
using namespace std;
typedef unsigned int uint;
/* 
 ARGUMENTS:
    dx- empty variable to later store the change in x
    dy- empty variable to later store the change in y
    direction- number 1-8 for the direction that we want to get the dx, dy for
 PURPOSE:
    switch to quickly get dx and dy from position
 RETURNS:
    void
*/
void get_direction(int& dx, int& dy, int direction)
{
    switch(direction)
    {
        case 1:
            dx = 0;
            dy = 1;
            break;
        case 2:
            dx = 1;
            dy = 0;
            break;
        case 3:
            dx = 1;
            dy = 1;
            break;
        case 4:
            dx = -1;
            dy = 0;
            break;
        case 5:
            dx = 0;
            dy = -1;
            break;
        case 6:
            dx = -1;
            dy = 1;
            break;
        case 7:
            dx = 1;
            dy = -1;
            break;
        case 8:
            dx = -1;
            dy = -1;
            break;
        default:
            break;
    }
    return;
}
/*
ARGUMENTS- 
    word- string that you want to place in the Grid
    x- row you want to place the first letter of the board
    y- col you want to place the first letter of the board
    direct- direction to place the word
    g- a Grid
    check- true if we are trying to place a "+" word and false if we are trying to see if a "-" word is in the Gird
 PURSPOSE:
    helper function to call add words
 RETURN:
    true if it can add the word, false otherwise
 */
bool place_word(string& word, int x, int y, int direct , Grid& g, bool check)
{
    int dx;
    int dy;
    get_direction(dx, dy, direct);
    if((dx != 0 || dy !=0) && check && g.addWord(x, y, dx,dy, word))
        return true;
    else if((dx != 0 || dy !=0) && g.not_addWord(x, y, dx, dy, word))
        return true;
    return false;
}
/*
 ARGUMENTS:
    word- a string that we want to check if it is in the Grid
    g- a Grid
    position- a number 0- (row*cols)-1 that represents the location in the Grid the first letter of the word your checking if it is in the Grid is
    direction- a number 1-8 that represents all directions that a word can be added, intial call to this function this number should be 0
 PURPOSE:
    recursively finds if the word you're looking for is in the Grid, already has location of a the first letter of the word you are looking for potential
 RETURNS:
    bool- true if the word is in the Grid, false otherwise
 */
bool check_for_not(string word, Grid& g, int position, int direction)
{
    direction++;
    if(direction == 9)
        return false;
    if(word.length() == 1)
        return true;
    int start_x = position / g.getCols();
    int start_y = position % g.getCols();
    if(place_word(word, start_x, start_y, direction, g, false))
        return true;
    return check_for_not(word, g, position, direction);
    //return check;
}
/*
 ARGUMENTS:
    word- the string you want to place in the Grid
    add- vector containing the words you want to add to the Grid
    sol- list of Grids containing all solutions
    g- a Grid
    which_word- int representing the index for the add vector to indicate the current word to add
    position- a number 0- (row*cols)-1 to represent where the first character of the word should be place if able to
    direction- a number 1-8 that represents that direction the word should be placed
PURPOSE:
    recursively creates all possible solutions of Grids from adding the words to add
 RETURN:
    void
 
 */
void createSolution(string& word, vector<string>& add, list<Grid>& sol, Grid g, int which_word, int position,int direction)
{
    int row_place = position / g.getCols();
    int col_place = position % g.getCols();
    if(position >= g.getRows()*g.getCols())
    {
        return;
    }
    if(which_word == add.size())
    {
        sol.push_back(g);
        return;
    }
    for(;direction<9; direction++)
    {
        Grid new_grid(g.getRows(),g.getCols());
        new_grid = g;
        if(place_word(add[which_word], row_place, col_place, direction, new_grid, true))
        {
            which_word++;
            if(which_word != add.size())
            {
                createSolution(add[which_word], add, sol, new_grid, which_word, 0, 1);
            }
            else
            {
                string w = "BYYYE BYYYE";//when there is a correct solution, sends a dumby string
                createSolution(w, add, sol, new_grid, which_word, 0, 1);
            }
            which_word--;
        }
    }
    position++;
    createSolution(add[which_word], add, sol, g, which_word, position, 1);
}
//direction must intially be 0
/* 
 ARGUMENTS:
    not_add- vector containing strings that should not be in the grid
    solutions- list of Grids containing all solutions
 PURPOSE:
    iterates through the solutions list and the not_add vector and calls check_for_not to find if a solution is valid, if solution is invalid, it erases the solution form solutions
 RETURNS:
    void
 */
void erase_invalid(const vector<string>& not_add, list<Grid>& solutions)
{
    for(list<Grid>::iterator itr = solutions.begin(); itr != solutions.end(); itr++)
    {
        bool check = false;
        for(int y = 0; y<((*itr).getRows()*(*itr).getCols()); y++)
        {
            int start_x = y / (*itr).getCols();
            int start_y = y % (*itr).getCols();
            for(int count_not = 0; count_not<not_add.size(); count_not++)
            {
                if((*itr).getVec()[start_x][start_y] == not_add[count_not][0] && check_for_not(not_add[count_not], (*itr), y, 0))
                {
                    itr = solutions.erase(itr);
                    if(itr != solutions.begin())
                    {
                        itr--;
                    }
                    check = true;
                    break;
                }
            }
        }
    }
    for(int y = 0; y<((*solutions.begin()).getRows()*(*solutions.begin()).getCols()); y++)
    {
        int start_x = y / (*solutions.begin()).getCols();
        int start_y = y % (*solutions.begin()).getCols();
        for(int count_not = 0; count_not<not_add.size(); count_not++)
        {
            if(((*solutions.begin()).getVec()[start_x][start_y] == not_add[count_not][0] && check_for_not(not_add[count_not], (*solutions.begin()), y, 0)))
            {
                solutions.erase(solutions.begin());
                return;
            }
        }
    }
}
void remove_duplicates(list<Grid>& sol){
    for(list<Grid>::iterator itr = sol.begin(); itr != sol.end(); itr++)
    {
        for(list<Grid>::iterator itr2 = sol.begin(); itr2 != sol.end(); itr2++)
        {
            if(itr != itr2 && (*itr) == (*itr2))
            {
                itr = sol.erase(itr);
            }
        }
    }
}
//solutions should be empty when sent intially
/*
 ARGUMENT:
    g- Grid object 
    sol- list of all solutions, sol should be initially empty when called from main
    alphabet- array of char represnting the alphebet
    positon- numbber 0 - (row*col)-1 to represent the position
 PURPOSE:
    recursively fills all empty spaces with every possible combination of characters
 RETURNS:
    void
 */
void placeLetters(Grid g, list<Grid>& sol, char alphabet [], int position)
{
    
    int pos = g.findEmpty();//finds the position of the first empty space
    if(pos == -1)//if no empty space left
    {
        sol.push_back(g);
        return;
    }
    for(int i = 0; i < 26; i ++)
    {
        g.set(pos,alphabet[i]);
        position = pos;
        placeLetters(g, sol, alphabet, pos);
    }
}
/*
 ARGUMENTS:
    out- ostream to output
    sol- list of solutions
PURPOSE:
    to output to file
RETURN:
    void
 */
void outputFunction(ostream& out,list<Grid>& sol, string type)
{
    if(sol.size() == 0)
        out<<"No solutions found"<<endl;
    else if(type == "one_solution")
    {
        out<<"Board:"<<endl;
        out<<(*sol.begin())<<endl;
    }
    else
    {
        out<<sol.size()<<" solution(s)"<<endl;
        for(list<Grid>::iterator itr = sol.begin(); itr != sol.end(); itr++)
        {
            out<<"Board:"<<endl;
            out<<(*itr)<<endl;
        }
    }
}
int main(int argc, const char * argv[]) {
    char alpha[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " grid-file\n";
        return 1;
    }
    ifstream input(argv[1]);
    if (!input.good()) {
        cerr << "Can not open the input file " << argv[1] << "\n";
        return 1;
    }
    ofstream output(argv[2]);
    if (!output.good()) {
        cerr << "Can not open the output file " << argv[1] << "\n";
        return 1;
    }
    string one_or_all = argv[3];
    int row = 0;
    int col = 0;
    input>>col;
    input>>row;
    vector<string> add_words;
    vector<string> not_add;
    string word;
    string oper;
    while(input>>oper>>word)
    {
        if(oper == "+")
            add_words.push_back(word);
        else if(oper == "-")
        {
            not_add.push_back(word);
        }
    }
    list<Grid> solutions;
    list<Grid> sol;
    Grid g1(row, col);
    g1.calculateValue();
    createSolution(add_words[0],add_words, solutions, g1,0, 0,1);
    for(list<Grid>::iterator itr = solutions.begin(); itr != solutions.end(); itr++)
        (*itr).calculateValue();
    remove_duplicates(solutions);
    erase_invalid(not_add, solutions);
    for(list<Grid>::iterator itr = solutions.begin(); itr != solutions.end(); itr++)
        placeLetters((*itr), sol, alpha,-1);
    erase_invalid(not_add, sol);
    remove_duplicates(sol);
    for(list<Grid>::iterator itr = sol.begin(); itr != sol.end(); itr++)
        (*itr).calculateValue();
    outputFunction(output, sol, one_or_all);
    return 0;
}
