//
//  main.cpp
//  hw2code
//
//  Created by mike Levine on 1/28/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Players.h"
#include "Champions.h"
using namespace std;
/*
 Purpose:
        Finds the index of the given string in a vector of strings or finds that that string is not in the vector
 Arguments:
        champions- vector of strings that holds the champions for a specific player
        c1- a string that is a champion that you want to find the index of in champions or find out that it's not in champions
 returns:
        x- the index of c1 in champions 
        -1: if c1 is not in champions
*/
int find_Champion(const vector<string>& champions, const string& c1)
{
    for(unsigned int x = 0; x<champions.size();x++)
    {
        if(c1 == champions[x])
            return x;
    }
    return -1;
}
/*
 Purpose:
        Finds the index of the given string in a vector of Champions or finds that that string is not in the vector
 Arguments:
        champions- vector of strings that holds the champions for a specific player
        c1- a string that is a champion that you want to find the index of in champions or find out that it's not in champions
 returns:
        x- the index of c1 in champions
        -1: if c1 is not in champions
 */
int find_Champion(const vector<Champions>& champions,const string& c1)
{
    for(unsigned int x = 0; x<champions.size();x++)
    {
        if(c1 == champions[x].getChampion())
            return x;
    }
    return -1;
}
/*
Purpose:
       Finds the index of the given string in a vector of Players or finds that that string is not in the vector
Arguments:
        player- vector of strings that holds the players for a specific player
        c1- a string that is a players that you want to find the index of in players or find out that it's not in players
returns:
        x- the index of p1 in players
        -1: if p1 is not in players
 */
