// Main.cpp for MINIblast homework
///////////////////////////////// INCLUDES
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Myhash.h"

///////////////////////////////// FUNCTIONS
int count_diff(std::string s1, std::string s2){
    // assumes both strings are the same length (they should)
    int num_diff = 0;
    for(unsigned int i=0; i<s1.size(); i++){
        if(s1[i] != s2[i])
            num_diff++;
    }
    return num_diff;
}

///////////////////////////////// MAIN
int main(){
    std::string cmd, fname;
    int k;
    int table_size = 100;
    float occupancy = 0.5;
    // assumes the first command is always genome
    std::cin >> cmd >> fname;
    // reading in initial info to build hash table
    while(std::cin >> cmd){
        if(cmd=="kmer") {
            std::cin >> k;
            break;
        }
        else if(cmd=="table_size") {std::cin >> table_size;}
        else if(cmd=="occupancy") {std::cin >> occupancy;}
    }
    
    // Reading in the looooooooong genome string
    std::ifstream instr(fname.c_str());
    std::string genome;
    std::string gen_part;
    // literally just concatenating line by line
    while(instr >> gen_part){
        genome += gen_part;
    }
    
    // Initializing the hash table
    Myhash hash_table (table_size, occupancy);
    // Adding all the kmers in the genome string to the hash table
    for(unsigned int ind =0; ind < (genome.size() - k); ind++){
        hash_table.add(genome.substr(ind, k), ind);
    }
    
    // initializing variables before the query loop
    int max_diff;
    std::string q_seq;
    std::vector<int> pos_v;
    // query loop
    while(std::cin >> cmd){
        if(cmd=="quit") break;
        // assumes only other possible command is query (after the above ones)
        std::cin >> max_diff >> q_seq;
        std::cout << "Query: " << max_diff << " " << q_seq << std::endl;
        if(hash_table.find_kmer(q_seq.substr(0,k), pos_v)){
            int total_diff;
            std::string g_seq;
            for(unsigned int i=0; i<pos_v.size(); i++){
                // counting the total number of differences
                g_seq = genome.substr(pos_v[i], pos_v[i] + q_seq.size()-1);
                total_diff = count_diff(q_seq, g_seq);
                // if the number is within the allowed limit
                if(total_diff <= max_diff){
                    std::cout << pos_v[i] << " " <<  total_diff << " " << g_seq;
                }
                else{
                    std::cout << "No Match" << std::endl;
                }
            }
        }
        else{
            std::cout << "No Match" << std::endl;
        }
    }
    
    return 0;
}
