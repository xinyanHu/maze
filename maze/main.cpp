//
//  main.cpp
//  maze
//
//  Created by 胡欣妍 on 2017/10/30.
//  Copyright © 2017年 胡欣妍. All rights reserved.
//

#include <iostream>
#include <stack>
#include <string>
using namespace std;

template <typename T>
class Stack : public stack<T> {
public:
    T pop() {
        T tmp = stack<T>::top();
        stack<T>::pop();
        return tmp;
    }
};

class Cell {
    int x, y;
    friend class Maze;
public:
    Cell(int i = 0, int j = 0) : x(i), y(j){}
    
    bool operator ==(const Cell& c) const {
        return x == c.x && y == c.y;
    }
    
    bool operator !=(const Cell& c) const {
        return !(*this == c);
    }
    friend ostream& operator <<(ostream&os, const Cell& c) {
        return os << "(" << c.x << "," << c.y << ")";
    }
};

class Maze {
public:
    Maze();
    void exitMaze();
    
private:
    Cell currentCell, exitCell, entryCell;
    const char exitMarker, entryMarker, visited, passage, wall;
    Stack<Cell> mazeStack;
    char **store;
    void pushVisted(int, int);
    friend ostream& operator <<(ostream&, const Maze&);
    int rows, cols;
};

Maze::Maze() : exitMarker('e'), entryMarker('m'), visited('.'), passage('0'), wall('1') {
    Stack<char *> mazeRows;
    char str[80], *s;
    int row = 0;
    cout << "enter a Maze" << endl;
    
    while (cin >> str) {
        row++;
        cols = (int)strlen(str);
        s = new char[cols + 3];
        
        mazeRows.push(s);
        strcpy(s + 1, str);
        s[0] = s[cols + 1] = wall;
        s[cols + 2] = '\0';
        
        if (strchr(s, exitMarker) != 0) {
            exitCell.x = row;
            exitCell.y = static_cast<int>(strchr(s, exitMarker) - s);
        }
        if (strchr(s, entryMarker) != 0) {
            entryCell.x = row;
            entryCell.y = static_cast<int>(strchr(s, entryMarker) - s);
        }
    }
    rows = row;
    store = new char*[rows + 2];
    store[0] = new char[cols + 3];
    for (; !mazeRows.empty(); row--) {
        store[row] = mazeRows.pop();
    }
    store[rows + 1] = new char[cols + 3];
    store[0][cols + 2] = store[rows][cols + 2] = '\0';
    
    for (int col = 0; col < cols + 2; col++) {
        store[0][col] = wall;
        store[rows + 1][col] = wall;
    }
}

void Maze::pushVisted(int row, int col) {
    if (store[row][col] == passage || store[row][col] == exitMarker) {
        mazeStack.push(Cell(row, col));
    }
}

ostream& operator <<(ostream& os, const Maze& maze) {
    for (int row = 0; row < maze.rows + 2 ; row++) {
        os << maze.store[row] << endl;
    }
    os << endl;
    return os;
}

void Maze::exitMaze() {
    int row, col;
    Cell last;
    last = currentCell = entryCell;
    
    while (currentCell != exitCell) {
        row = currentCell.x;
        col = currentCell.y;
        cout << *this;
        
        if (currentCell != entryCell && last != entryCell) {
//            store[row][col] = visited;
            cout << currentCell;
            if (abs(currentCell.x - last.x) == 1)
                store[last.x][last.y] = '|';
            if (abs(currentCell.y - last.y) == 1)
                store[last.x][last.y] = '-';
        }
        
        if (currentCell.x == exitCell.x && currentCell.y > exitCell.y) {
           pushVisted(row, col - 1);
        }
        else {
            pushVisted(row, col - 1);
            pushVisted(row - 1, col);
            pushVisted(row + 1, col);
            pushVisted(row, col + 1);
        }
        
        if (mazeStack.empty()) {
            cout << "error" << endl;
        }
        else {
            last = currentCell;
            currentCell = mazeStack.pop();
        }
    }
    if (abs(currentCell.x - last.x) == 1)
        store[last.x][last.y] = '|';
    if (abs(currentCell.y - last.y) == 1)
        store[last.x][last.y] = '-';
    cout << "success!!!" << endl;
    cout << *this;
}

int main(int argc, const char * argv[]) {
    Maze maze;
    maze.exitMaze();
    return 0;
}
