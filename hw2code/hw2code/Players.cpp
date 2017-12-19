//
//  Players.cpp
//  hw2code
//
//  Created by mike Levine on 1/29/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//
#include <string>
#include <iostream>
#include <vector>
#include <stdio.h>
#include "Players.h"
using namespace std;
Players::Players(std::string aPlayer)//the constructor
{
    player = aPlayer;
    kills = 0;
    deaths = 0;
    KDR = 0;
    assist = 0;
}
void Players::incrementKills()
{
    kills+=1;
}

void Players::incrementDeaths()
{
    deaths+=1;
}

void Players::calculateKDR()
{
    if(deaths == 0)
        KDR = float(kills);
    else
        KDR = float(kills)/deaths;//cast to float to allow KDR to get the correct decimal because if int/int, KDR would have gotten the int representation of the number
}

void Players::addChamp_vec(string aChamp)
{
    champ_vec.push_back(aChamp);
}
void Players::incrementAssist()
{
    assist+=1;
}
/*
Purpose:
        to sort by players-KDR(highest),kills(Highest),deaths(lowests), alphabetically by player name
Arguments:
        p1- Player object
        p2- Player object
returns:
        bool
*/
bool to_Sort(const Players& p1, const Players& p2)
{
    if(p1.getKDR() > p2.getKDR())
       return true;
    if(p1.getKDR() == p2.getKDR() && p1.getKills() > p2.getKills())
        return true;
    if(p1.getKDR() == p2.getKDR() && p1.getKills() == p2.getKills() && p1.getDeath() < p2.getDeath())
        return true;
    if(p1.getKDR() == p2.getKDR() && p1.getKills() == p2.getKills() && p1.getDeath() == p2.getDeath() && p1.getPlayer() < p2.getPlayer())
        return true;
    return false;
}
/*
Purpose:
        to sort by assist(highest), and then names
Arguments:
        p1- Player object
        p2- Player object
returns:
        bool
*/
bool to_Sort_Assist(const Players& p1, const Players& p2)
{
    if(p1.getAssist() > p2.getAssist())
        return true;
    else if(p1.getAssist() == p2.getAssist() &&  p1.getPlayer() < p2.getPlayer())
        return true;
    return false;
}
    
    
    
    
    
