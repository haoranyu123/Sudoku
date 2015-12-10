//
//  suduku.h
//  suduku
//
//  Created by Kevin Yu on 5/6/15.
//  Copyright (c) 2015 Calvin Yu. All rights reserved.
//

#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Sudoku {
public:
    Sudoku();
    void startGame();
private:
    void listHistory();
    void loadHistory();
    void saveHistory();
    void newGame(int mode);
    string history;
    vector<int> history_data;
};

#endif
