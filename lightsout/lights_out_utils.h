#ifndef LIGHTS_OUT_UTILS_H
#define LIGHTS_OUT_UTILS_H

#include<iostream>
#include<fstream>
#include<cstring>
#define MAX 3
// #define MAX 4 for lightsout/lightsout.cpp generating game tree for 4x4 grid killed the kernel
using namespace std;

int cordx[] = {0,-1,0,1,0};
int cordy[] = {0,0,-1,0,1};

void print_mat(bool mat[][MAX])
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

bool is_valid(int i, int j)
{
    return i>=0 && i<MAX && j>=0 && j<MAX;
}

void toggle(bool mat[][MAX], int i, int j)
{
    for (int k = 0; k < 5; k++)
    {
        int x = i+cordx[k];
        int y = j+cordy[k];
        if(is_valid(x, y))
        {
            mat[x][y] = !mat[x][y];
        }
    }
}

string get_string(bool mat[][MAX])
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

string get_label(bool mat[][MAX])
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

void write_header(std::ofstream &dotfile)
{
    dotfile << "digraph tree{" <<endl;
    dotfile << "node [shape=circle];"<<endl;
    return;
}

void write_footer(std::ofstream &dotfile)
{
    dotfile << "}" <<endl;
    return;
}

#endif