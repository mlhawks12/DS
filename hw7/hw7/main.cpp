// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include "mtrand.h"


// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE
typedef  std::map<std::string, std::map<std::string, int> >  MY_MAP;
typedef  std::map<std::string, int> sub_MAP;
typedef  std::pair<std::string, int> sub_PAIR;
typedef  std::pair<std::string, sub_PAIR> my_PAIR;
typedef  std::map<std::string, std::map<std::string,std::map<std::string, int> > >  threeD_MAP;



// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
    char c;
    word.clear();
    while (istr) {
        // just "peek" at the next character in the stream
        c = istr.peek();
        if (isspace(c)) {
            // skip whitespace before a word starts
            istr.get(c);
            if (word != "") {
                // break words at whitespace
                return true;
            }
        } else if (c == '"') {
            // double quotes are a delimiter and a special "word"
            if (word == "") {
                istr.get(c);
                word.push_back(c);
            }
            return true;
        } else if (isalpha(c)) {
            // this a an alphabetic word character
            istr.get(c);
            word.push_back(tolower(c));
        } else {
            // ignore this character (probably punctuation)
            istr.get(c);
        }
    }
    return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
    // returns a vector of strings of the different words
    std::vector<std::string> answer;
    std::string word;
    bool open_quote = false;
    while (ReadNextWord(istr,word)) {
        if (word == "\"") {
            if (open_quote == false) { open_quote=true; }
            else { break; }
        } else {
            // add each word to the vector
            answer.push_back(word);
        }
    }
    return answer;
}



// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
/*
 ARGUMENTS:
    data- a map of window 2
    filename- the string of the name of the file
    window- the window size/ demension of the map
    parse_method- a string that tell what type of parsing it should do
 PURPOSE:
    -processes all the data and puts it in to the data structure
    -only works for window 2
 RETURNS:
    void
*/
void LoadSampleText(MY_MAP &data, const std::string &filename, int window, const std::string &parse_method) {
    // open the file stream
    
    std::ifstream istr(filename.c_str());
    if (!istr) {
        std::cerr << "ERROR cannot open file: " << filename << std::endl;
        exit(1);
    }
    // verify the window parameter is appropriate
    if (window < 2) {
        std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
    }
    // verify that the parse method is appropriate
    bool ignore_punctuation = false;
    if (parse_method == "ignore_punctuation") {
        ignore_punctuation = true;
    } else {
        std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
        exit(1);
    }
    
    
    
    //
    // ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
    //
    std::string word;
    std::string last_word = "@";
    while (ReadNextWord(istr,word)) {
        // skip the quotation marks (not used for this part)
        if (word == "\"") continue;
        if(last_word == "@")//if the variable word is equal to the first word in the file, then continue
        {
            last_word = word;
            continue;
        }
        //
        // ASSIGNMENT: PROCESS ALL THE NON PUNCTUATION WORDS
        // INSERTING THESE WORDS INTO THE DATA STRUCTURE
        //
        
        MY_MAP::iterator itr = data.find(last_word);
        sub_MAP inner;
        if(itr == data.end()){//didnt find the key for last_word in the outer map
            inner.insert(std::make_pair("num_uses",1));//a key to tell how many times the outer key is used
            inner.insert(std::make_pair(word,1));//creates inner pair
            data.insert(std::make_pair(last_word, inner));
        }
        else
        {
            sub_MAP::iterator itr2 = (itr->second).find(word);
            if(itr2 == itr->second.end())//didnt find the key in the inner map
            {
                itr->second.insert(std::make_pair(word, 1));
            }
            else
                itr2->second++;
            ++((itr->second)["num_uses"]);
        }
        last_word = word;
    }
    MY_MAP::iterator last_itr = data.find(last_word);
    if(last_itr != data.end())//if last_word is the last word in the entire file, the increment num_uses key
        ++((last_itr->second)["num_uses"]);
}
/*
 ARGUMENTS:
    data- a map of window 3
    filename- the string of the name of the file
    window- the window size/ demension of the map
    parse_method- a string that tell what type of parsing it should do
 PURPOSE:
    -processes all the data and puts it in to the data structure
    -only works for window 3
 RETURNS:
    void
 */
