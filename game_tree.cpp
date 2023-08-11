#include<iostream>
#include<fstream>
#include<cstdlib>
#include<memory>
#include<cstring>
#define MAX 3
using namespace std;



struct TreeNode
{
    int mat[MAX][MAX];
    int x,y,px,py,depth;
    shared_ptr<TreeNode> left;
    shared_ptr<TreeNode> right;
    shared_ptr<TreeNode> top;
    shared_ptr<TreeNode> bottom;
};


shared_ptr<TreeNode> newnode(int mat[MAX][MAX],int x,int y,int nx,int ny,int depth)
{
    auto t = make_shared<TreeNode>();
    memcpy(t->mat,mat,MAX*MAX*sizeof(mat[0][0]));
    t->px = x;
    t->py = y;
    swap(t->mat[x][y], t->mat[nx][ny]);
    t->x = nx;
    t->y = ny;
    t->depth = depth;
    t->left = NULL;
    t->right = NULL;
    t->top = NULL;
    t->bottom = NULL;
    return t;
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

bool is_valid_move(int movex,int movey)
{
    if(movex<0 || movex>=MAX || movey<0 || movey>=MAX)
    {
        return false;
    }
    return true;
}

void expand_left(shared_ptr<TreeNode>& root)
{
    if((is_valid_move(root->x-1,root->y)) && (root->x-1!=root->px))
    { 
        shared_ptr<TreeNode> t = newnode(root->mat,root->x,root->y,root->x-1,root->y,root->depth+1);
        root->left = t;
    }
return;
}

void expand_right(shared_ptr<TreeNode>& root)
{
    if((is_valid_move(root->x+1,root->y)) && (root->x+1!=root->px))
    { 
        shared_ptr<TreeNode> t = newnode(root->mat,root->x,root->y,root->x+1,root->y,root->depth+1);
        root->right = t;
    }
return;
}

void expand_top(shared_ptr<TreeNode>& root)
{
    if((is_valid_move(root->x,root->y+1)) && (root->y+1!=root->py))
    { 
        shared_ptr<TreeNode> t = newnode(root->mat,root->x,root->y,root->x,root->y+1,root->depth+1);
        root->top = t;
    }
return;
}

void expand_bottom(shared_ptr<TreeNode>& root)
{
    if((is_valid_move(root->x,root->y-1)) && (root->y-1!=root->py))
    { 
        shared_ptr<TreeNode> t = newnode(root->mat,root->x,root->y,root->x,root->y-1,root->depth+1);
        root->bottom = t;
    }
return;
}


void make_tree(shared_ptr<TreeNode>& root, int depth_limit)
{
    if(root == NULL || root->depth == depth_limit)
    {return;}
    expand_left(root);
    expand_right(root);
    expand_top(root);
    expand_bottom(root);
    make_tree(root->left,depth_limit);
    make_tree(root->right,depth_limit);
    make_tree(root->top,depth_limit);
    make_tree(root->bottom,depth_limit);
    return;
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
return;
}

void write_tree(shared_ptr<TreeNode>& root)
{
    if(root == NULL)
    {cout<<"NULL value"<<endl;return;}
    cout<<"\n--------------\n";
    print_mat(root->mat);
    cout<<"----------------\n";
    write_tree(root->left);
    write_tree(root->right);
    write_tree(root->top);
    write_tree(root->bottom);
    return;
}

void write_header(std::ofstream &dotfile)
{
    dotfile << "digraph tree{" <<endl;
    dotfile << "node [shape=circle];"<<endl;
return;
}

void add_nodes(std::ofstream &dotfile, const shared_ptr<TreeNode>& root)
{
    if(root == NULL)
    {
        // dotfile <<"null [label=\"Invalid Move\"]"<<endl;
        return;
    }
    dotfile << get_string(root->mat)+" [label="+" \""+get_label(root->mat)+"\"]" <<endl;
    add_nodes(dotfile,root->left);
    add_nodes(dotfile,root->right);
    add_nodes(dotfile,root->top);
    add_nodes(dotfile,root->bottom);
return;
}

void add_edges(std::ofstream &dotfile, const shared_ptr<TreeNode>& root)
{
    if(root == NULL)
    {return;}
    if(root->left!=NULL)
    {
        dotfile << get_string(root->mat)+" -> "+get_string(root->left->mat)<<endl;
    }
    if(root->right!=NULL)
    {
        dotfile << get_string(root->mat)+" -> "+get_string(root->right->mat)<<endl;
    }
    if(root->top!=NULL)
    {
        dotfile << get_string(root->mat)+" -> "+get_string(root->top->mat)<<endl;
    }
    if(root->bottom!=NULL)
    {
        dotfile << get_string(root->mat)+" -> "+get_string(root->bottom->mat)<<endl;
    }
    add_edges(dotfile,root->left);
    add_edges(dotfile,root->right);
    add_edges(dotfile,root->top);
    add_edges(dotfile,root->bottom);
return;
}

void write_footer(std::ofstream &dotfile)
{
    dotfile << "}" <<endl;
return;
}

void draw_tree(const shared_ptr<TreeNode>& root)
{
    std::ofstream dotfile("tree.dot");
    if (!dotfile) {
        std::cerr << "Error opening DOT file." <<endl;
        return ;
    }
    write_header(dotfile);
    shared_ptr<TreeNode> save = root;
    add_nodes(dotfile,root);
    add_edges(dotfile,save);
    write_footer(dotfile);
    dotfile.close();
    system("dot -Tsvg tree.dot -o game_tree.svg");
    cout<<"SVG file saved in current directory"<<endl;
return;
}

int main()
{
    int initial[MAX][MAX] ={{1,2,3},
                            {4,5,6},
                            {7,8,0}};

    shared_ptr<TreeNode> root = newnode(initial,2,2,2,2,0);
    make_tree(root,5);
    // write_tree(root);
    draw_tree(root);
    return 0;
}