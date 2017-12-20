//
//  dan.cpp
//  hw6
//
//  Created by mike Levine on 3/20/17.
//  Copyright (c) 2017 mike Levine. All rights reserved.
//

//
//  main.cpp
//  HW6
//
//  Created by Daniel McCrevan on 3/19/17.
//  Copyright © 2017 Daniel McCrevan. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

void print_boards(const std::vector<std::vector<std::vector<char> > >& v, std::string file_name){
    std::ofstream output_file;
    output_file.open(file_name);
    output_file << v.size() << " solution(s)\n";
    for(int i = 0; i < v.size(); i++){
        output_file << "Board:\n";
        for(int j = 0; j < v[i].size(); j++){
            output_file << "  ";
            for(int k = 0; k < v[i][j].size(); k++){
                output_file << v[i][j][k];
            }
            output_file << "\n";
        }
    }
    
    
    
}
bool add_word(std::vector<std::vector<char> >& grid, const int dr, const int dc,
              const int pr, const int pc, const std::string& word){
    int c = 0;
    int i = pr;
    int j = pc;
    while (i < grid.size() && i >= 0 && j < grid[i].size() && j >= 0) {
        if(c == word.size()){
            return true;
        }
        if(grid[i][j] == '*' || grid[i][j] == word[c]){
            grid[i][j] = word[c];
            c++;
        }
        else{
            grid.clear();
            return false;
        }
        j+=dc;
        i+=dr;
    }
    if(c == word.size()){
        return true;
    }
    else{
        grid.clear();
        return false;
    }
    
}

void recurse_add_words(std::vector<std::vector<std::vector<char> > >& v, std::vector<std::vector<char> >& grid,
                       const std::vector<std::string>& words_to_include,
                       const std::vector<std::string>& words_not_to_include, int word_counter){
    if(word_counter == words_to_include.size()){
        v.push_back(grid);
        return;
    }
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[i].size(); j++){
            // East
            std::vector<std::vector<char> > grid_copy = grid;
            bool valid = add_word(grid_copy, 0, 1, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
            // South East
            grid_copy = grid;
            valid = add_word(grid_copy, 1, 1, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
            // South
            grid_copy = grid;
            valid = add_word(grid_copy, 1, 0, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
            // South West
            grid_copy = grid;
            valid = add_word(grid_copy, 1, -1, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
            // West
            grid_copy = grid;
            valid = add_word(grid_copy, 0, -1, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
            // North West
            grid_copy = grid;
            valid = add_word(grid_copy, -1, -1, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
            // North
            grid_copy = grid;
            valid = add_word(grid_copy, -1, 0, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
            // North East
            grid_copy = grid;
            valid = add_word(grid_copy, -1, 1, i, j, words_to_include[word_counter]);
            if(valid){
                recurse_add_words(v, grid_copy, words_to_include, words_not_to_include, word_counter+1);
            }
        }
    }
    
}



void recurse_filler_letter(std::vector<std::vector<std::vector<char> > >& v, std::vector<std::vector<char> >& grid, const std::string& ALPHABET){
    bool filled = true;
    for(int i = 0; i < grid.size() && filled; i++){
        for(int j = 0; j < grid[i].size(); j++){
            if(grid[i][j] == '*'){
                filled = false;
                break;
            }
        }
    }
    if(filled){
        v.push_back(grid);
        return;
    }
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[i].size(); j++){
            if(grid[i][j] == '*'){
                for(int k = 0; k < ALPHABET.size(); k++){
                    std::vector<std::vector<char> > grid_copy = grid;
                    grid_copy[i][j] = ALPHABET[k];
                    recurse_filler_letter(v, grid_copy, ALPHABET);
                }
            }
        }
    }
}

void remove_words(std::vector<std::vector<std::vector<char> > >& v, std::vector<std::vector<char> >& grid, const std::vector<std::string>& words_not_to_include, int word_counter, int v_counter){
    if(word_counter == words_not_to_include.size()){
        return;
    }
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[i].size(); j++){
            if(words_not_to_include[word_counter][0] == grid[i][j]){
                bool valid;
                // East
                std::vector<std::vector<char> > grid_copy = grid;
                std::vector<std::vector<char> > temp;
                valid = add_word(grid_copy, 0, 1, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
                // South East
                grid_copy = grid;
                valid = add_word(grid_copy, 1, 1, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
                // South
                grid_copy = grid;
                valid = add_word(grid_copy, 1, 0, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
                // South West
                grid_copy = grid;
                valid = add_word(grid_copy, 1, -1, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
                // West
                grid_copy = grid;
                valid = add_word(grid_copy, 0, -1, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
                // North West
                grid_copy = grid;
                valid = add_word(grid_copy, -1, -1, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
                // North
                grid_copy = grid;
                valid = add_word(grid_copy, -1, 0, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
                // South East
                grid_copy = grid;
                valid = add_word(grid_copy, -1, 1, i, j, words_not_to_include[word_counter]);
                if(valid){
                    v[v_counter] = temp;
                    return;
                }
            }
        }
    }
    remove_words(v, v[v_counter], words_not_to_include, word_counter+1, v_counter);
}

void remove_duplicates(std::vector<std::vector<std::vector<char> > >& v){
    for(int i = 0; i < v.size(); i++){
        for(int j = i+1; j < v.size(); j++){
            if(v[i] == v[j]){
                v.erase(v.begin()+j);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    const std::string ALPHABET = "abcdefghijklmnopqrstuvwxyz";
    // Verifies the input command
    if(argc != 4){
        std::cout << "Usage: " << argv[0] << " puzzle-file-name output-file-name solution-type\n";
        return 1;
    }
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){
        std::cout << "Usage: " << argv[0] << " puzzle-file-name output-file-name solution-type\n";
        return 1;
    }
    // Read in the file and parse the data into the dimensions & vectors of words
    int col;
    input_file >> col;
    int row;
    input_file >> row;
    std::string word;
    std::vector<std::string> words_to_include; // Words to include in grid
    std::vector<std::string> words_not_to_include; // Words not to include in grid
    while(input_file >> word){
        if(word == "+"){
            input_file >> word;
            words_to_include.push_back(word);
        }
        else if(word == "-"){
            input_file >> word;
            words_not_to_include.push_back(word);
        }
    }
    std::vector<std::vector<std::vector<char> > > v;
    std::vector<std::vector<char> > grid;
    for(int i = 0; i < row; i++){
        std::vector<char> line;
        for(int j = 0 ; j < col; j++){
            line.push_back('*');
        }
        grid.push_back(line);
    }
    int c = 0;
    recurse_add_words(v, grid, words_to_include, words_not_to_include, c);
    if(words_not_to_include.size() != 0){
        for(int i = 0; i < v.size(); i++){
            remove_words(v, v[i], words_not_to_include, 0, i);
        }
    }
    remove_duplicates(v);
    std::vector<std::vector<std::vector<char> > > v_filler;
    for(int i = 0; i < v.size(); i++){
        recurse_filler_letter(v_filler, v[i], ALPHABET);
    }
    
    remove_duplicates(v_filler);
    if(words_not_to_include.size() != 0){
        for(int i = 0; i < v_filler.size(); i++){
            remove_words(v_filler, v_filler[i], words_not_to_include, 0, i);
        }
    }
    std::vector<std::vector<std::vector<char> > > v_final;
    for(int i = 0; i < v_filler.size(); i++){
        if(v_filler[i].size() != 0){
            v_final.push_back(v_filler[i]);
        }
    }
    print_boards(v_final, argv[2]);
    return 0;
}
