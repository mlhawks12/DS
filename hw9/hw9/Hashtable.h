//
//  Hashtable.h
//  hw9
//
//  Created by mike Levine on 4/17/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

#ifndef __hw9__Hashtable__
#define __hw9__Hashtable__

#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <list>
class Hashtable
{
public:
    Hashtable(int aTable_size);
    
    //ACCESSORS
    int getUnique_keys() const {return unique_keys; }
    int gettable_size()  const { return table_size; }
    std::vector<std::pair<std::string, std::list<int> > > getHashTable() const { return hash_table;}
    bool find(std::string kmer, size_t& index);
    
    //modifiers
    void insert(std::string kmer, int index);
    size_t getHash(std::string kmer);
    void linear_prob(std::string kmer, size_t& x);
    void resize();
    
    
private:
    std::vector<std::pair<std::string, std::list<int> > > hash_table;
    int table_size;
    int unique_keys;
};
#endif /* defined(__hw9__Hashtable__) */
