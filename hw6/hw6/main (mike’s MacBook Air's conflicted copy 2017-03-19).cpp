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
using namespace std;
typedef unsigned int uint;
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
        {
            break;
        }
    }
    return;
}
bool place_word(string word, int x, int y, int direct , Grid& g)
{
    int dx;
    int dy;
    get_direction(dx, dy, direct);
    if((dx != 0 || dy !=0) && g.addWord(x, y, dx,dy, word))
    {
        return true;
    }
    return false;
}
void createSolution(string word,vector<string>& add, vector<Grid>& sol, Grid g, int which_word, int position,int direction)
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
    Grid new_grid(g.getRows(),g.getCols());
    new_grid = g;
    for(;direction<9; direction++)
    {
        if(place_word(add[which_word], row_place, col_place, direction, new_grid))
        {
            which_word++;
            if(which_word != add.size())
                createSolution(add[which_word], add, sol, new_grid, which_word, 0, 1);
            else
                createSolution("BYYY", add, sol, new_grid, which_word, 0, 1);
            which_word--;
        }
    }
    position++;
    createSolution(add[which_word], add, sol, g, which_word, position, 1);
}
//direction must intially be 0
bool check_for_not(string word, vector<string>& not_add, Grid& g, int position, int direction)
{
    direction++;
    if(direction == 9)
        return false;
    if(word.length() == 1)
        return true;
    int start_x = position / g.getCols();
    int start_y = position % g.getCols();
    if(place_word(word, start_x, start_y, direction, g))
       return true;
    check_for_not(word, not_add, g, position, direction);
    return false;
}
int main(int argc, const char * argv[]) {
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
    cout<<col<<" "<<row<<endl;
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
    vector<Grid> solutions;
    vector<Grid> sol;
    Grid g1(row, col);
    g1.calculateValue();
    Grid g2(row, col);
    g2.calculateValue();
    g2.addWord(0,2,0,-1,add_words[0]);
    g2.addWord(1,0,0,1,add_words[1]);
    Grid g3(row,col);
    g3.addWord(0,0, 0, 1, add_words[0]);
    g3.addWord(1,0, 0, 1, add_words[1]);
    sol.push_back(g2);
    sol.push_back(g3);
    g2.print_grid();
    g3.print_grid();
    for(int x = 0; x<sol.size();x++)
    {
    for(int y = 0; y<(row*col); y++)
    {
        int start_x = y / g2.getCols();
        int start_y = y % g2.getCols();
        for(int count_not = 0; count_not<not_add.size(); count_not++)
        {
            if(g2.getVec()[start_x][start_y] == not_add[count_not][0] && check_for_not(word, not_add, g2, y, 0))
            {
                sol.erase(solutions.begin()+x);
                x--;
            }
        }
    }
    }
    cout<<sol.size()<<endl;
    createSolution(add_words[0],add_words, solutions, g1,0, 0,1);
    cout<<solutions.size()<< " SSSSSSSS" <<endl;
    for(uint x = 0; x<solutions.size(); x++)
        solutions[x].calculateValue();
    int count = 0;
    for(int x = 0; x<solutions.size(); x++)
    {
        for(int y = x+1; y<solutions.size(); y++)
        {
            if(solutions[x].getValue() == solutions[y].getValue())
            {
                solutions.erase(solutions.begin()+x);
                x--;
                count++;
            }
        }
    }
    cout<<solutions.size();
    for(int x = 0; x<solutions.size();x++)
    {
        for(int y = 0; y<(row*col); y++)
        {
            int start_x = y / col;
            int start_y = y % col;
            for(int count_not = 0; count_not<not_add.size(); count_not++)
            {
                if(solutions[x].getVec()[start_x][start_y] == not_add[count_not][0] && !check_for_not(word, not_add, solutions[x], y, 0))
                {
                    
                    cout<<" SHIIIIT " << solutions.size()<<endl;
                    solutions.erase(solutions.begin()+x);
                    cout<<" FUNK " << solutions.size()<<endl;
                    x--;
                }
            }
        }
    }
    cout<<count<<endl;
    for(uint x = 0; x<solutions.size(); x++)
    {
        solutions[x].print_grid();
        cout<<endl;
    }
    count = 0;
    for(int x = 0; x<solutions.size(); x++)
    {
        for(int y = x+1; y<solutions.size(); y++)
        {
            if(solutions[x].getValue() == solutions[y].getValue())
            {
                count++;
            }
        }
    }
    cout<<count<<" " <<solutions.size()<<endl;
    return 0;
}
