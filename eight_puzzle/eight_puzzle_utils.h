#ifndef EIGHT_PUZZLE_UTILS_H
#define EIGHT_PUZZLE_UTILS_H

#include<iostream>
#include<cstring>
#include<fstream>
#define MAX 3
using namespace std;

string get_string(int mat[MAX][MAX])
{
    string re = "";
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            re+=to_string(mat[i][j]);
        }
    }
    return re;
    
}

string get_label(int mat[MAX][MAX])
{
    string re = "";
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            re+=to_string(mat[i][j]);
        }
    re+="\n";
    }
    return re;
}

bool is_valid_move(int movex,int movey)
{
    if(movex<0 || movex>=MAX || movey<0 || movey>=MAX)
    {
        return false;
    }
    return true;
}

void print_mat(int mat[][MAX])
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            cout<<mat[i][j]<<" ";
        }
    cout<<endl;
    }
}

void write_header(std::ofstream &dotfile)
{
    dotfile << "digraph path{" <<endl;
    dotfile << "node [shape=circle];"<<endl;
    return;
}

void write_footer(std::ofstream &dotfile)
{
    dotfile << "}" <<endl;
    return;
}

#endif