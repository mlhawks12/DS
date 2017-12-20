//
//  Hashtable.cpp
//  hw9
//
//  Created by mike Levine on 4/17/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#include "Hashtable.h"
#include <iostream>
Hashtable::Hashtable(int aTable_size)
{
    table_size = aTable_size;
    std::vector<std::pair<std::string, std::list<int> > > temp(table_size);
    hash_table = temp;
    int unique_keys = 0;
}
size_t Hashtable::getHash(std::string kmer)
{
    unsigned long long sum = 0;
    for(int x = 0; x < kmer.size(); x++)
    {
        sum += (int(kmer[x])*pow(128,x)*31)/51;
        sum ^=  (kmer[x]);
    }
    return sum % table_size;
}
bool Hashtable::linear_prob(std::string kmer, size_t& x)
{
    while(hash_table[x].first != "" && hash_table[x].first != kmer)
    {
        //std::cout<<"HHHHHHHHHH"<<std::endl;
        x++;
        if(x == hash_table.size()-1)
            x = 0;
    }
    if(hash_table[x].first != "")
        return true;
    return false;
}
void Hashtable::insert(std::string kmer, int index_sen)
{
    size_t hash = getHash(kmer);
    if(hash_table[hash].first == "")
    {
        std::cout<<1111111<<std::endl;
        hash_table[hash].first = kmer;
        (hash_table[hash].second).push_back(index_sen);
        unique_keys+=1;
    }
    else if(hash_table[hash].first == kmer)
    {
        std::cout<<2222222<<std::endl;
        (hash_table[hash].second).push_back(index_sen);
    }
    else
    {
        size_t x = hash;
        linear_prob(kmer, x);
        if(hash_table[x].first == kmer)
        {
            std::cout<<333333<<std::endl;
            (hash_table[x].second).push_back(index_sen);
        }
        else
        {
            hash_table[x].first = kmer;
            (hash_table[x].second).push_back(index_sen);
            unique_keys+=1;
            std::cout<<hash_table[x].first<<std::endl;
        }
    }
    if(kmer == "TATTACTGCC")
    {
        int c;
        std::cin>>c;
    }
}
void Hashtable::resize()
{
    table_size*=2;
    std::vector<std::pair<std::string, std::list<int> > > temp_new(table_size);
    std::vector<std::pair<std::string, std::list<int> > > temp_old = hash_table;
    hash_table = temp_new;
    bool check = false;
    for(int x = 0; x < temp_old.size(); x++)
    {
        size_t hash = getHash(temp_old[x].first);
        if(temp_new[hash].first != "" && temp_new[hash].first != temp_old[x].first)
            linear_prob(temp_old[x].first, hash);
        temp_new[hash] = temp_old[x];
    }
    hash_table = temp_new;
}




