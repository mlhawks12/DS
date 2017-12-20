//
//  Grid.h
//  hw6
//
//  Created by mike Levine on 3/13/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#ifndef __hw6__Grid__
#define __hw6__Grid__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
typedef unsigned int uint;
using namespace std;
class Grid
{
public:
    //CONSTRUCTORS & OPERATOR & DECONSTRUCTOR
    Grid(int aRow, int aCol);
    Grid operator=(Grid& g);
    bool operator==(Grid& g);
    //ACCESSSORS
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    //char** getArray() const { return data; }
    vector<vector<char> > getVec() const { return data; }
    string getValue() const { return value; }
    void print_grid() const;
    //MODIFIERS AND OTHER FUNCTIONS
    void calculateValue();
    void set(int pos, char letter);
    bool addWord(int x, int y, int dx, int dy, string word);
    //bool addWord(int x, int y, uint direction, string word);//returns true if word added
    bool not_addWord(int x, int y, int dx, int dy, string word);//checks if the words you do not want t be in the Grid are in the grid
    int findEmpty();//finds empty space, if none return -1
    bool checkSolution(); //checks  if it is a valid solution
private:
    int rows;
    int cols;
    //char** data;
    vector<vector<char> > data;
    string value;
};
ostream& operator<< (ostream& output, const Grid& g);
//bool equals(Grid& g1, Grid& g2);
bool equals(string& g1, string& g2);
#endif /* defined(__hw6__Grid__) */