void LoadSampleText(threeD_MAP &data, const std::string &filename, int window, const std::string &parse_method) {
    // open the file stream
    
    std::ifstream istr(filename.c_str());
    if (!istr) {
        std::cerr << "ERROR cannot open file: " << filename << std::endl;
        exit(1);
    }
    // verify the window parameter is appropriate
    if (window < 2) {
        std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
    }
    // verify that the parse method is appropriate
    bool ignore_punctuation = false;
    if (parse_method == "ignore_punctuation") {
        ignore_punctuation = true;
    } else {
        std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
        exit(1);
    }
    
    
    
    //
    // ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
    //
    std::string word;
    std::string last_word = "@";
    std::string last_last_word = "@@";
    while (ReadNextWord(istr,word)) {
        // skip the quotation marks (not used for this part)
        if (word == "\"") continue;
        if(last_word == "@")//if the variable word is the first word continue
        {
            last_word = word;
            continue;
        }
        if(last_last_word == "@@")//if the variable word is the second word continue
        {
            last_last_word = last_word;
            continue;
        }
        threeD_MAP::iterator itr = data.find(last_last_word);
        MY_MAP inner1;
        sub_MAP inner2;
        sub_MAP inner3;
        if(itr == data.end())//if key is not found
        {
            inner3.insert(std::make_pair("num_uses", 1));//creates a num_uses for the key num_uses to count how many times word is used
            inner2.insert(std::make_pair("num_uses", 1));//creates a num_uses for all word found in the file
            inner2.insert(std::make_pair(word, 1));
            inner1.insert(std::make_pair("num_uses", inner3));
            inner1.insert(std::make_pair(last_word, inner2));
            data.insert(std::make_pair(last_last_word, inner1));
        }
        else
        {
            MY_MAP::iterator itr2 = (itr->second).find(last_word);
            MY_MAP::iterator num_itr = (itr->second).find("num_uses");
            ++((num_itr->second)["num_uses"]);
            if(itr2 == itr->second.end())
            {
                inner2.insert(std::make_pair("num_uses", 1));
                inner2.insert(std::make_pair(word, 1));
                itr->second.insert(std::make_pair(last_word, inner2));
            }
            else
            {
                sub_MAP::iterator itr3 = (itr2->second).find(word);
                ++((itr2->second)["num_uses"]);
                if(itr3 == itr2->second.end())
                {
                    itr2->second.insert(std::make_pair(word, 1));
                }
                else
                {
                    itr3->second++;
                }
            }
        }
        last_last_word = last_word;
        last_word = word;
    }
    /*for(threeD_MAP::iterator itr4 = data.begin(); itr4 != data.end(); itr4++)
    {
        std::cout<<itr4->first<<std::endl;
        for(MY_MAP::iterator itr5 = itr4->second.begin(); itr5 != itr4->second.end(); itr5++)
        {
            std::cout<<"\t"<<itr5->first<<std::endl;
            for(sub_MAP::iterator itr6 = itr5->second.begin(); itr6 != itr5->second.end(); itr6++)
            {
                std::cout<<"\t\t"<<itr6->first<<" " <<itr6->second<<std::endl;
            }
        }
    }
     */
}
/*
 ARGUMENTS:
    sen- takes in the word in the quotes to be printed
    data- a map with window 2
 PURPOSE:
    -to print a specified word and all the words that have follewed it any point
    -for window 2 only
 RETURN:
    void
 */
