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
    BPlusTree(const BPlusTree<T>& tree);
    BPlusTree<T>& operator=(const BPlusTree* b);
    ~BPlusTree();
    
    BPlusTreeNode<T>* copy_helper(const BPlusTreeNode<T>* other);
    void destroy_tree(BPlusTreeNode<T>* temp);
    
    void print_sideways(std::ofstream& outfile);
    void print_BFS(std::ofstream& outfile);
    
    void insert(const T& val);
    
    
    BPlusTreeNode<T>* getRoot() const {return root; }
    BPlusTreeNode<T>* find(const T& val);
    
    void splitChild(BPlusTreeNode<T>* temp);
    void splitRoot();
    
    void pop(BPlusTreeNode<T>* temp, const T& val);
    
    friend class BPlusTreeNode<T>;
private:
    int degree;
    BPlusTreeNode<T>* root;
};

template <class T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree* old){
    this->degree = old->degree;
    this->root = copy_helper(old->root);
}


template <class T> BPlusTree<T>::BPlusTree(const BPlusTree& old){
    this->degree = old.degree;
    this->root = copy_helper(old.root);
}


template <class T> BPlusTreeNode<T>* BPlusTree<T>::copy_helper(const BPlusTreeNode<T>* tree){
    if(!tree) return NULL;
    BPlusTreeNode<T>* new_tree = new BPlusTreeNode<T>;

    for(int i = 0; i < tree->keys.size(); i++){
        new_tree->keys.push_back(tree->keys[i]);
    }
    
    for(int i = 0; i < tree->children.size();i++){
        new_tree->children.push_back(copy_helper(tree->children[i]));
    }
    
    return new_tree;
}

template <class T>
BPlusTree<T>::~BPlusTree(){
    destroy_tree(root);
    root = NULL;
}

template <class T>
void BPlusTree<T>::destroy_tree(BPlusTreeNode<T>* cursor){
    if(!cursor)
        return;
    
    for(unsigned int i=0; i<cursor->children.size(); i++){
        destroy_tree(cursor->children[i]);
    }
    
    delete cursor;
}
template <class T> BPlusTreeNode<T>* BPlusTree<T>::find(const T& val)
{
    BPlusTreeNode<T>* temp = root;
    BPlusTreeNode<T>* next_temp = root;
    int nxt_index = 0;
    T check_val;
    
    
    if(!temp)
    {
        return root;
    }
    
    if(next_temp->is_leaf())
    {
        return root;
    }
    
    while(!next_temp->is_leaf())
    {
        unsigned int x = 0;
        bool check = false;
        for(; x < next_temp->keys.size(); x++)
        {
            check_val = next_temp->keys[x];
            if(val < check_val)
            {
                nxt_index = x;
                check = true;
                break;
            }
        }
        if(!check)
        {
            nxt_index =  next_temp->keys.size();
        }
        next_temp = next_temp->children[nxt_index];
    }
    
    
    temp = next_temp;
    return temp;
}
/*
 ARGUMENTS:
    val- the value you want to insert
 PURPOSE:
    to insert a value into the BPlusTree and balence the tree
 RETURNS:
    void
 */
template <class T> void BPlusTree<T>::insert(const T& val)
{
    BPlusTreeNode<T>* temp = root;
    temp = find(val);
    
    //*********************************************************************
    //if there is ONLY a root
    if(temp == root)
    {
        if(!temp)//root NULL
        {
            temp = new BPlusTreeNode<T>;
            temp->keys.push_back(val);
            root = temp;
        }
        else
        {
            //inserts in sorted position
            temp->keys.insert(std::lower_bound( temp->keys.begin(), temp->keys.end(), val),val);
            if(temp->keys.size() != degree)
            {
                BPlusTreeNode<T>* null_child = NULL;
                temp->children.push_back(null_child);
            }
            else
            {
                splitRoot();
            }
            
        }
    }
    //*********************************************************************
    
    
    
    else
    {
        temp->keys.insert(std::lower_bound( temp->keys.begin(), temp->keys.end(), val),val);
        if(temp->keys.size() == degree)
        {
            splitChild(temp);
        }
        
    }
    //temp = find(val);
}
/* 
 ARGUMENTS;
    temp- BPlusTreeNode that initial is passed the leaf where you inserted the val and want to split the child, then it is the parent of the last for every recusive call
 PURPOSE:
    recursively splits all children that are full when you insert a val
 RETURNS:
    void
 */
