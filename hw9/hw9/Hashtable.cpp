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
/*
 ARGUMENTS:
    kmer- string that is k length representing a partial genome
PURPOSE:
    hashes the kmer to get the index to put in
RETURN:
    size_t that is the index within the range of 0- table_size-1
 */
size_t Hashtable::getHash(std::string kmer)
{
    //const size_t initial = 2166136261U;
    const size_t seed = 16777619;
    //unsigned long hash = initial;
    unsigned long hash = 2166136261;
    for(size_t i = 0; i < kmer.length(); i++)
    {
        hash = hash ^ (kmer[i]); //xor  the low 8 bits
        hash = hash * seed; // multiply by the seed number
    }
    return hash % table_size;
}

void Hashtable::linear_prob(std::string kmer, size_t& x)
{
    while(hash_table[x].first != "" && hash_table[x].first != kmer)
    {
        x++;
        if(x == hash_table.size())//goes back to start of vector if it hits end and didnt find it
            x = 0;
    }
}
/*
 ARGUMENTS:
    kmer- string of k length
    index- passed by reference to store the index where the kmer is found
PURPOSE:
    finds the index where the kmer is in the hash_table
 RETURNS:
    true if found, false if not
 */
bool Hashtable::find(std::string kmer, size_t& index)
{
    size_t hash = getHash(kmer);
    index = hash;
    if(hash_table[hash].first == kmer)
    {
        return true;
    }
    else
    {
        size_t x = hash;
        linear_prob(kmer, x);
        index = x;
        if(hash_table[x].first == kmer)
        {
            return true;
        }
    }
    return false;
}
void Hashtable::insert(std::string kmer, int index_sen)
{
    size_t hash = getHash(kmer);//hashes
    if(hash_table[hash].first == "")//if the initial hash index has nothing there,then add the kmer
    {
        hash_table[hash].first = kmer;
        (hash_table[hash].second).push_back(index_sen);
        unique_keys+=1;
    }
    else if(hash_table[hash].first == kmer)//if the kmer is at the hash index, then just push the index in the genome back
    {
        (hash_table[hash].second).push_back(index_sen);
    }
    else
    {
        
        
        //Colision--------------------
        size_t x = hash;
        linear_prob(kmer, x);
        if(hash_table[x].first == kmer)
        {
            (hash_table[x].second).push_back(index_sen);
        }
        else
        {
            hash_table[x].first = kmer;
            (hash_table[x].second).push_back(index_sen);
            unique_keys+=1;
        }
        //------
    }
}
void Hashtable::resize()
{
    table_size*=2;
    std::vector<std::pair<std::string, std::list<int> > > temp_new(table_size);
    std::vector<std::pair<std::string, std::list<int> > > temp_old = hash_table;
    hash_table = temp_new;
    //copys all the data in the old table to the new table
    for(int x = 0; x < temp_old.size(); x++)
    {
        size_t hash = getHash(temp_old[x].first);
        if(temp_new[hash].first != "" && temp_new[hash].first != temp_old[x].first)
            linear_prob(temp_old[x].first, hash);
        temp_new[hash] = temp_old[x];
    }
    hash_table = temp_new;
}