void print2D(const std::vector<std::string>& sen, MY_MAP& data)
{
    std::cout<<std::endl;
    MY_MAP::const_iterator itr = data.find(sen[0]);
    if(itr == data.end())
        return;
    sub_MAP::const_iterator itr2 = (itr->second).find("num_uses");
    std::cout<<itr->first<< " ("<<itr2->second<<")"<<std::endl;
    for(sub_MAP::const_iterator itr3 = itr->second.begin(); itr3 != itr->second.end(); itr3++)
    {
        if(itr3->first == "num_uses")continue;
        std::cout<<itr->first<< " " << itr3->first<< " ("<<itr3->second<<")"<<std::endl;
    }
}
/*
ARGUMENTS:
    sen- takes in the word in the quotes to be printed
    data- a map with window 2
    filename- the string of the name of the file
    window- the window size/ demension of the map
    parse_method- a string that tell what type of parsing it should do
PURPOSE:
    -to print a specified word and all the words that have follewed it any point
    -for window 2 only
RETURN:
    void
*/
void print3D(const std::vector<std::string>& sen, threeD_MAP& data, const std::string &filename, int window, const std::string &parse_method)
{
    if(sen.size() == 1)
    {
        MY_MAP data1;
        LoadSampleText(data1, filename, 2, parse_method);
        print2D(sen, data1);
        return;
    }
    else
    {
        std::cout<<std::endl;
        threeD_MAP::const_iterator itr = data.find(sen[0]);
        MY_MAP::const_iterator itr2 = itr->second.find(sen[1]);
        if(itr == data.end())//if word not there, dont do anything
            return;
        sub_MAP::const_iterator num_itr = (itr2->second).find("num_uses");//find the number of uses the word
        std::cout<<itr->first<< " " << itr2->first<<" ("<<num_itr->second<<")"<<std::endl;
        for(sub_MAP::const_iterator itr3 = itr2->second.begin(); itr3 != itr2->second.end(); itr3++)
        {
            if(itr3->first == "num_uses")continue;
            std::cout<<itr->first<< " " << itr2->first<< " " << itr3->first<<" ("<<itr3->second<<")"<<std::endl;
        }
    }
}
/*
 ARGUMENTS:
    data- a map of window 2
    word- vector of words that you want to find the most common things that come after
    length- length of the sentence to make after the word in sen
 PURPOSE:
    To print out a sentence out of the most common occucernces of word after the specified word
RETURN:
    void
*/
void mostCommon2D(const MY_MAP& data, std::vector<std::string> word , int length)
{
    std::cout<<std::endl;
    std::string sentence;
    MY_MAP::const_iterator itr;
    std::string word1 = word[0];
    sentence += (word1);
    for(int x = 0; x < length; x++)
    {
        itr = data.find(word1);
        int max = 0;
        sub_MAP::const_iterator max_itr = itr->second.begin();
        for(sub_MAP::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
        {
            if(itr2->first == "num_uses")continue;
            if(itr2->second > max)
            {
                max_itr = itr2;
                max = itr2->second;
            }
        }
        word1 = max_itr->first;
        sentence+=(" "+word1);
    }
    std::cout<<sentence<<std::endl<<std::endl;
}
/*
 ARGUMENTS:
    data- a map of window 2
    word- vector of words that you want to find the things that come after randomly
    length- length of the sentence to make after the word in word
 PURPOSE:
    To print out a sentence out of the words randomly chosen that come after the specified word
 RETURN:
    void
 */
void gen_rand2D(const MY_MAP& data, std::vector<std::string> word , int length)
{
    MTRand_closed nums;
    std::string senternce;
    MY_MAP::const_iterator itr;
    std::string word1 = word[0];
    std::string max_word = word1;
    senternce+= max_word;
    for(int x = 0; x < length; x++)
    {
        itr = data.find(max_word);
        float prob1 = 0;
        float prob2 = nums();
        //int sum = 0;
        sub_MAP::const_iterator itr_num = (itr->second).find("num_uses");
        //for(sub_MAP::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
            //sum+=itr2->second;
        for(sub_MAP::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
        {
            if(itr2->first == "num_uses")continue;
            prob1 += float(itr2->second)/itr_num->second;
            if(prob2 < prob1)
            {
                max_word = itr2->first;
                break;
            }
        }
        senternce+=(" "+max_word);
    }
    std::cout<<senternce<<std::endl;
    std::cout<<std::endl;
}
/*
 ARGUMENTS:
    data- a map of window 3
    sen- vector of words that you want to find the most common things that come after
    length- length of the sentence to make after the word in sen
    filename- the string of the name of the file
    window- the window size/ demension of the map
    parse_method- a string that tell what type of parsing it should do
 PURPOSE:
    To print out a sentence out of the most common occucernces of word after the specified words
 RETURN:
    void
 */
void mostCommon3D(const threeD_MAP& data, std::vector<std::string> sen , int length,const std::string &filename, int window, const std::string&parse_method)
{
    std::cout<<std::endl;
    std::string word1 = sen[0];
    std::string word2;
    std::string sentence;
    sentence+= (word1);
    if(sen.size() == 1)
    {
        MY_MAP data1;
        LoadSampleText(data1, filename, 2, parse_method);
        std::string sentence;
        MY_MAP::const_iterator itr;
        std::string word_1 = sen[0];
        sentence += (word1);
            itr = data1.find(word1);
            int max = 0;
            sub_MAP::const_iterator max_itr = itr->second.begin();
            for(sub_MAP::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
            {
                if(itr2->first == "num_uses")continue;
                if(itr2->second > max)
                {
                    max_itr = itr2;
                    max = itr2->second;
                }
            }
            word2 = max_itr->first;
        length-=1;
    }
    else
    {
        word2 = sen[1];
    }
    sentence+= (" "+word2);
    for(int x = 0; x < length; x++)
    {
        threeD_MAP::const_iterator itr = data.find(word1);
        MY_MAP::const_iterator itr2 = (itr->second).find(word2);
        int max = 0;
        sub_MAP::const_iterator max_itr = itr2->second.begin();
        for(sub_MAP::const_iterator itr3 = itr2->second.begin(); itr3 != itr2->second.end(); itr3++)
        {
            if(itr3->first == "num_uses")continue;
            if(itr3->second > max)
            {
                max_itr = itr3;
                max = itr3->second;
            }
        }
        word1 = word2;
        word2 = max_itr->first;
        sentence+=(" "+word2);
    }
    std::cout<<sentence<<std::endl;
}
/*
 ARGUMENTS:
    data- a map of window 3
    sen- vector of words that you want to find the things that come after randomly
    length- length of the sentence to make after the word in sen
    filename- the string of the name of the file
    window- the window size/ demension of the map
    parse_method- a string that tell what type of parsing it should do
 PURPOSE:
    To print out a sentence out of the words randomly chosen that come after the specified word
 RETURN:
    void
 */
void gen_rand3D(const threeD_MAP& data, std::vector<std::string> sen, int length,const std::string &filename, int window, const std::string&parse_method)
{
    std::cout<<std::endl;
    MTRand_closed nums;
    std::string word1 = sen[0];
    std::string word2;
    std::string max_word = word2;
    std::string sentence;
    sentence+= (word1);
    if(sen.size() == 1)//if sen is size one word, finds the second word randomly like in 2D then goes into 3D
    {
        std::string max_word2 = word1;
        MY_MAP data2;
        MY_MAP::iterator itr = data2.find(max_word);
        float prob1 = 0;
        float prob2 = nums();
        //int sum = 0;
        sub_MAP::const_iterator itr_num = (itr->second).find("num_uses");
        for(sub_MAP::const_iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++)
        {
            if(itr2->first == "num_uses")continue;
            prob1 += float(itr2->second)/itr_num->second;
            if(prob2 < prob1)
            {
                max_word2 = itr2->first;
                break;
            }
        }
        word2 = max_word;
        length-=1;
    }
    else
        word2 = sen[1];
    sentence+= (" "+word2);
    for(int x = 0; x < length; x++)
    {
        threeD_MAP::const_iterator itr = data.find(word1);
        MY_MAP::const_iterator itr2 = (itr->second).find(word2);
        sub_MAP::const_iterator itr_num = (itr2->second).find("num_uses");
        float prob1 = nums();
        float prob2 = 0;
        for(sub_MAP::const_iterator itr3 = itr2->second.begin(); itr3 != itr2->second.end(); itr3++)
        {
            if(itr3->first == "num_uses")continue;
            prob2 += float(itr3->second)/itr_num->second;//probabilty for each word
            if(prob1 < prob2)
            {
                max_word = itr3->first;
                break;
            }
        }
        sentence+=(" "+max_word);
        word1 = word2;
        word2 = max_word;
    }
    std::cout<<sentence<<std::endl;

}
int main () {
    
    // ASSIGNMENT: THE MAIN DATA STRUCTURE
    MY_MAP data;
    threeD_MAP data2;
    //random11();
    // Parse each command
    std::string command;
    int window = 0;
    std::string filename;
    std::string parse_method;
    while (std::cin >> command) {
        
        // load the sample text file
        if (command == "load") {
            std::cin >> filename >> window >> parse_method;
            std::cout<<"Loaded "<<filename << " with window = " <<window <<" and parse method = " <<parse_method<<std::endl;
            //
            // ASSIGNMENT: ADD YOUR COMMANDS HERE
            //
            if(window == 2)
                LoadSampleText(data, filename, window, parse_method);
            if(window == 3)
                LoadSampleText(data2, filename, window, parse_method);
        }
        
        // print the portion of the map structure with the choices for the
        // next word given a particular sequence.
        else if (command == "print") {
            std::vector<std::string> sentence = ReadQuotedWords(std::cin);
            
            //
            // ASSIGNMENT: ADD YOUR COMMANDS HERE
            //
            if(window == 2)
                print2D(sentence, data);
            else
            {
                print3D(sentence, data2, filename, window, parse_method);
            }
            
        }
        
        // generate the specified number of words
        else if (command == "generate") {
            std::vector<std::string> sentence = ReadQuotedWords(std::cin);
            // how many additional words to generate
            int length;
            std::cin >> length;
            std::string selection_method;
            std::cin >> selection_method;
            bool random_flag;
            if (selection_method == "random") {
                random_flag = true;
            } else {
                assert (selection_method == "most_common");
                random_flag = false;
            }
            
            
            //
            // ASSIGNMENT: ADD YOUR COMMANDS HERE
            //
            if(random_flag)
            {
                if(window == 2)
                    gen_rand2D(data, sentence, length);
                else
                    gen_rand3D(data2, sentence, length, filename, window, parse_method);
            }
            else
            {
                if(window == 2)
                    mostCommon2D(data, sentence, length);
                else
                    mostCommon3D(data2, sentence, length, filename, window, parse_method);
            }
            
        } else if (command == "quit") {
            break;
        } else {
            std::cout << "WARNING: Unknown command: " << command << std::endl;
        }
    }
    std::cout<<std::endl;
}