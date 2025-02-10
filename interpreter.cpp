#include <iostream>
using namespace std;

class RessemblyInterpreter {
public:
    int memCells[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int currCell = 0;
    
    // Method to increment the value at the current memory cell
    void increment(int val) {
        memCells[currCell] += val;
        if (memCells[currCell] > 255)
            memCells[currCell] = 0;
        else if (memCells[currCell] < 0)
            memCells[currCell] = 255;
    }

    // Method to move to a different memory cell
    void move(int val) {
        currCell += val;
        if (currCell > 15)
            currCell = 0;
        else if (currCell < 0)
            currCell = 15;
    }

    // Method to output the character at the current memory cell
    char output() {
        cout << static_cast<char>(memCells[currCell]);
        return static_cast<char>(memCells[currCell]);
    }

    // Method to input a character and store its ASCII value in the current memory cell
    void input(char c) {
        memCells[currCell] = static_cast<int>(c);
    }

    int getCurrentValue() const {
        return memCells[currCell];
    }

    
    void reset(){
        for (int i = 0; i < 16; i++){
            memCells[i] = 0;
        }
       
        currCell = 0;
    }
};
