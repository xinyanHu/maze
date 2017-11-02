//
//  main.cpp
//  Maze 2.0
//
//  Created by 胡欣妍 on 2017/10/30.
//  Copyright © 2017年 胡欣妍. All rights reserved.
//

#include <iostream>
#include <stack>
#include <string>
#include <deque>
#include <cmath>
#include <iomanip>
using namespace std;

const char exitMarker = 'e', entryMarker = 'm', visited = '.', passage = '0', wall = '1';

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
//    public:
    int x, y;
    long distance = -1;
    friend class Maze;
public:
    Cell(int i = 0, int j = 0, long d = -1) : x(i), y(j), distance(d){}
    
    bool operator ==(const Cell& c) const {
        return x == c.x && y == c.y;
    }
    
    friend ostream& operator <<(ostream& os, const Cell& c) {
        return os << "(" << c.x << "," << c.y << ")";
    }
    
    bool operator !=(const Cell& c) const {
        return !(*this == c);
    }
    
    friend bool sortCriterion(const Cell&, const Cell&);
};

bool sortCriterion(const Cell& c1, const Cell& c2) {
    return c1.distance <= c2.distance;
}

struct Node {
    char c = wall;
    long dis = -1;
    
    friend ostream& operator <<(ostream& os, const Node& n) {
        return os << n.c;
    }
};

class Maze {
public:
    Maze();
    void exitMaze();
    
private:
    int rows, cols;
    Cell currentCell, exitCell, entryCell;
    Stack<Cell> mazeStack;
    deque<deque<Node>> s;
    
    void pushVisted(int, int);
    friend ostream& operator <<(ostream&, const Maze&);
};

ostream& operator <<(ostream& os, const Maze& maze) {
    for (int i = 0; i < maze.s.size(); i++) {
        for (int j = 0; j < maze.cols + 2; j++) {
            os << maze.s[i][j];
        }
        os << endl;
    }
    os << endl;
    return os;
}

Maze::Maze() {
    int row = 0;
    string str;
    
    while (cin >> str) {
        row++;
        cols = (int) str.length();
        deque<Node> d(cols);
        for (int i = 0; i <= cols; i++) {
            d[i].c = str[i];
        }
        d.insert(d.begin(), Node());
        d.insert(d.end(), Node());
        s.insert(s.end(), d);
        if (str.find(exitMarker) != string::npos)
            exitCell = Cell(row, (int) str.find(exitMarker) + 1);
        if (str.find(entryMarker) != string::npos)
            entryCell = Cell(row, (int) str.find(entryMarker) + 1);
    }
    deque<Node> d(cols + 2);
    s.insert(s.begin(), d);
    s.insert(s.end(), d);
    
    cout << exitCell << endl;
    cout << entryCell << endl;
    rows = row;
    
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            long dis = pow(exitCell.x - i, 2) +  pow(exitCell.y - j, 2);
            s[i][j].dis = dis;
        }
    }
}

void Maze::pushVisted(int row, int col) {
    if ((s[row][col].c == passage || s[row][col].c == exitMarker)) {
        mazeStack.push(Cell(row, col));
    }
}

void Maze::exitMaze() {
    int row, col;
    currentCell = entryCell;
    
    while (currentCell != exitCell) {
        row = currentCell.x;
        col = currentCell.y;
        cout << *this;
        
        if (currentCell != entryCell) {
            s[row][col].c = visited;
            cout << currentCell << endl;
        }
        
        deque<Cell> d = {
            Cell(row, col - 1, s[row][col - 1].dis),
            Cell(row, col + 1, s[row][col + 1].dis),
            Cell(row + 1, col, s[row + 1][col].dis),
            Cell(row - 1, col, s[row - 1][col].dis),
        };
        sort(d.begin(), d.end(), sortCriterion);
        for (int i = 3; i >=0; i--) {
            pushVisted(d[i].x, d[i].y);
        }
        
        if (mazeStack.empty()) {
            cout << "error" << endl;
        }
        else
            currentCell = mazeStack.pop();
    }
    cout << "success!!!" << endl;
    cout << *this;
}


int main(int argc, const char * argv[]) {
    Maze maze;
    maze.exitMaze();
    return 0;
}
