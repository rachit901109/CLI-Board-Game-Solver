#include"lights_out_utils.h"
#include<vector>
#include<algorithm>
#include<cstdlib>
using namespace std;

// all true values
bool final[MAX][MAX] = {{true, true, true, true}, {true, true, true, true}, {true, true, true, true}, {true, true, true, true}};

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

void add_nodes(std::ofstream &dotfile, bool initial[][MAX], vector<vector<int>>& all_moves, int solution_index)
{
    dotfile << get_string(initial) << " [label=\"" << get_label(initial) << "\"]" << endl;
    for(int i=0;i<all_moves[solution_index].size();i++)
    {
        int move = all_moves[solution_index][i];
        int x = move/MAX;
        int y = move%MAX;
        toggle(initial, x, y);
        dotfile << get_string(initial) << " [label=\"" << get_label(initial) << "\"]" << endl;
    }
}

void add_edges(std::ofstream &dotfile, bool initial[][MAX], vector<vector<int>>& all_moves, int solution_index)
{
    for(int i=all_moves[solution_index].size()-1;i>=0;i--)
    {
        int move = all_moves[solution_index][i];
        int x = move/MAX;
        int y = move%MAX;
        dotfile << get_string(initial) << " -> ";
        toggle(initial, x, y);
        dotfile << get_string(initial) << " [label=\"(" << x << "," << y << ")\"]" << endl;
    }
}

void draw(vector<vector<int>>& all_moves, bool initial[][MAX], int solution_index)
{
    std::ofstream dotfile("lightsout_solution.dot");
    if (!dotfile) {
        std::cerr << "Error opening DOT file." <<endl;
        return ;
    }
    write_header(dotfile);
    add_nodes(dotfile, initial, all_moves, solution_index);
    add_edges(dotfile, initial, all_moves, solution_index);
    write_footer(dotfile);
    dotfile.close();

    system("dot -Tsvg lightsout_solution.dot -o lo_solution.svg");
    cout<<"SVG file saved in current directory";
    return;
}

int main()
{
    // initial state
    bool initial[MAX][MAX] = {{false, true, true, true}, {false, false, false, true}, {false, true, false, true}, {false, false, false, false}};
    cout<<"Initial state: "<<endl;
    print_mat(initial);

    // all possible moves for MAX x MAX grid ie all subsequences vector cells
    vector<int> cells;
    for (int i = 0; i < MAX*MAX; i++)
    {
        cells.push_back(i);
    }
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
        // untoogle all cells in the move
        for(int j=0;j<all_moves[i].size();j++)
        {
            int move = all_moves[i][j];
            int x = move/MAX;
            int y = move%MAX;
            toggle(initial, x, y);
        }

    }

    cout<<"Solution index: "<<solution_index<<endl;
    cout<<"Solution sequence: "<<endl;
    for(int i=0;i<all_moves[solution_index].size();i++)
    {
        cout<<all_moves[solution_index][i]<<" ";
    }
    cout<<endl;

    draw(all_moves, initial, solution_index);

    return 0;
}