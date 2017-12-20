//
//  edwin.cpp
//  hw6
//
//  Created by mike Levine on 3/20/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

// basic file operations
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <ctime>
void readFile(int& cols,int& rows, std::list<std::list<char> >& addWords, std::list<std::list<char> >& subWords, std::ifstream& inputs){
    std::string word;
    inputs >> cols;
    inputs >> rows;
    while(!inputs.eof()){
        std::list<char> wordy;
        inputs >> word;
        if(word=="+"){
            inputs >> word;
            for(int i = 0; i < word.size();i++){
                wordy.push_back(word[i]);
            }
            addWords.push_back(wordy);
        }else if(word=="-"){
            inputs >> word;
            for(int i = 0; i < word.size();i++){
                wordy.push_back(word[i]);
            }
            subWords.push_back(wordy);
        }
    }
}
void printWords(std::ofstream& output, std::list<std::list<char> >& addWords, std::list<std::list<char> >& subWords){
    std::list<char>::iterator col;
    std::list<std::list<char> >::iterator row;
    for( row = addWords.begin(); row != addWords.end(); ++row){
        for( col = row->begin(); col != row->end(); ++col){
            output << *col;
        }
        output << std::endl;
    }
    for(row = subWords.begin(); row != subWords.end(); ++row){
        for(col = row->begin(); col != row->end(); ++col){
            output << *col;
        }
        output << std::endl;
    }
    
}
void makeBlankBoard(std::vector<std::vector<char> >& board, int rows, int cols){
    char initial = '*';
    std::vector<char> col;
    for(int i = 0; i < cols; i++){
        col.push_back(initial);
    }
    for(int i = 0; i < rows; i++){
        board.push_back(col);
    }
}
void outputBoard(std::ofstream& outputs,const std::vector<std::vector<char> >& board){
    for(int i = 0; i < board.size(); i++){
        outputs << "  ";
        for(int j = 0; j < board[i].size(); j++){
            outputs << board[i][j];
        }
        outputs << std::endl;
    }
}
void printBoard(const std::vector<std::vector<char> >& board){
    for(int i = 0; i < board.size(); i++){
        std::cout << "  ";
        for(int j = 0; j < board[i].size(); j++){
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
}
void printWord(std::list<char> word){
    std::list<char>::iterator itr = word.begin();
    while(itr != word.end()){
        std::cout << *itr;
        itr++;
    }
    std::cout << std::endl;
}
/*
 
 
 returns
 - 0 if the word can go there
 - 1 if the first letter in the word can't be placed
 - 2 other
 */
int checkWord(std::string direction, int row, int col,const std::vector<std::vector<char> >& board, std::list<char>& word){
    
    std::list<char>::iterator itr = word.begin();
    int vertoffset = 0;
    int horioffset = 0;
    if(direction=="N" ||direction=="NE"||direction=="NW"){
        vertoffset = -1;
    }
    if(direction=="S" ||direction=="SE"||direction=="SW"){
        vertoffset = 1;
    }
    if(direction=="E" ||direction=="NE"||direction=="SE"){
        horioffset = 1;
    }
    if(direction=="W" ||direction=="NW"||direction=="SW"){
        horioffset = -1;
    }
    
    if(!(*itr==board[row][col])&&!('*'==board[row][col])){
        return 1;
    }
    while(itr != word.end()){
        if( (row >= 0) && (col >= 0) && (row < board.size()) && (col < board[0].size())){
            if(board[row][col]== '*'||board[row][col]==*itr){
                itr++;
                row = row + vertoffset;
                col = col +horioffset;
            }else{
                return 2;
            }
        }else{
            return 2;
        }
    }
    return 0;
}
std::vector<std::vector<char> > placeOne(std::string direction, int row, int col, std::vector<std::vector<char> > board, std::list<char>& word){
    int vertoffset = 0;
    int horioffset = 0;
    std::list<char>::iterator itr = word.begin();
    if(direction=="N" ||direction=="NE"||direction=="NW"){
        vertoffset = -1;
    }
    if(direction=="S" ||direction=="SE"||direction=="SW"){
        vertoffset = 1;
    }
    if(direction=="E" ||direction=="NE"||direction=="SE"){
        horioffset = 1;
    }
    if(direction=="W" ||direction=="NW"||direction=="SW"){
        horioffset = -1;
    }
    while(itr != word.end()){
        board[row][col] = *itr;
        itr++;
        row = row + vertoffset;
        col = col + horioffset;
    }
    return board;
}

void placeAll(int count, std::vector<std::vector<char> >& board, std::vector<std::vector<std::vector<char> > >& boards, std::list<std::list<char> >::iterator wordsItr, std::list<std::list<char> >& words){
    int code;
    std::vector<std::string> dir;
    dir.push_back("E");
    dir.push_back("SE");
    dir.push_back("S");
    dir.push_back("SW");
    dir.push_back("W");
    dir.push_back("NW");
    dir.push_back("N");
    dir.push_back("NE");
    std::vector<std::vector<char> > tempBoard;
    if(count>(board.size()*board[0].size())-1){
        return;
    }
    if(wordsItr == words.end()){
        boards.push_back(board);
        return;
    }
    std::list<char> temp = *wordsItr;
    int row = count/board[0].size();
    int col = count%board[0].size();
    for(int i = 0; i < dir.size(); i++) {
        code = checkWord(dir[i], row, col, board, temp);
        if(code == 0){
            tempBoard = placeOne(dir[i], row, count%board[0].size(), board, temp);
            wordsItr++;
            placeAll(0, tempBoard, boards, wordsItr, words);
            wordsItr--;
        }else if(code == 1){
            break;
        }
    }
    count++;
    placeAll(count, board, boards, wordsItr, words);
}
int checkWordSub(std::string direction, int row, int col,const std::vector<std::vector<char> >& board, std::list<char>& word){
    
    std::list<char>::iterator itr = word.begin();
    int vertoffset = 0;
    int horioffset = 0;
    if(direction=="N" ||direction=="NE"||direction=="NW"){
        vertoffset = -1;
    }
    if(direction=="S" ||direction=="SE"||direction=="SW"){
        vertoffset = 1;
    }
    if(direction=="E" ||direction=="NE"||direction=="SE"){
        horioffset = 1;
    }
    if(direction=="W" ||direction=="NW"||direction=="SW"){
        horioffset = -1;
    }
    
    if(!(*itr==board[row][col])){
        return 1;
    }
    while(itr != word.end()){
        if( (row >= 0) && (col >= 0) && (row < board.size()) && (col < board[0].size())){
            if(board[row][col]==*itr){
                itr++;
                row = row + vertoffset;
                col = col +horioffset;
            }else{
                return 2;
            }
        }else{
            return 2;
        }
    }
    return 0;
}
bool subBoardsHelper(std::vector<std::vector<char> >& board,std::list<std::list<char> >& words){
    int row;
    int col;
    int code;
    std::vector<std::string> dir;
    dir.push_back("E");
    dir.push_back("SE");
    dir.push_back("S");
    dir.push_back("SW");
    dir.push_back("W");
    dir.push_back("NW");
    dir.push_back("N");
    dir.push_back("NE");
    std::vector<std::vector<std::vector<char> > >::iterator boardsitr;
    std::list<char> currword;
    std::list<std::list<char> >::iterator wordsitr;
    //loop through words
    for(wordsitr = words.begin(); wordsitr != words.end(); wordsitr++){
        currword = *wordsitr;
        std::list<char>::iterator titers = currword.begin();
        //printWord(currword);
        //loop through postions
        for(int pos = 0; pos<((board.size()*board[0].size())); pos++){
            row = pos/board[0].size();
            col = pos%board[0].size();
            //loop through directions
            if(board[row][col]==*titers){
                for(int i = 0; i < dir.size(); i ++){
                    code = checkWordSub(dir[i], row, col, board, currword);
                    if(code==0){
                        return true;
                    }else if(code == 1){
                        break;
                    }
                }
            }
        }
    }
    return false;
}

void subBoards(std::vector<std::vector<std::vector<char> > >& allBoards, std::list<std::list<char> >& words){
    std::vector<std::vector<char> > board;
    std::vector<std::vector<std::vector<char> > >::iterator boardsitr;
    //loop through boards
    for(boardsitr = allBoards.begin(); boardsitr != allBoards.end(); boardsitr++){
        board = *boardsitr;
        //printBoard(board);
        if(subBoardsHelper(board, words)){
            boardsitr = allBoards.erase(boardsitr);
            boardsitr--;
        }
        
    }
}
int findFree(std::vector<std::vector<char> > & board){
    int row;
    int col;
    for(int pos = 0; pos < board.size()*board[0].size(); pos++){
        row = pos/board[0].size();
        col = pos%board[0].size();
        if(board[row][col]=='*'){
            return pos;
        }
    }
    return -1;
}

void findAllVariations(std::vector<std::vector<char> > board, std::vector<std::vector<std::vector<char> > > &allBoard, std::vector<char>& alphabet){
    
    int count = findFree(board);
    if(count== -1){
        allBoard.push_back(board);
        return;
    }
    int row = count/board[0].size();
    int col = count%board[0].size();
    for(int i = 0; i < alphabet.size(); i ++){
        board[row][col] = alphabet[i];
        findAllVariations(board, allBoard, alphabet);
    }
}
std::string flatten(std::vector<std::vector<char> >& board){
    std::string temp;
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[0].size();j++){
            temp.push_back(board[i][j]);
        }
    }
    return temp;
}
bool hasNoCopies(std::vector<std::vector<std::vector<char> > >& allBoards, std::vector<std::vector<char> > & board){
    for(int i = 0; i < allBoards.size(); i ++){
        if(flatten(board)==flatten(allBoards[i])){
            return false;
        }
    }
    return true;
}
std::vector<std::vector<std::vector<char> > > finalChecks(std::vector<std::vector<std::vector<char> > >& allBoards){
    char alpha[] = "abcdefghijklmnopqrstuvwyxz";
    std::vector<std::vector<std::vector<char> > > allVariation;
    std::vector<char> alphabet( alpha, alpha+sizeof(alpha)-1 ) ;
    std::vector<std::vector<std::vector<char> > > finalBoards;
    std::vector<std::vector<std::vector<char> > > temp;
    std::vector<std::vector<std::vector<char> > > meow;
    std::vector<std::vector<std::vector<char> > >::iterator itr;
    std::vector<std::vector<char> > board;
    std::unique(allBoards.begin(),allBoards.end());
    for(int j = 0; j < allBoards.size(); j++){
        if(findFree(allBoards[j])==-1){
            temp.push_back(allBoards[j]);
        }else{
            findAllVariations(allBoards[j], meow, alphabet);
        }
    }
    for(int i = 0; i < temp.size(); i++){
        meow.push_back(temp[i]);
    }
    std::sort(meow.begin(),meow.end());
    meow.erase(std::unique(meow.begin(),meow.end()),meow.end());
    finalBoards = meow;
    return finalBoards;
}
int main (int argc, char** argv) {
    
    std::clock_t start;
    double duration;
    start = std::clock();
    bool allSolutions;
    if(argc != 4){
        std::cerr << "Should have 4 arguments" << std::endl;
        return -1;
    }
    std::ifstream inputs(argv[1]);
    if(!inputs){
        std::cerr << "Could not open: " << argv[1] << std::endl;
        return -1;
    }
    std::ofstream output(argv[2]);
    if(!output){
        std::cerr << "Could not open: " << argv[2] << std::endl;
        return -1;
    }
    std::string argv3 = argv[3];
    if(argv3 == "all_solutions"){
        allSolutions = true;
    }else{
        allSolutions = false;
    }
    int cols;
    int rows;
    std::list<std::list<char> > addWords;
    std::list<std::list<char> > subWords;
    readFile(cols, rows, addWords, subWords, inputs);
    std::vector<std::vector<char> > blankBoard;
    makeBlankBoard(blankBoard, rows, cols);
    std::vector<std::vector<std::vector<char> > > allBoards;
    std::vector<std::vector<std::vector<char> > > FinalBoards;
    std::list<std::list<char> >::iterator wordItr = addWords.begin();
    placeAll(0, blankBoard, allBoards, wordItr, addWords);
    subBoards(allBoards, subWords);
    FinalBoards = finalChecks(allBoards);
    subBoards(FinalBoards, subWords);
    
    
    if(allSolutions){
        if(FinalBoards.size()>0){
            output << FinalBoards.size() << " solutions(s)"<< std::endl;
            for(int i = 0; i < FinalBoards.size(); i++){
                output << "Board: " << std::endl;
                outputBoard(output, FinalBoards[i]);
            }
        }else{
            output << "No solutions found";
        }
    }else{
        if(FinalBoards.size()>0){
            output << "Board: " << std::endl;
            outputBoard(output, FinalBoards[0]);
        }else{
            output << "No solutions found";
        }
    }
    std::cout << "Done" << std::endl;
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout << duration << std::endl;
    
    return 0;
}
