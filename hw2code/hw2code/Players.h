//
//  Players.h
//  hw2code
//
//  Created by mike Levine on 1/29/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#ifndef __hw2code__Players__
#define __hw2code__Players__
#endif /* defined(__hw2code__Players__) */
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
class Players
{
public:
    Players(string aPlayer);
    
    //ACCESSORS
    string getPlayer() const { return player; }
    int getKills() const {return kills; }
    int getDeath() const {return deaths; }
    float getKDR() const {return KDR; }
    int getAssist() const {return assist; }
    vector<string> champVector() const {return champ_vec; }
    
    //MODIFIERS
    void incrementKills();
    void incrementDeaths();
    void calculateKDR();
    void addChamp_vec(string aChamp);
    void incrementAssist();
    
private:
    string player;
    int kills;
    int deaths;
    int assist;
    float KDR;
    vector<string> champ_vec;
};
bool to_Sort(const Players& p1, const Players& p2);

bool to_Sort_Assist(const Players& p1, const Players& p2);