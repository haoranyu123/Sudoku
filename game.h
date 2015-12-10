//
//  game.h
//  suduku
//
//  Created by Kevin Yu on 5/6/15.
//  Copyright (c) 2015 Calvin Yu. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Game {
public:
    Game(const char *fn);
    // return score
    int start();
    
private:
    // output board
    void print(int pos = -1, int num = 0);
    
    // use by fill
    bool test(int pos);
    // fill board
    void fill();
    
    string filename;
    // save data on board (81 elements)
    vector<int> board;
    // control field on board show or hidden (81 elements)
    // 0: empty 1:init 2:input
    vector<int> mask;
    int score;
};

#endif