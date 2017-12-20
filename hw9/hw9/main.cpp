//
//  main.cpp
//  hw9
//
//  Created by mike Levine on 4/16/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include "Hashtable.h"
size_t hash(std::string sen, int table_size)
{
    unsigned long long sum = 0;
    for(int x = 0; x < sen.size(); x++)
    {
        sum+= int(sen[x])*pow(128,x) * 31;
        sum ^= sen[x];
    }
    return sum % table_size;
}
size_t hash_func(std::string word, int table_size){
    const size_t InitialFNV = 2166136261U;
    const size_t FNVMultiple = 16777619;
    unsigned long hash = InitialFNV;
    for(size_t i = 0; i < word.length(); i++)
    {
        hash = hash ^ (word[i]); //xor  the low 8 bits
        hash = hash * FNVMultiple; // multiply by the magic number
    }
    return hash % table_size;
}
int main(int argc, const char * argv[]) {
    
    std::string command;
    std::cin>>command;
    std::string filename;
    std::cin>>filename;
    std::ifstream input(filename);
    if (!input.good()) {
        std::cerr << "Can not open the input file " << filename << "\n";
        return 1;
    }
    std::string temp;
    std::string genome;
    while(input>>temp)
    {
        genome+=temp;
    }
    //std::cout<<genome<<std::endl;
    int table_size = 100;
    //std::cout<<genome.size()<<std::endl;
   
    std::cin>>command;
    if(command == "table_size")
    {
        std::cin>>table_size;
        std::cin>>command;
    }
    
    float occupancy = 0.5;
    if(command == "occupancy")
    {
        std::cin>>occupancy;
        std::cin>>command;
    }
    int kmer_size = 10;
    std::cin>>kmer_size;
    Hashtable h1(table_size);
    //goes through the genome and adds all the kmers to the hash_table
    for(int x = 0; x < genome.size()-(kmer_size-1); x++)
    {
        std::string t = "";
        for(int y = x; y < x+kmer_size; y++)
        {
            t+=genome[y];//gets the kmer out of the genome
            
        }
        h1.insert(t, x);
        if((float(h1.getUnique_keys()))/h1.gettable_size() > occupancy)
           {
               h1.resize();
           }
        //std::cout<<t << " " << h1.getHash(t)<<std::endl;
    }
    std::vector<std::pair<std::string, std::list<int> > > temp_vec = h1.getHashTable();
    
    
    
    //to parse the query
    while(std::cin>>command)
    {
        
        if(command == "query")
        {
            int diff = 0;
            std::string blast;
            std::cin>>diff>>blast;
            std::string kmer = blast.substr(0,kmer_size);//kmer in the blast
            size_t index;
            bool check = h1.find(kmer, index);//finds the kmer in the hash_table
            bool check2 = false;
            if(check)
            {
                std::list<int>::iterator itr = (temp_vec[index].second).begin();
                std::cout<<"Query: " << blast<<std::endl;
                
                //iterates through to find the differences in the blast
                for(; itr!= temp_vec[index].second.end(); itr++)
                {
                    
                    int diff_count = 0;//stores the differences in the blast from the one in the genome
                    size_t long_str = *itr;
                    std::string temp_blast;
                    for(int str_index = 0; str_index < blast.size(); str_index++, long_str++)
                    {
                        if(blast[str_index] != genome[long_str])//if char in blast is equal to one in genome sequence
                        {
                            diff_count++;
                        }
                        temp_blast+=genome[long_str];//creates the string from the genome 
                    }
                    if(diff_count <= diff)
                    {
                        std::cout<<*itr <<" "<< diff_count << " " << temp_blast<<std::endl;
                        check2 = true;
                    }
                }
                if(!check2)
                    std::cout<<"No Match"<<std::endl;
            }
        }
        
        if(command == "quit")
        {
            break;
        }
    }
    return 0;
}