template <class T> void BPlusTree<T>::splitChild(BPlusTreeNode<T>* temp)
{
    
    //************************************************************************************** BASE CASES
    
    if(temp->keys.size() != degree)
    {
        return;
    }
    
    
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
    
    //************************************************************************************** BASE CASES
    
    
    
    BPlusTreeNode<T>* left = new BPlusTreeNode<T>;
    BPlusTreeNode<T>* right = new BPlusTreeNode<T>;
    int splitAT = temp->keys.size()/2;
    
    //***************************************************************
    //splits the temp keys vector into the left and right from the splitAT value
    for(int x = 0; x<temp->keys.size();x++)
    {
        if(x<splitAT)
            left->keys.push_back(temp->keys[x]);
        else
            right->keys.push_back(temp->keys[x]);
    }
    
    //***************************************************************
    
    (temp->parent)->keys.insert(std::lower_bound( (temp->parent)->keys.begin(), (temp->parent)->keys.end(), temp->keys[splitAT]),temp->keys[splitAT]);//inserts the val in at splitAT in to the parent of temp sorted
    
    //assigns parent pointers for left and right
    left->parent = temp->parent;
    right->parent = temp->parent;
    
    
    //assigns the new children pointers for temp->parent
    std::vector<BPlusTreeNode<T>*> temp_vec;
    for(int x = 0; x < (temp->parent)->children.size(); x++)
    {
        if(temp == (temp->parent)->children[x])
        {
            temp_vec.push_back(left);
            temp_vec.push_back(right);
        }
        else
            temp_vec.push_back((temp->parent)->children[x]);
    }
    (temp->parent)->children = temp_vec;
    
    
    //assigns the children pointers for left and right to what the temp was pointing to
    //assings what ever the childrens parents pointers of the temp to the new left and right
    if(!temp->is_leaf())
    {
        for(int x = 0; x <= splitAT; x++)
        {
            left->children.push_back(temp->children[x]);
            (temp->children[x])->parent = left;
        }
        for(int x = splitAT+1; x < temp->children.size(); x++)
        {
            right->children.push_back(temp->children[x]);
            (temp->children[x])->parent = right;
        }
        std::vector<BPlusTreeNode<T>*> temp_vec2;
        if(temp->keys.size() == degree)
        {
            for(int x = 0; x < temp->children.size(); x++)
            {
                if(!((temp->children[x])->contains(temp->keys[splitAT])))
                    temp_vec2.push_back(temp->children[x]);
                else
                {
                    std::vector<T> temp_vec3;
                    for(int y = 0; y < (temp->parent)->children[x]->keys.size(); y++)
                    {
                        if((temp->parent)->children[x]->keys[y] != temp->keys[splitAT])
                        {
                            temp_vec3.push_back((temp->parent)->children[x]->keys[y]);
                        }
                    }
                    
                }
            }
            (temp->parent)->children = temp_vec2;
        }
        /*
        if(root->keys.size() == degree)
        {
            
            for(int x = 0; x < new_root->children.size(); x++)
            {
                if(!((new_root->children[x])->contains(root->keys[splitAT])))
                {
                    temp_vec2.push_back(new_root->children[x]);
                }
                else
                {
                    std::vector<T> temp_vec3;
                    for(int y = 0; y < new_root->children[x]->keys.size(); y++)
                    {
                        if(new_root->children[x]->keys[y] != root->keys[splitAT])
                        {
                            temp_vec3.push_back(new_root->children[x]->keys[y]);
                        }
                    }
                    new_root->children[x]->keys = temp_vec3;
                    temp_vec2.push_back(new_root->children[x]);
                }
            }
            new_root->children = temp_vec2;
        }
         */
    }
    delete temp;//delete the old Node
    splitChild(left->parent);//recursive call, can call left or right parent same thing
}
/*
 ARGUMENTS:
    NONE
 PURPOSE:
    splits the root when full and creates a new root with the value at the midpoint
 RETURN:
    void
 */
template <class T> void BPlusTree<T>::splitRoot()
{
    BPlusTreeNode<T>* left = new BPlusTreeNode<T>;
    BPlusTreeNode<T>* right = new BPlusTreeNode<T>;
    BPlusTreeNode<T>* new_root = new BPlusTreeNode<T>;
    
    int splitAT = root->keys.size()/2;//finds where to split
    
    //splits the roots keys vector into the left and right from the splitAT value
    for(int x = 0; x<root->keys.size();x++)
    {
        if(x<splitAT)
            left->keys.push_back(root->keys[x]);
        else
            right->keys.push_back(root->keys[x]);
    }
    
    
    //the new_roots children to the left and right nodes you created
    new_root->children.push_back(left);
    new_root->children.push_back(right);
    
    //*********************************************************************
    //If the root is not a leaf then yo have to reassign the children pointers for the root
    if(!root->is_leaf())
    {
        //*********************************************************************
        //assigns the children pointers for left and right to what the root was pointing to
        //assings what ever the childrens parents pointers of the root to the new left and right
        for(int x = 0; x <= splitAT; x++)
        {
            left->children.push_back(root->children[x]);
            root->children[x]->parent = left;
        }
        for(int x = splitAT+1; x < root->children.size(); x++)
        {
            right->children.push_back(root->children[x]);
            root->children[x]->parent = right;
        }
        //*********************************************************************
        
        
        
        //*********************************************************************
        //this block checks if the value pop into the new_root is also in the old root, and if it is, you need to take it out of the old one
        std::vector<BPlusTreeNode<T>*> temp_vec2;
        if(root->keys.size() == degree)
        {
            
            for(int x = 0; x < new_root->children.size(); x++)
            {
                if(!((new_root->children[x])->contains(root->keys[splitAT])))
                {
                    temp_vec2.push_back(new_root->children[x]);
                }
                else
                {
                    std::vector<T> temp_vec3;
                    for(int y = 0; y < new_root->children[x]->keys.size(); y++)
                    {
                        if(new_root->children[x]->keys[y] != root->keys[splitAT])
                        {
                            temp_vec3.push_back(new_root->children[x]->keys[y]);
                        }
                    }
                    new_root->children[x]->keys = temp_vec3;
                    temp_vec2.push_back(new_root->children[x]);
                }
            }
            new_root->children = temp_vec2;
            
        }
    }
    //*********************************************************************
    
    
    left->parent = new_root;
    right->parent = new_root;
    
    
    new_root->keys.push_back(root->keys[splitAT]);
    new_root->parent = NULL;
    delete root;
    root = new_root;
}
template <class T> void BPlusTree<T>::print_sideways(std::ofstream& out)
{
    if(root == NULL)
    {
        out<<"Tree is empty."<<std::endl;
        return;
    }
}
template <class T> void BPlusTree<T>::print_BFS(std::ofstream& out)
{
    if(root == NULL)
    {
        out<<"Tree is empty."<<std::endl;
        return;
    }
    for(int x = 0; x< root->keys.size(); x++)
    {
        out<<root->keys[x];
        if(root->keys.size()>1 && x != root->keys.size()-1)
            out<<",";
    }
    out<<"\n";
}
#endif
