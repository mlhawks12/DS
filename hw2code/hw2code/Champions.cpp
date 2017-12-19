//
//  Champions.cpp
//  hw2code
//
//  Created by mike Levine on 1/29/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#include "Champions.h"
#include <string>
#include <stdio.h>
using namespace std;
Champions::Champions(string aChampion)//the constuctor
{
    champion = aChampion;
    wins = 0;
    loses = 0;
    minion_deaths = 0;
    win_percent = 0;
}

void Champions::incrementWins()
{
    wins+=1;
}

void Champions::incrementLoses()
{
    loses+=1;
}

void Champions::incrementMinion_Deaths()
{
    minion_deaths+=1;
}

void Champions::calculateWin_Percent()
{
    //so program does divide by zero
    if(loses == 0 && wins>=1)
        win_percent = 1.00;
    else if(wins == 0)
        win_percent = 0.00;
    else
        win_percent = float(wins)/(loses+wins);//cast to float so result is float, int/int would have been an int
}
/*
Purpose:
        to sort by champions-win percentage(highest),wins(highest),losses(lowest), name 
Arguments:
        c1- a Champion object
        c2- a Champion object
returns:
        bool
*/
bool to_Sort_Champ(const Champions& c1, const Champions& c2)
{
    if(c1.getWin_Percent() > c2.getWin_Percent())
        return true;
    if(c1.getWin_Percent() == c2.getWin_Percent() && c1.getWins() > c2.getWins())
        return true;
    if(c1.getWin_Percent() == c2.getWin_Percent() && c1.getWins() == c2.getWins() && c1.getLoses() < c2.getLoses())
        return true;
    if(c1.getWin_Percent() == c2.getWin_Percent() && c1.getWins() == c2.getWins() && c1.getLoses() == c2.getLoses() && c1.getChampion() < c2.getChampion())
        return true;
    return false;
}




