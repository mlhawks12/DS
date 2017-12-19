//
//  Champions.h
//  hw2code
//
//  Created by mike Levine on 1/29/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#ifndef __hw2code__Champions__
#define __hw2code__Champions__

#include <stdio.h>
#include <string>
using namespace std;
#endif /* defined(__hw2code__Champions__) */

class Champions
{
public:
    Champions(string aChampion);
    
    //ACCESSORS
    string getChampion() const { return champion; }
    int getWins() const { return wins; }
    int getLoses() const { return loses; }
    float getWin_Percent() const { return win_percent; }
    int getMinion_Deaths() const { return minion_deaths; }
    
    //MODIFIERS
    void incrementWins();
    void incrementLoses();
    void incrementMinion_Deaths();
    void calculateWin_Percent();
private:
    string champion;
    int wins;
    int loses;
    int minion_deaths;
    float win_percent;
        
};
bool to_Sort_Champ(const Champions& c1, const Champions& c2);