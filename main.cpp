#include <iostream>
#include "StackFrame.h"
#include "errors.h"
using namespace std;

/*
Run the testcase written in `filename`
@param filename name of the file
*/
void test(string filename)
{
    StackFrame *sf = new StackFrame();
    try
    {
        sf->run(filename);
    }
    catch (exception &e)
    {
        cout << e.what();
    }
    delete sf;
}
void preOrder(StackFrame::treeNode *root)
{
    if(root != NULL)
    {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}
/*
Main function
*/
int main(int argc, char **argv)
{
    StackFrame *sf = new StackFrame();
    sf->treeRoot = sf->insert(sf->treeRoot, "1", 1, 1);
    sf->treeRoot = sf->insert(sf->treeRoot, "3", 1, 1);
    sf->treeRoot = sf->insert(sf->treeRoot, "5", 1, 1);
    sf->treeRoot = sf->insert(sf->treeRoot, "4", 1, 1);
    sf->treeRoot = sf->insert(sf->treeRoot, "6", 1, 1);
    sf->treeRoot = sf->insert(sf->treeRoot, "2", 1, 1);
    cout<<("1"<"0");
    //preOrder(sf->treeRoot);
    return 0;
}