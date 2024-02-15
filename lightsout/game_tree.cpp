#include"lights_out_utils.h"
#include<vector>
#include <map>
#include<algorithm>
#include<cstdlib>
using namespace std;

void add_nodes_and_edges(std::ofstream &dotfile, bool initial[][MAX], vector<vector<int>>& all_moves, map<string, bool> visited_nodes, map<string, string> drawn_edges)
{
    // add initial state
    dotfile << get_string(initial) << " [label=\"" << get_label(initial) << "\"]" << endl;
    visited_nodes[get_string(initial)] = true;
    
    // traverse all moves and add nodes
    for (int i = 0; i < all_moves.size(); i++)
    {
        // toggle all cells in the move
        for (int j = 0; j < all_moves[i].size(); j++)
        {
            bool prev[MAX][MAX];
            copy(&initial[0][0], &initial[0][0]+MAX*MAX, &prev[0][0]);
            int x = all_moves[i][j]/MAX;
            int y = all_moves[i][j]%MAX;
            toggle(initial, x, y);

            // add node if not visited
            string node = get_string(initial);
            if(visited_nodes.find(node) == visited_nodes.end())
            {
                visited_nodes[node] = true;
                dotfile << node << " [label=\"" << get_label(initial) << "\"]" << endl;
            }
            // add edge if not drawn
            string prev_node = get_string(prev);
            if(drawn_edges.find(prev_node) == drawn_edges.end())
            {
                drawn_edges[prev_node] = node;
                dotfile << prev_node << " -> " << node << " [label=\"(" << x << "," << y << ")\"]" << endl;
            }
            else if(drawn_edges[prev_node] != node)
            {
                drawn_edges[prev_node] = node;
                dotfile << prev_node << " -> " << node << " [label=\"(" << x << "," << y << ")\"]" << endl;
            }
        }
        // untoggle all cells in the move
        for (int j = 0; j < all_moves[i].size(); j++)
        {
            int x = all_moves[i][j]/MAX;
            int y = all_moves[i][j]%MAX;
            toggle(initial, x, y);
        }
    }
}

int main()
{
    // initial state
    bool initial[MAX][MAX] = {{true, true, true}, {true, true, true}, {true, true, true}};
    cout<<"Initial state: "<<endl;
    print_mat(initial);

    // all possible moves for 3x3 grid ie all subsequences vector cells
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

    map<string, bool> visited_nodes;
    map<string, string> drawn_edges;

    // draw game tree of all moves
    std::ofstream dotfile("game_tree.dot");
    write_header(dotfile);
    add_nodes_and_edges(dotfile, initial, all_moves, visited_nodes, drawn_edges);
    write_footer(dotfile);
    dotfile.close();

    // convert dot file to svg
    system("dot -Tsvg game_tree.dot -o game_tree.svg");
    cout<<"SVG file saved in current directory";

    return 0;
}