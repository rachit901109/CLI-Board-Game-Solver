#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
#define MAX 3

// all true values
bool final[MAX][MAX] = {{true, true, true}, {true, true, true}, {true, true, true}};
int cordx[] = {-1,0,1,0};
int cordy[] = {0,-1,0,1};

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
    if(is_valid(i, j))
    {
        mat[i][j] = !mat[i][j];
    }
    for (int k = 0; k < 4; k++)
    {
        int x = i+cordx[k];
        int y = j+cordy[k];
        if(is_valid(x, y))
        {
            mat[x][y] = !mat[x][y];
        }
    }
}

bool reached_final(bool mat[][MAX])
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            if(!(mat[i][j] & final[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}


int main()
{
    // initial state
    bool initial[MAX][MAX] = {{true, true, true}, {true, true, true}, {false, false, false}};
    bool ini_copy[MAX][MAX];
    copy(&initial[0][0], &initial[0][0]+MAX*MAX, &ini_copy[0][0]);

    // all possible moves for 3x3 grid ie all subsequences vector cells
    vector<int> cells = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    vector<vector<int>> all_moves;
    int n = cells.size();
    int move_len = 1<<n;

    for (int i = 0; i < move_len; i++)
    {
        vector<int> temp;
        for (int j = 0; j < n; j++)
        {
            if(i & (1<<j))
            {
                temp.push_back(j);
            }
        }
        all_moves.push_back(temp);
    }

    // sort all moves by size and lexicographically
    auto comparator = [](const vector<int>& a, const vector<int>& b) {
        if (a.size() != b.size()) {
            return a.size() < b.size();  // Compare by size first
        }
        return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    };
    sort(all_moves.begin(), all_moves.end(), comparator);

    // perform all moves
    int solution_index;
    for(int i=0;i<move_len;i++)
    {
        // toggle all cells in the move
        for(int j=0;j<all_moves[i].size();j++)
        {
            int move = all_moves[i][j];
            int x = move/MAX;
            int y = move%MAX;
            toggle(initial, x, y);
        }
        if(reached_final(initial))
        {
            cout<<"Reached final state"<<endl;
            solution_index = i;
            break;
        }
        // ]untoogle all cells in the move
        for(int j=0;j<all_moves[i].size();j++)
        {
            int move = all_moves[i][j];
            int x = move/MAX;
            int y = move%MAX;
            toggle(initial, x, y);
        }

    }

    cout<<"Solution index: "<<solution_index<<endl;
    cout<<"Solution sequence:"<<endl;
    for(int i=0;i<all_moves[solution_index].size();i++)
    {
        cout<<all_moves[solution_index][i]<<" ";
    }
    cout<<endl;


    return 0;
}