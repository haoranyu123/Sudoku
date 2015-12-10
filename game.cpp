//
//  game.cpp
//  suduku
//
//  Created by Kevin Yu on 5/6/15.
//  Copyright (c) 2015 Calvin Yu. All rights reserved.
//

#include "game.h"
#include <stdlib.h>
#include <unistd.h>

Game::Game(const char *fn) : filename(fn) {
    score = 100;
}

int Game::start() {
    // load data from file
    ifstream ifs(filename.c_str());
    int in = 0;
    for (int i = 0; i < 81; i++) {
        ifs >> in;
        if (!ifs || in < 0 || in > 9) {
            cerr << "Load data from file failed: " << filename << endl;
            exit(-1);
        }
        board.push_back(in);
        if (in) {
            mask.push_back(1);
        } else {
            mask.push_back(0);
        }
    }
    ifs.close();
    
    // fill empty field
    fill();
    
    
    // get user input
    string input;
    do {
        // print board
        print();
        cout << endl << "You can input like this:" << endl;
        cout << "\t1. <row><col><num> (row, col, num in range 1-9, e.g. 129)" << endl;
        cout << "\t2. test (just for test, fill a empty field auto)" << endl;
        cout << "\t3. quit (quit this game)" << endl;
        cout << "Please input your action: ";
        cin.clear();
        cin.ignore();
        cin >> input;
        if (!cin) {
            cin.clear();
            cin.ignore();
            cout << "invalid action" << endl;
            sleep(1);
            continue;
        }
        if (input == "quit") {
            return -1;
        }
        if (input == "test") {
            for (int i = 0; i < 81; i++) {
                if (mask[i] == 0) {
                    int r = i / 9;
                    int c = i % 9;
                    int n = board[i];
                    input.clear();
                    input.append(1, '1' + r).append(1, '1' + c).append(1, '0' + n);
                }
            }
        }
        if (input.size() != 3 || !(input[0] >= '1' && input[0] <= '9')
            || !(input[1] >= '1' && input[1] <= '9')
            || !(input[2] >= '1' && input[2] <= '9')) {
            cout << "invalid action: " << input << endl;
            sleep(1);
            continue;
        }
        int row = input[0] - '1';
        int col = input[1] - '1';
        int num = input[2] - '0';
        int pos = row * 9 + col;
        if (mask[pos] != 0) {
            cout << "field not empty" << endl;
            sleep(1);
            continue;
        }
        if (num != board[pos]) {
            score -= 5;
            score = score < 0 ? 0 : score;
            print(pos, num);
            cout << endl << "error" << endl;
            sleep(5);
        } else {
            mask[pos] = 2;
            bool end = true;
            for (int i = 0; i < 81; i++) {
                if (mask[i] == 0) {
                    end = false;
                }
            }
            if (end) {
                cout << "Your get \033[5m\033[33m" << score << "\033[0m in this game!" << endl;
                sleep(5);
                return score;
            }
        }
    } while(1);
}

bool Game::test(int pos) {
    int row = pos / 9;
    int col = pos % 9;
    
    // test row
    for (int i = 0; i < 9; i++) {
        int n = row * 9 + i;
        if (n != pos && board[n] == board[pos]) {
            return false;
        }
    }
    
    // test col
    for (int i = 0; i < 9; i++) {
        int n = i * 9 + col;
        if (n != pos && board[n] == board[pos]) {
            return false;
        }
    }
    
    // test block
    int block_row = row / 3;
    int block_col = col / 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int n = (block_row * 3 + i) * 9 + block_col * 3 + j;
            if (n != pos && board[n] == board[pos]) {
                return false;
            }
        }
    }
    return true;
}

void Game::fill() {
    // first get empty field
    vector<int>poss;
    for (int i = 0; i < 81; i++) {
        if (board[i] == 0) {
            poss.push_back(i);
        }
    }
    
    // test each empty field
    int i = 0;
    int pos = 0;
    int flag = false;
    while (i >= 0 && i < (int)poss.size()) {
        pos = poss[i];
        flag = false;
        while (board[pos] < 9) {
            board[pos] += 1;
            if (test(pos)) {
                flag = true;
                break;
            }
        }
        if (flag) {
            // success, test next empty field
            i++;
        } else {
            // failed, trace back
            board[pos] = 0;
            i--;
        }
    }
    
    if (i < 0) {
        cerr << "data invalid" << endl;
        exit(-1);
    }
}

void Game::print(int blink_pos, int num) {
    // clear and move to (0,0)
    cout << "\033[2J\033[0;0H" << endl;
    
    cout << "(Data from " << filename << ")" << endl;
    cout << "Current score: \033[33m" << score << "\033[0m" << endl;
    cout << "    1 2 3   4 5 6   7 8 9" << endl;
    cout << "  +-------+-------+-------+" << endl;
    for (int i = 0; i < 9; i++) {
        cout << i + 1 << " | ";
        for (int j = 0; j < 9; j++) {
            int pos = i * 9 + j;
            int n = board[pos];
            if (pos == blink_pos) {
                cout << "\033[5m\033[32m" << num << "\033[0m";
            } else if (mask[pos] == 0) {
                cout << 0;
            } else if (mask[pos] == 1) {
                cout << "\033[34m" << n << "\033[0m";
            } else {
                cout << "\033[32m" << n << "\033[0m";
            }
            if (!mask[pos]) {
                n = 0;
            }
            cout << " ";
            if (j % 3 == 2) {
                cout << "| ";
            }
        }
        cout << endl;
        if (i % 3 == 2) {
            cout << "  +-------+-------+-------+" << endl;
        }
    }
}