int find_player(const vector<Players>& players,const string& p1)
{
    for(unsigned int x = 0; x<players.size();x++)
    {
        if(p1 == players[x].getPlayer())
            return x;
    }
    return -1;
}
/*
Purpose:
        to parse the winning and losing teams into a vector of Players and Champions.
        to calculate the amount of wins and loses for each champion
Arguments: 
        input- the iftream for reading in from the input file
        player_vec- a vector of Players
        champ_vec- a vector of Champions
returns:
        void
*/
void parse_Teams(ifstream& input, vector<Players>& player_vec, vector<Champions>& champ_vec)
{
    string result;
    string player;
    string champion;
    input>>result;//The first instance in the input file of WINNING or LOSING
    input>>player;//skips the first instance of the word TEAM after reading
    bool check = false;//to check if LOSING comes before WINNING
    do
    {
    if(result == "WINNING" || player == "WINNING")
    {
        //If player is WINNING then that means WINNING came after LOSING and then skips TEAM
        if(player == "WINNING")
        {
            input>>player;
        }
        while(input>>player)//goes through the input file
        {
            //breaks the while loop when the WINNING team players are all parsed
            if(player == "LOSING")
            {
                break;
            }
            //breaks the while loop when we hit EVENTS signifying we are done parsing all the players on the LOSING team
            if(player == "EVENTS")
            {
                return;
            }
            input>>champion>>champion>>champion;//reads the champion for the specific player and skips the works between the tow
            //if player is not in the player_vec, then create a new Player object and put it in the vector
            if(find_player(player_vec,player) == -1)
            {
                Players p(player);
                player_vec.push_back(p);
                //if champion is not in the champ_vec, then create a new Champion and put it in the vector
                if(find_Champion(champ_vec, champion) == -1)
                {
                    Champions c(champion);
                    champ_vec.push_back(c);
                }
                //add the champion to the vector of strings in the players class to keep track of the champions that the player plays with
                player_vec[player_vec.size()-1].addChamp_vec(champion);
            }
            else
            {
                int index1 = find_player(player_vec, player);//finds index of player in player_vec or if it is not in it
                //if the champion is not in the vector of champion for the specific player, then add the champion
                if(find_Champion(player_vec[index1].champVector(), champion) == -1)
                {
                    player_vec[index1].addChamp_vec(champion);
                }
            }
            //finds the index of champion in champ_vec
            int index2 = find_Champion(champ_vec, champion);
            champ_vec[index2].incrementWins();
        }
    }
    
    if(result == "LOSING" || player == "LOSING")
    {
        check = true;
        //if the input of the file is LOSING, then LOSING is second in the file after WINNING
        if(player == "LOSING")
        {
            check = false;//so it wont go back up to WINNING
            input>>player;//skips TEAM
        }
        while(input>>player)
        {
            //breaks when done parsing the LOSING team and goes to the WINNING team
            if(player == "WINNING")
            {
                break;
            }
            //breaks when done parsing the LOSING team and goes back to main
            if(player == "EVENTS")
            {
                return;
            }
            input>>champion>>champion>>champion;//skips words between the player and champion and takes in the champion
            //checks if the player is not in the player_vec
            if(find_player(player_vec, player) == -1)
            {
                Players p(player);
                player_vec.push_back(p);
                if(find_Champion(champ_vec, champion) == -1)
                {
                    Champions c(champion);
                    champ_vec.push_back(c);
                }
                player_vec[player_vec.size()-1].addChamp_vec(champion);
            }
            else
            {
                int index1 = find_player(player_vec, player);
                if(find_Champion(player_vec[index1].champVector(), champion) == -1)
                {
                    player_vec[index1].addChamp_vec(champion);
                }
            }
            int index2 = find_Champion(champ_vec, champion);
            champ_vec[index2].incrementLoses();
        }
    }
    } while(check);//ends the loop when both WINNING and LOSING have been parsed
}
/*
Purpose:
        to parse the killers and the killed players in the EVENTS section of the input file
        to calculate the kills, deaths, and assists for each player
        to calculate the minion deaths of each champion
Arguments:
        input- the input stream
        player_vec- vector of Players passed by reference
        champ_vec- vector of Champions passed by reference
return:
        void
*/
void parse_Events(ifstream& input, vector<Players>& player_vec, vector<Champions>& champ_vec)
{
    string killer;
    string killed;
    int index1;
    bool check_minion = false;//to check if minion killed a player
    while(input>>killer)
    {
        //goes back to main when the file reachs END
        if(killer == "END")
        {
            return;
        }
        input>>killer>>killer;//skips to set killer to the player who killed another player
        if(killer == "minion")
        {
            check_minion = true;
        }
        else
        {
            //to find the index of the killer in the player_vec
            index1 = find_player(player_vec,killer);
            player_vec[index1].incrementKills();
        }
        while(input>>killed)
        {
            //goes through the line and skips all the strings before we read the string killed, then the next word is the player that was killed
            if(killed == "killed")
            {
                input>>killed;//skips killed to get to the player that was killed
                index1 = find_player(player_vec,killed);
                if(check_minion)//if a minion killed a player
                {
                    vector<string> vec_for_minion =player_vec[index1].champVector();//vector of strings of the current player
                    string dummy_champ = vec_for_minion[vec_for_minion.size()-1];//the string of the champion who was killed by the minion
                    int index2 = find_Champion(champ_vec, dummy_champ);//checks what the index of dummy_champ is in champ_vec
                    champ_vec[index2].incrementMinion_Deaths();
                }
                check_minion = false;
                player_vec[index1].incrementDeaths();
                break;
            }
            //used to go through the assist list, until the variable killed is not equal to "killed", all the strings between "[" anda "]" are players that assisted the kill
            else if(killed != "[" && killed != "]")
            {
                index1 = find_player(player_vec,killed);
                player_vec[index1].incrementAssist();
            }
            
        }
    }
}
/*
Purpose:
        to write the Players information into a file
Arguments:
        output- the output stream
        player_vec- a vector of Players
returns:
        void
*/
void to_Write_Players(ofstream& output, const vector<Players>& player_vec)
{
    output<<left<<setw(23)<<"PLAYER NAME"<<"KILLS"<<right<<setw(8)<<"DEATHS"<<setw(8)<<"KDR";
    output<<setw(26)<<"PLAYED WITH CHAMPION(S)"<<endl;
    for(unsigned int x = 0; x<player_vec.size(); x++)
    {
        size_t a = to_string(player_vec[x].getKills()).length();//finds the length of the string of the number of kills
        //I subtract 28 - a to take the length of the number out of the spaces alloted to allow the last digit of each number to end a the same place
        output<<left<<setw(28-int(a))<<player_vec[x].getPlayer();
        output<<right<<player_vec[x].getKills();
        output<<setw(8)<<player_vec[x].getDeath();
        a =to_string(player_vec[x].getDeath()).length();
        output<<setw(8)<<fixed<<setprecision(2)<<player_vec[x].getKDR();
        output<<"   ";
        vector<string> champ_s_vec = player_vec[x].champVector();
        sort(champ_s_vec.begin(), champ_s_vec.end());
        for(unsigned int y = 0; y<champ_s_vec.size(); y++)
        {
            output<<champ_s_vec[y];
            if(y<champ_s_vec.size()-1)
                output<<", ";
        }
        output<<endl;
    }
}
/*
Purpose:
        to write the Champions information into a file
Arguments:
        output- the output stream
        champion_vec- a vector of Champions
returns:
        void
*/
void to_Write_Champions(ofstream& output, const vector<Champions>& champ_vec)
{
    output<<left<<setw(20)<<"CHAMPION NAME"<<right<<setw(8)<<"WINS"<<setw(8)<<"LOSSES"<<setw(8)<<"WIN%"<<setw(16)<<"MINION DEATHS"<<endl;
    for(unsigned int x = 0; x<champ_vec.size(); x++)
    {
        size_t a = to_string(champ_vec[x].getWins()).length();
        output<<left<<setw(28-int(a))<<champ_vec[x].getChampion();
        output<<right<<champ_vec[x].getWins();
        output<<setw(8)<<champ_vec[x].getLoses();
        a = to_string(champ_vec[x].getLoses()).length();
        output<<setw(8)<<fixed<<setprecision(2)<<champ_vec[x].getWin_Percent();
        output<<setw(16)<<champ_vec[x].getMinion_Deaths()<<endl;
    }
    
}
/*
Purpose:
        to write the custom information into a file
Arguments:
        output- the output stream
        player_vec- a vector of Players
returns:
        void
*/
void to_Write_Custom(ofstream& output, const vector<Players>& player_vec)
{
    output<<left<<setw(21)<<"PLAYER NAME"<<right<<"ASSISTS"<<endl;
    for(unsigned int x = 0; x<player_vec.size(); x++)
    {
        size_t a = to_string(player_vec[x].getAssist()).length();
        output<<left<<setw(28-int(a))<<player_vec[x].getPlayer();
        output<<right<<player_vec[x].getAssist()<<endl;
    }
}
int main(int argc, const char * argv[]) {
    //makes sure there is the right number of arguments
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " file\n";
        return 1;
    }

    ifstream input(argv[1]);
    //makes sure file is good
    if (!input.good()) {
        cerr << "Can not open the input file " << argv[1] << "\n";
        return 1;
    }
    ofstream output(argv[2]);
    //makes sure file is good
    if (!output.good()) {
        cerr << "Can not open the output file " << argv[2] << "\n";
        return 1;
    }
    string type_of_output = argv[3];
    string id;
    vector<Players> player_vec;
    vector<Champions> champ_vec;
    //gets the id of the game
    while(input>>id>>id>>id)
    {
        parse_Teams(input, player_vec, champ_vec);
        parse_Events(input, player_vec, champ_vec);
    }
    if(type_of_output == "players")
    {
        //goes though and calculates the KDR
        for(unsigned int x = 0; x<player_vec.size(); x++)
            player_vec[x].calculateKDR();
        //sorts by players-KDR(highest,kills(Highest),deaths(lowests), alphabetically by player name
        sort(player_vec.begin(), player_vec.end(),to_Sort);
        to_Write_Players(output, player_vec);
    }
    if(type_of_output == "champions")
    {
        //goes throug champ_vec and calculate the Win percentage
        for(unsigned int x = 0; x<champ_vec.size(); x++)
            champ_vec[x].calculateWin_Percent();
        //sorts by champions-win percentage(highest),wins(highest),losses(lowest), name
        sort(champ_vec.begin(), champ_vec.end(), to_Sort_Champ);
        to_Write_Champions(output, champ_vec);
    }
    if(type_of_output == "custom")
    {
        //sorts by assists(highest), then name
        sort(player_vec.begin(), player_vec.end(),to_Sort_Assist);
        to_Write_Custom(output, player_vec);
    }
    return 0;
}
