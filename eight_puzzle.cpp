#include<iostream>
#include<queue>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<memory>
#include<cstring>
using namespace std;
#define MAX 3

// Solvability: - https://intellipaat.com/community/6144/8-puzzle-solvability-and-shortest-solution

/*
8-puzzle has a property that exactly half (of all) permutations can be reached from any starting state. 
9! is the total number of possible configurations of the puzzle, whereas 9!/2 is the total number of solvable configurations. 
The simple explanation is that exactly half the configurations aren't possible to reach from any chosen starting point
(or alternatively, can't reach any randomly chosen ending point), so we divide total configurations (9!)/2 to get the 
total number of states that can move step by step to the solution.
*/

// using shared pointer can reduce task of explicit memory management, shared pointers use reference counting to maintain a counter 
// on number of owners(pointers) of the object. Deallocation happens when the last remaining pointer owning the object is destriyed
// Smart Pointers: - https://en.cppreference.com/w/cpp/memory/shared_ptr

int final[MAX][MAX] = {{1,2,3},{4,5,6},{7,8,0}};
int cordx[] = {-1,0,1,0};
int cordy[] = {0,-1,0,1};

struct Node {
    int mat[MAX][MAX];
    int x, y, depth, cost;
    shared_ptr<Node> parent; 
};

shared_ptr<Node> newnode(int mat[MAX][MAX], int x, int y, int nx, int ny, int depth, const shared_ptr<Node>& parent) {
    auto t = make_shared<Node>();
    memcpy(t->mat, mat, MAX * MAX * sizeof(mat[0][0]));
    swap(t->mat[x][y], t->mat[nx][ny]);
    t->x = nx;
    t->y = ny;
    t->depth = depth;
    t->parent = parent;
    t->cost = INT16_MAX;

    return t;
}

int cost(int mat[MAX][MAX])
{
    int s = 0;
    for(int i=0;i<MAX;i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            if(mat[i][j]!=0 && mat[i][j]!=final[i][j])
            {s++;}
        }
    }
    return s;
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

void write_header(std::ofstream &dotfile)
{
    dotfile << "digraph path{" <<endl;
    dotfile << "node [shape=circle];"<<endl;
    return;
}

void add_nodes(std::ofstream &dotfile, const shared_ptr<Node>& root)
{
    if(root == NULL)
    {return;}
    add_nodes(dotfile,root->parent);
    dotfile << get_string(root->mat)+" [label="+" \""+get_label(root->mat)+"\"]" <<endl;
}

void add_edges(std::ofstream &dotfile, const shared_ptr<Node>& root)
{
    shared_ptr<Node> temp = root->parent;
    if(temp == NULL)
    {return;}
    add_edges(dotfile,root->parent);
    dotfile << get_string(temp->mat)+" -> "+get_string(root->mat)<<endl;
}


void write_footer(std::ofstream &dotfile)
{
    dotfile << "}" <<endl;
    return;
}

void draw_path(const shared_ptr<Node>& root)
{
    std::ofstream dotfile("eight_puzzle_solution.dot");
    if (!dotfile) {
        std::cerr << "Error opening DOT file." <<endl;
        return ;
    }
    write_header(dotfile);
    shared_ptr<Node> save = root;   
    add_nodes(dotfile,root);
    add_edges(dotfile,save);
    write_footer(dotfile);
    dotfile.close();

    system("dot -Tsvg eight_puzzle_solution.dot -o 8puzzle_solution.svg");
    cout<<"SVG file saved in current directory"<<endl;
    return;
}



bool is_valid_move(int movex,int movey)
{
    if(movex<0 || movex>=MAX || movey<0 || movey>=MAX)
    {
        return false;
    }
    return true;
}

struct cmp
{
    bool operator()(const shared_ptr<Node>& lhs, const shared_ptr<Node>& rhs) const
    {
        return (lhs->cost+lhs->depth > rhs->cost+rhs->depth);
    }
};

void solve(int initial[MAX][MAX],int x,int y,int depth_limit)
{
    shared_ptr<Node> root = newnode(initial,x,y,x,y,0,NULL);
    root->cost = cost(root->mat);
    int current_depth=root->depth;

    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, cmp>pq;
    pq.push(root);

    while (!pq.empty() && current_depth<=depth_limit)
    {
        shared_ptr<Node> min_node = pq.top();

        pq.pop();
        
        if(min_node->cost == 0)
        {
            draw_path(min_node);
            return;
        }

        current_depth = min_node->depth;
        for(int i=0; i<4; i++)
        {
            if(is_valid_move(min_node->x+cordx[i],min_node->y+cordy[i]))
            {
                shared_ptr<Node> t = newnode(min_node->mat,min_node->x,min_node->y,min_node->x+cordx[i],min_node->y+cordy[i],min_node->depth+1,min_node);
                t->cost = cost(t->mat);
                pq.push(t);
            }
        }
    }
    cout<<"\nDepth Limit reached! Couldn't find solution"<<endl;
    return;
}

int count_inversion(int initial[MAX][MAX])
{
    int ic=0;
    for(int i=0;i < MAX*MAX-1; i++)
    {
        for(int j=i+1; j<MAX*MAX; j++)
        {
            if(initial[i/MAX][i%MAX]!=0 && initial[j/MAX][j%MAX]!=0 && initial[i/MAX][i%MAX] > initial[j/MAX][j%MAX])
            {
                ic++;
            }
        }
    }
    return ic;
}


int main()
{
    // valid puzzles
    int initial[MAX][MAX] ={{1,3,2},
                            {5,8,7},
                            {6,0,4}};

    // int initial[MAX][MAX] ={{1,2,3},
    //                         {4,0,5},
    //                         {7,8,6}};

    // int initial[MAX][MAX] ={{1,3,0},
    //                         {4,2,6},
    //                         {7,5,8}};         

    // int initial[MAX][MAX] ={{1,2,3},
    //                         {4,5,6},
    //                         {7,0,8}};

    // int initial[MAX][MAX] ={{5,3,8},
    //                         {7,4,1},
    //                         {0,6,2}};

    // int initial[MAX][MAX] ={{2,7,4},
    //                         {1,0,3},
    //                         {6,8,5}};

    // invalid puzzle
    // int initial[MAX][MAX] ={{2,8,5},
    //                         {1,6,4},
    //                         {3,0,7}};

    int cx,cy,dl;
    // starting position of 0 and depth limit
    cx=2;cy=1;dl=50;
    
    int inversion_count = count_inversion(initial);
    if(inversion_count%2==0)
    {
        cout<<"\nPuzzle is solvable. Inversion count: "<<inversion_count<<endl;
        solve(initial,cx,cy,dl);
    }
    else
    {
        cout<<"\nInitial state is not solvable. Inversion count: "<<inversion_count<<endl;
    }
return 0;
}