//
//  suduku.cpp
//  suduku
//
//  Created by Kevin Yu on 5/6/15.
//  Copyright (c) 2015 Calvin Yu. All rights reserved.
//

#include "suduku.h"
#include "game.h"
#include <stdlib.h>

// construct, do noting
Sudoku::Sudoku() {
    history = "history.txt";
    loadHistory();
}

void Sudoku::loadHistory() {
    // history format <time> <total>
    history_data.clear();
    for (int i = 0; i < 4; i++) {
        history_data.push_back(0);
        history_data.push_back(0);
    }
    ifstream ifs(history.c_str());
    for (int i = 0; i < 4; i++) {
        int times = 0, total = 0;
        ifs >> times >> total;
        history_data[i * 2] = times;
        history_data[i * 2 + 1] = total;
    }
    ifs.close();
}

void Sudoku::saveHistory() {
    ofstream ofs(history.c_str());
    for (int i = 0; i < 8; i++) {
        ofs << history_data[i] << endl;
    }
    ofs.close();
}

// start game
void Sudoku::startGame() {
    // prompt user to select menu
    int select = 0;
    do {
        do {
            // clear and move to (0,0)
            cout << "\033[2J\033[0;0H" << endl;
            
            cout << "\t1. very easy"<< endl;
            cout << "\t2. easy"<< endl;
            cout << "\t3. medium"<< endl;
            cout << "\t4. hard"<< endl;
            cout << "\t5. list history"<< endl;
            cout << "\t6. quit" << endl;
            cout << "Please select: ";
            
            cin >> select;
            if (!cin || select <= 0 || select > 6) {
                cin.clear();
                cin.ignore();
            } else {
                break;
            }
        } while(1);
        switch (select) {
            case 6:
                cout << "Godbye!" << endl;
                return;
            case 5:
                listHistory();
                break;
            default:
                newGame(select);
        }
    } while (1);
}

// list history
void Sudoku::listHistory() {
    // clear and move to (0,0)
    char ch;
    cout << "\033[2J\033[0;0H" << endl;
    cout << "very easy mode: times=" << history_data[0] << " total=" << history_data[1]
    << " agv=" << (history_data[0] > 0 ? history_data[1]/history_data[0] : 0 )<< endl;
    cout << "easy mode: times=" << history_data[2] << " total=" << history_data[3]
    << " agv=" << (history_data[2] > 0 ? history_data[3]/history_data[2] : 0)<< endl;
    cout << "medium mode: times=" << history_data[4] << " total=" << history_data[5]
    << " agv=" << (history_data[4] > 0 ? history_data[5]/history_data[4] : 0 )<< endl;
    cout << "hard mode: times=" << history_data[6] << " total=" << history_data[7]
    << " agv=" << (history_data[6] > 0 ? history_data[7]/history_data[6] : 0 )<< endl;
    
    cout << endl << "Please any key to return";
    cin >> ch;
    cin.clear();
}

// start new game
void Sudoku::newGame(int mode) {
    // random filename base on mode
    srand( (unsigned)time(NULL));
    int rnd = random() % 10 + (mode - 1) * 10;
    char filename[1024];
    snprintf(filename, sizeof(filename), "data/%d.txt", rnd);
    
    Game game(filename);
    int score = game.start();
    if (score >= 0) {
        //save history
        history_data[(mode - 1) * 2] += 1;
        history_data[(mode - 1) * 2 + 1] += score;
        saveHistory();
    }
}
