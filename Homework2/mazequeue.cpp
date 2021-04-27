//
//  mazequeue.cpp
//  Homework2
//
//  Created by Edmund Zhi on 4/23/21.
//
#include <queue>
#include <iostream>
using namespace std;

class Coord {
public:
    Coord(int r, int c) : m_row(r), m_col(c) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
private:
    int m_row;
    int m_col;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec) {
    queue<Coord> coordQueue;
    Coord start(sr,sc);
    Coord end(er,ec);
    coordQueue.push(start);
    // 1 indicates that algorithm has encountered coord
    maze[sr][sc] = '1';
    while (!coordQueue.empty()) {
        Coord current = coordQueue.front();
        coordQueue.pop();
        cout << "(" << current.r() << "," << current.c() << ")" << endl;
        int r = current.r();
        int c = current.c();
        if ((r == end.r()) && (c == end.c())) {
            return true;
        }
        //south
        if (maze[r+1][c] == '.') {
            coordQueue.push(Coord(r+1,c));
            maze[r+1][c] = '1';
        }
        //west
        if (maze[r][c-1] == '.') {
            coordQueue.push(Coord(r,c-1));
            maze[r][c-1] = '1';
        }
        //north
        if (maze[r-1][c] == '.') {
            coordQueue.push(Coord(r-1,c));
            maze[r-1][c] = '1';
        }
        //east
        if (maze[r][c+1] == '.') {
            coordQueue.push(Coord(r,c+1));
            maze[r][c+1] = '1';
        }
    }
    return false;
}

int main() {
    char maze[10][10] = {
        { 'X','X','X','X','X','X','X','X','X','X' },
        { 'X','.','.','.','.','X','.','.','X','X' },
        { 'X','.','X','X','.','X','X','.','.','X' },
        { 'X','X','X','.','.','.','.','X','.','X' },
        { 'X','.','X','X','X','.','X','X','X','X' },
        { 'X','.','X','.','.','.','X','.','.','X' },
        { 'X','.','.','.','X','.','X','.','.','X' },
        { 'X','X','X','X','X','.','X','.','X','X' },
        { 'X','.','.','.','.','.','.','.','.','X' },
        { 'X','X','X','X','X','X','X','X','X','X' }
    };

    if (pathExists(maze, 3,5, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}


