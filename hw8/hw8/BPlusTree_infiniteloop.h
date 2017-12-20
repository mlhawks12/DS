/* ASSUMPTIONS:
 1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
 2. There will never be a duplicate key passed to insert.
 */

#include <vector>
#include <iostream>
#include <fstream>

#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode{
public:
    BPlusTreeNode() : parent(NULL) {};
    bool is_leaf();
    bool contains(const T& key);
    
    //For grading only, this is actually bad to have because
    //it exposes a private member variable.
    BPlusTreeNode* get_parent() { return parent; }
    
    //We need this to let BPlusTree access private members
    friend class BPlusTree<T>;
private:
    bool contains(const T& key,std::size_t low,std::size_t high);
    std::vector<T> keys;
    std::vector<BPlusTreeNode*> children;
    BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
    for(unsigned int i=0; i<children.size(); i++){
        if(children[i]){
            return false;
        }
    }
    return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
    return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
    if(low>high){
        return false;
    }
    if(low==high){
        return key == keys[low];
    }
    std::size_t mid = (low+high)/2;
    if(key<=keys[mid]){
        return contains(key,0,mid);
    }
    else{
        return contains(key,mid+1,high);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree{
public:
    BPlusTree(int aDegree): degree(aDegree), root(NULL) {};
    BPlusTree(const BPlusTree<T>* tree);
    void operator=(const BPlusTree* b);
    BPlusTreeNode<T>* copy(const BPlusTreeNode<T>* other);
    
    void print_sideways(std::ofstream& outfile);
    void print_BFS(std::ofstream& outfile);
    
    void insert(const T& val);
    
    
    BPlusTreeNode<T>* getRoot() const {return root; }
    BPlusTreeNode<T>* find(const T& val);
    
    
    //BPlusTreeNode<T>* splitChild(BPlusTreeNode<T>* temp, T& val);
    void splitChild(BPlusTreeNode<T>* temp);
    void splitRoot();
    
    void pop(BPlusTreeNode<T>* temp, const T& val);
    
    friend class BPlusTreeNode<T>;
private:
    int degree;
    BPlusTreeNode<T>* root;
};
/*
 template <class T> std::vector<T> std::vector<T>::operator=(const std::vector<T>& vec1)
 {
 for(int x = 0; x < vec1.size(); x++)
 {
 *this.push_back(vec1[x]);
 }
 }*/

template <class T> BPlusTree<T>::BPlusTree(const BPlusTree<T>* tree)
{
    this->root = copy(tree->root);
    this->degree = tree->degree;
}
template <class T> BPlusTreeNode<T>* copy(const BPlusTreeNode<T>* other) {
    if(!other) return NULL;
    BPlusTreeNode<T>* new_node = new BPlusTreeNode<T>;
    new_node->parent = other->parent;
    for (unsigned int i=0; i<other->keys.size(); i++)
        new_node->keys.push_back(other->keys[i]);
    for (unsigned int j=0; j<other->children.size(); j++)
        new_node->children.push_back(copy(other->children[j]));
    return new_node;
}
template <class T> void BPlusTree<T>::operator=(const BPlusTree<T>* b)
{
    this->root = copy(b->root);
    this->degree = b->degree;
}
template <class T> BPlusTreeNode<T>* BPlusTree<T>::find(const T& val)
{
    std::cout<<"XXXXXXXXXXXaaaaXXXXXXXXXX"<<std::endl;
    BPlusTreeNode<T>* temp = root;
    BPlusTreeNode<T>* next_temp = root;
    int nxt_index = 0;
    T check_val;
    std::cout<<val<<" val "<<std::endl;
    if(!temp)
    {
        return root;
    }
    if(temp->is_leaf())
    {
        return root;
    }
    while(!next_temp->is_leaf())
    {
        unsigned int x = 0;
        for(; x < temp->keys.size(); x++)
        {
            //std::cout<<temp->keys[<<" " <<val<<" temp->keys.size() "<<std::endl;
            check_val = temp->keys[x];
            if(val < check_val)
            {
                nxt_index = x;
            }
        }
        next_temp = temp->children[nxt_index];
    }
    temp = next_temp;
    std::cout<<val<<" val "<<std::endl;
    for(int x = 0; x< temp->keys.size(); x++)
    {
        std::cout<<temp->keys[x]<< " ";
    }
    std::cout<<std::endl;
    std::cout<<"XXXXXXXXXXXXXXXXXXXXX"<<std::endl;
    return temp;
}
template <class T> void BPlusTree<T>::insert(const T& val)
{
    BPlusTreeNode<T>* temp = root;
    temp = find(val);
    if(temp == root)
    {
        if(!temp)
        {
            std::cout<<"FUCK"<<std::endl;
            temp = new BPlusTreeNode<T>;
            BPlusTreeNode<T>* null_child1 = NULL;
            BPlusTreeNode<T>* null_child2 = NULL;
            temp->keys.push_back(val);
            temp->children.push_back(null_child1);
            temp->children.push_back(null_child2);
            root = temp;
        }
        else
        {
            //temp->keys.push_back(val);
            //std::sort(temp->keys.begin(), temp->keys.end());
            temp->keys.insert(std::lower_bound( temp->keys.begin(), temp->keys.end(), val),val);
            if(temp->keys.size() != degree)//-1 XXXXXXXX
            {
                BPlusTreeNode<T>* null_child = NULL;
                temp->children.push_back(null_child);
            }
            else
            {
                std::cout<<"BYYY"<<std::endl;
                splitRoot();
            }
            
        }
    }
    else
    {
        temp->keys.insert(std::lower_bound( temp->keys.begin(), temp->keys.end(), val),val);
        if(temp->keys.size() == degree)
        {
            splitChild(temp);
            /*if(temp->keys.size() == degree)
             {
             splitRoot();
             //fix split root to rearrange pointers (DONE)?
             }
             */
            
        }
        
    }
    //temp = find(val);
}
template <class T> void BPlusTree<T>::splitChild(BPlusTreeNode<T>* temp)
{
    if(!temp->parent)
    {
        if(temp->keys.size() != degree)
        {
            return;
        }
        else
        {
            splitRoot();
            return;
        }
    }
    BPlusTreeNode<T>* left = new BPlusTreeNode<T>;
    BPlusTreeNode<T>* right = new BPlusTreeNode<T>;
    int splitAT = temp->keys.size()/2;
    for(int x = 0; x<temp->keys.size();x++)
    {
        if(x<splitAT)
            left->keys.push_back(temp->keys[x]);
        else
            right->keys.push_back(temp->keys[x]);
    }
    std::cout<<right->keys.size()<< "right size " << std::endl;
    (temp->parent)->keys.push_back(temp->keys[splitAT]);
    std::sort((temp->parent)->keys.begin(), (temp->parent)->keys.end());
    left->parent = temp->parent;
    right->parent = temp->parent;
    std::vector<BPlusTreeNode<T>*> temp_vec;
    
    for(int x = 0; x < (temp->parent)->children.size(); x++)
    {
        if(temp == (temp->parent)->children[x])
        {
            std::cout<<"TESTING"<<std::endl;
            temp_vec.push_back(left);
            temp_vec.push_back(right);
        }
        else
            temp_vec.push_back((temp->parent)->children[x]);
    }
    std::cout<<temp_vec.size()<< " temp_vec size " << std::endl;
    std::cout<<temp_vec[2]->keys.size()<< " keys from temp_vec" << std::endl;
    //0 - 1 3 4  ||||| 1 - 12 17
    std::cout<<(temp->parent)->children[0]->keys[3]<<"   SJKNJNsjNjnNNUNUNNNNIJII"<<std::endl;
    (temp->parent)->children = temp_vec;
    if(!temp->is_leaf())
    {
        /*
         std::vector<BPlusTreeNode<T>*> temp_vec2;
         if(temp->keys.size() == degree)
         {
         for(int x = 0; x < temp->children.size(); x++)
         {
         if(!((temp->children[x])->contains(temp->keys[splitAT])))
         temp_vec2.push_back(temp->children[x]);
         }
         temp->children = temp_vec2;
         }
         */
        for(int x = 0; x <= splitAT; x++)
        {
            left->children.push_back(temp->children[x]);
        }
        for(int x = splitAT+1; x < temp->children.size(); x++)
        {
            right->children.push_back(temp->children[x]);
        }
    }
    //std::cout<<"XXXXXXXXXXXXXXXXXXXXX"<<std::endl;
    //(temp->parent)->keys.insert(std::lower_bound( (temp->parent)->keys.begin(), (temp->parent)->keys.end(), temp->keys[splitAT]),temp->keys[splitAT]);
    std::cout<<temp->keys.size()<< " AGDVHSVDHJSHJBJBJKSBJKDBJKBDJKSJKB"<<std::endl;
    for(int x =0; x < temp->keys.size(); x++)
        std::cout<<temp->keys[x]<<std::endl;
    std::cout<<splitAT<<std::endl;
    splitChild(left->parent);
}
template <class T> void BPlusTree<T>::splitRoot()
{
    BPlusTreeNode<T>* left = new BPlusTreeNode<T>;
    BPlusTreeNode<T>* right = new BPlusTreeNode<T>;
    BPlusTreeNode<T>* new_root = new BPlusTreeNode<T>;
    int splitAT = root->keys.size()/2;
    //if(root->keys.size()%2 == 0) splitAT -=1;
    for(int x = 0; x<root->keys.size();x++)
    {
        if(x<splitAT)
            left->keys.push_back(root->keys[x]);
        else
            right->keys.push_back(root->keys[x]);
    }
    if(!root->is_leaf())
    {
        /*std::vector<BPlusTreeNode<T>*> temp_vec2;
         if(root->keys.size() == degree)
         {
         std::cout<<"Get in if"<<std::endl;
         for(int x = 0; x < root->children.size(); x++)
         {
         if(!((root->children[x])->contains(root->keys[splitAT])))
         {
         std::cout<<"Delete poped"<<std::endl;
         temp_vec2.push_back(root->children[x]);
         }
         }
         root->children = temp_vec2;
         
         }
         */
        for(int x = 0; x <= splitAT; x++)
        {
            left->children.push_back(root->children[x]);
        }
        for(int x = splitAT+1; x < root->children.size(); x++)
        {
            right->children.push_back(root->children[x]);
        }
    }
    new_root->children.push_back(left);
    new_root->children.push_back(right);
    left->parent = new_root;
    right->parent = new_root;
    new_root->keys.push_back(root->keys[splitAT]);
    new_root->parent = NULL;
    delete root;
    root = new_root;
}
template <class T> void BPlusTree<T>::print_sideways(std::ofstream& outfile)
{
    if(root == NULL)
        outfile<<"Tree is empty."<<std::endl;
}
template <class T> void BPlusTree<T>::print_BFS(std::ofstream& outfile)
{
    if(root == NULL)
        outfile<<"Tree is empty."<<std::endl;
}
/*
 template <class T> void BPlusTree<T>::insert(const T& val)
 {
 BPlusTreeNode<T>* temp = root;
 temp = find(val);
 if(temp == root)
 {
 if(!temp)
 {
 std::cout<<"FUCK"<<std::endl;
 temp = new BPlusTreeNode<T>;
 BPlusTreeNode<T>* null_child1 = NULL;
 BPlusTreeNode<T>* null_child2 = NULL;
 temp->keys.push_back(val);
 temp->children.push_back(null_child1);
 temp->children.push_back(null_child2);
 root = temp;
 }
 else
 {
 //temp->keys.push_back(val);
 //std::sort(temp->keys.begin(), temp->keys.end());
 temp->keys.insert(std::lower_bound( temp->keys.begin(), temp->keys.end(), val),val);
 if(temp->keys.size() != degree)//-1 XXXXXXXX
 {
 BPlusTreeNode<T>* null_child = NULL;
 temp->children.push_back(null_child);
 }
 else
 {
 std::cout<<"BYYY"<<std::endl;
 splitRoot();
 }
 
 }
 }
 else
 {
 std::cout<<"SHIIIIIIIT    "<<val<<std::endl;
 temp = find(val);
 std::cout<<"SHIIIIIIIT    "<<val<<std::endl;
 temp->keys.insert(std::lower_bound( temp->keys.begin(), temp->keys.end(), val),val);
 temp = find(val);
 if(temp->keys.size() == degree)
 {
 std::cout<<"SssssssT    "<<val<<std::endl;
 T temp_val = val;
 while(temp->parent && temp->keys.size() == degree)
 {
 std::cout<<"HIIIIIIaaaa"<<std::endl;
 temp = splitChild(temp, temp_val);
 temp = find(temp_val);
 temp->keys.insert(std::lower_bound( temp->keys.begin(), temp->keys.end(), temp_val),temp_val);
 //val needs to chnage
 }
 if(temp->keys.size() == degree)
 {
 std::cout<<"HIIIIIIaaaa"<<std::endl;
 splitRoot();
 std::cout<<"AAAAA"<<std::endl;
 //fix split root to rearrange pointers (DONE)?
 }
 
 }
 
 }
 //temp = find(val);
 }
 */
/*
 template <class T> BPlusTreeNode<T>* BPlusTree<T>::splitChild(BPlusTreeNode<T>* temp, T& val)
 {
 BPlusTreeNode<T>* left = new BPlusTreeNode<T>;
 BPlusTreeNode<T>* right = new BPlusTreeNode<T>;
 int splitAT = root->keys.size()/2;
 //if(root->keys.size()%2 == 0) splitAT -=1;
 for(int x = 0; x<root->keys.size();x++)
 {
 if(x<splitAT)
 left->keys.push_back(root->keys[x]);
 else
 right->keys.push_back(root->keys[x]);
 }
 left->parent = temp->parent;
 right->parent = temp->parent;
 //typename std::vector<T>::iterator itr = parent->children.find(temp);
 std::vector<BPlusTreeNode<T>*> temp_vec;
 for(int x = 0; x < temp->parent->children.size(); x++)
 {
 if(temp == temp->parent->children[x])
 {
 temp_vec.push_back(left);
 temp_vec.push_back(right);
 }
 else
 temp_vec.push_back((temp->parent)->children[x]);
 }
 ((temp->parent)->children) = temp_vec;
 std::vector<BPlusTreeNode<T>*> temp_vec2;
 std::cout<<(temp->parent)->keys[0]<<std::endl;
 //temp->parent->keys.insert(std::lower_bound( temp->parent->keys.begin(), temp->parent->keys.end(), temp->keys[splitAT]),temp->keys[splitAT]);
 (temp->parent)->keys.push_back(temp->keys[splitAT]);
 std::sort((temp->parent)->keys.begin(), (temp->parent)->keys.end());
 if(!temp->is_leaf())
 {
 if(temp->keys.size() == degree)
 {
 for(int x = 0; x < temp->children.size(); x++)
 {
 if(!(temp->children[x])->contains(temp->keys[splitAT]))
 temp_vec2.push_back(temp->children[x]);
 }
 temp->children = temp_vec2;
 }
 for(int x = 0; x <= splitAT; x++)
 {
 left->children.push_back(temp->children[x]);
 }
 for(int x = splitAT+1; x < temp->children.size(); x++)
 {
 right->children.push_back(temp->children[x]);
 }
 }
 val = temp->keys[splitAT];
 delete temp;
 return temp->parent;
 
 }
 */
#endif
