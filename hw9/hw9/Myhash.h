#include <vector>
#include <string>
#include <utility>

typedef std::pair< std::string, std::vector<int> >  hashpair;
typedef std::vector< hashpair > hashvect;

class Myhash{
public:
    Myhash();
    Myhash(int _table_size, float _max_occ);
    void add(const std::string& kmer, const unsigned int s_ind);
    bool find_kmer(const std::string& kmer, std::vector<int>& pos_vector) const;
private:
    hashvect data;
    int size;
    int unique;
    float max_occ;
    unsigned int str_hash(const std::string& s) const;
    void resize();
    int linear_probe( int hashind, const std::string& kmer) const;
    int probe_for_kmer(const unsigned int hashind, const std::string& kmer) const;
    
};


Myhash::Myhash(int _size, float _max_occ){
    size = _size;
    max_occ = _max_occ;
    unique = 0;
    // making the initial data
    std::vector<int> init_vec;
    hashpair init_pair = std::make_pair("EMPTY", init_vec);
    for(int i=0; i<size; i++){
        data.push_back(init_pair);
    }
}

unsigned int Myhash::str_hash(const std::string& s) const{
    /*unsigned long long hash = 101;
    for (unsigned int i=0; i<s.size(); i++){
        hash = ((hash << 5) + hash) + s[i];
    }
    std::cout<<hash<< " hash in hash function"<<std::endl;
    return hash%size;*/
    const size_t InitialFNV = 2166136261U;
    const size_t FNVMultiple = 16777619;
    unsigned long hash = InitialFNV;
    for(size_t i = 0; i < s.length(); i++)
    {
        hash = hash ^ (s[i]); //xor  the low 8 bits
        hash = hash * FNVMultiple; // multiply by the magic number
    }
    std::cout<< hash<< "    jjjj   "<<hash%size <<"    " <<size<< " hash imm hash function " <<std::endl;
    return hash % size;
}
int Myhash::linear_probe(int hashind, const std::string& kmer) const{
    // check for next available spot
    for(int i=hashind+1; i<data.size(); i++){
        if(data[i].first == "EMPTY"||data[i].first == kmer){
            return i;
        }
    }
    // if reached end and not found yet, check for spots in beginning
    for(int i=0; i<hashind; i++){
        if(data[i].first == "EMPTY"||data[i].first == kmer){
            return i;
        }
    }
    return -1;
    /*
    std::cout<<hashind<<std::endl;
    while(data[hashind].first != "EMPTY" && data[hashind].first != kmer)
    {
        hashind++;
        if(hashind == data.size())
            hashind = 0;
        std::cout<<"FUCKKKKKKKKKKK"<<std::endl;
    }
    return hashind;
     */
}

int Myhash::probe_for_kmer(const unsigned int hashind, const std::string& kmer) const{
    // check next spots for kmer, if blank space found, it means kmer
    // isnt there bc otherwise it would have been linear-probed there
    for(int i=hashind+1; i<data.size(); i++){
        if(data[i].first == "EMPTY")
            return -1;
        else if(data[i].first == kmer)
            return i;
    }
    // if reached end and not found kmer or empty spot yet,
    // check for kmer/empties in beginning
    for(int i=0; i<hashind; i++){
        if(data[i].first == "EMPTY")
            return -1;
        else if(data[i].first == kmer)
            return i;
    }
    return -1;
}

void Myhash::add(const std::string& kmer, const unsigned int s_ind){
    unsigned int hashind = str_hash(kmer);
    // CASE: No data stored in hashed ind yet
    std::cout<<"222222"<<std::endl;
    if(data[hashind].first == "EMPTY"){
        std::cout<<"findme 111111111"<<std::endl;
        data[hashind].first = kmer;
        (data[hashind].second).push_back(s_ind);
        unique++; // indicating another unique space has been filled
    }
    // CASE: kmer already in hashed index
    else if(data[hashind].first == kmer){
        std::cout<<"findme 2222222"<<std::endl;
        // adding current s_ind to kmer's s_ind vector
        (data[hashind].second).push_back(s_ind);
        // no new spots have been filled
    }
    // CASE: hashed index contains data for another kmer
    else{
        std::cout<<"findme 333333333"<<std::endl;
        unsigned int new_hashind = linear_probe(hashind, kmer);
        std::cout<<"1111111111 HIIIIIIIIII"<<std::endl;
        // Actually adding the entry now
        data[new_hashind].first = kmer;
        std::cout<<"2222222222 FUCK"<<new_hashind<< "real hash "<<hashind<<std::endl;
        data[new_hashind].second.push_back(s_ind);
        std::cout<<"333333333333 SHIT"<<std::endl;
        unique++; // indicating another unique space has been filled
    }
    std::cout<<"H"<<std::endl;
    // -- Checking if table needs to be resized
    if((1.0*unique/size) > max_occ){
        resize();
    }
}

void Myhash::resize(){
    // Storing the old hash table
    std::cout<<"11111111"<<std::endl;
    hashvect old_data = data;
    // Incrementing size & making new blank hash table
    size = size * 2;
    hashvect new_data (size);
    data = new_data;
    // clearing uniques
    unique = 0;
    // Looping through old data, rehashing & adding to new data
    for(unsigned int i=0; i < old_data.size(); i++){
        // for every entry in the hash table
        if(old_data[i].first != "EMPTY"){
            // getting new hashi index for kmer
            unsigned int hashind = str_hash(old_data[i].first);
            // if collision, linear probe to find next available spot
            if(data[hashind].first != "EMPTY"){
                hashind = linear_probe(hashind, old_data[i].first);
            }
            // adding entry to new hash table
            std::cout<<i<< "i index "<<hashind<< "     d ddd     " <<size<<" hashind printing out"<<std::endl;
            data[hashind] = old_data[i];
            //data[hashind].second = old_data[i].second;
        }
    }
}


bool Myhash::find_kmer(const std::string& kmer, std::vector<int>& pos_vector) const{
    unsigned int hashind = str_hash(kmer);
    // CASE: kmer not in index (means kmer not in hash table)
    if(data[hashind].first == "EMPTY"){
        return false;
    }
    // CASE: kmer found in hashed index
    else if(data[hashind].first != "EMPTY" && data[hashind].first != kmer){
        unsigned int new_hashind = probe_for_kmer(hashind, kmer);
        if(new_hashind==-1)
            return false;
        else
            hashind = new_hashind;
    }
    // setting pos_vector to vector of starting inds of kmer within genome
    pos_vector = data[hashind].second;
    return true;
}