//
//  Grid.cpp
//  hw6
//
//  Created by mike Levine on 3/13/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#include "Grid.h"
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
using namespace std;
//constructor
Grid::Grid(int aRow, int aCol)
{
    rows = aRow;
    cols = aCol;
    for(uint x = 0; x< rows; x++)
    {
        vector<char> temp;
        for(uint y = 0; y < cols; y++)
            temp.push_back('-');
        data.push_back(temp);
    }
}
bool Grid::operator==(Grid& g)
{
    for(int x = 0; x<rows; x++)
    {
        for(int y = 0; y < cols; y++)
        {
            if(this->data[x][y] != g.data[x][y])
                return false;
        }
    }
    return true;
}
bool equals(string g1, string g2)//checks if 2 strings are equal
{
    if(g1 == g2)
        return true;
    return false;
}
void Grid::calculateValue()//calculates the value of a Grid
{
    for(int x = 0; x<(rows*cols)-1; x++)
    {
        int pos_x = x/cols;
        int pos_y = x%rows;
        value.append(1,data[pos_x][pos_y]);
    }
}
/*
 ARGUMENTS:
    pos- a positons to add a letter
    letter- char to add at pos
 PURPOSE:
    to set a blank space to a desired letter
RETURNS:
    void
 */
void Grid::set(int pos, char letter)
{
    int row = pos/cols;
    int col = pos%cols;
    data[row][col] = letter;
}
/*
 ARGUMENTS:
    x- row to add the first letter of the word
    y- col to add the first letter of the word
    dx- change in x 
    dy - change in y
    word- string to add to grid
 PURPOSE:
    adds the word to the Grid if it can
 RETURNS:
    true if can add, false otherwise
 */
bool Grid::addWord(int x, int y, int dx, int dy, string word)
{
    int end_x = x + (int(word.length()-1))*dx;
    int end_y = y + (int(word.length())-1)*dy;
    if(end_x >= rows || end_x <0 ||end_y >= cols || end_y <0)
        return false;
    int word_c = 0;
    uint j = y;
    for(uint i = x; word_c < word.size(); i+=dx, j+=dy, word_c++)
        if(data[i][j] != '-' && word[word_c] != data[i][j])
            return false;
    word_c = 0;
    j = y;
    for(uint i = x; word_c< word.size(); i+=dx, j+=dy, word_c++)
        data[i][j] = word[word_c];
    return true;
}
/*
 ARGUMENTS:
 x- row to add the first letter of the word
 y- col to add the first letter of the word
 dx- change in x
 dy - change in y
 word- string to add to grid
 PURPOSE:
 adds the word to the Grid if it can
 RETURNS:
 true if can add, false otherwise
 */
bool Grid::not_addWord(int x, int y, int dx, int dy, string word)
{
    int end_x = x + (int(word.length()-1))*dx;
    int end_y = y + (int(word.length())-1)*dy;
    if(end_x >= rows || end_x <0 ||end_y >= cols || end_y <0)
        return false;
    int word_c = 0;
    uint j = y;
    for(uint i = x; word_c < word.size(); i+=dx, j+=dy, word_c++)
        if(word[word_c] != data[i][j])
            return false;
    word_c = 0;
    j = y;
    for(uint i = x; word_c< word.size(); i+=dx, j+=dy, word_c++)
        data[i][j] = word[word_c];
    return true;
}
//finds an empty space in the Grid
int Grid::findEmpty()
{
    for(int x = 0; x<(rows*cols); x++)
    {
        int pos_x = x/cols;
        int pos_y = x%cols;
        if(data[pos_x][pos_y] == '-')
            return x;
    }
    return -1;
}
//sets one Grid equal to the other
Grid Grid::operator=(Grid& g)
{
    this->rows = g.rows;
    this->cols = g.cols;
    this->data = g.data;
    this->value = "";
    return *this;
}
//overloaded << operator for output to text file
ostream& operator<< (ostream& out, const Grid& g)
{
    for(uint x = 0; x<g.getRows(); x++)
    {
        out<<"  ";
        for(uint y  = 0; y<g.getCols(); y++)
        {
            out<<g.getVec()[x][y];
        }
        if(x<g.getRows()-1)
            out<<endl;
    }
    return out;
}
//used for debugging, prints out a grid
void Grid::print_grid() const
{
    for(uint x = 0; x<rows; x++)
    {
        for(uint y  = 0; y<cols; y++)
        {
            cout<<data[x][y]<< " ";
        }
        cout<<endl;
    }
}