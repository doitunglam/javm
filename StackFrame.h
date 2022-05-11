#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

/*
StackFrame declaration
*/
class StackFrame
{
public:
    int opStackMaxSize;           // max size of operand stack
    int currentLocalVarSpaceSize; // size of local variable array
    float *opStack;
    int opStackIndex;
    int lineCount;
    int localVarSpaceSize;
    struct node
    {
        float value;
        float type;
        std::string key;
        node *left;
        node *right;
        int height;
        // Construction With Data
        node(std::string key, float value, float type);
    };
    node *treeRoot;

public:
    /*
    Constructor of StackFrame
    */
    StackFrame();

    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    node *rightRotate(node *y);
    node *leftRotate(node *x);
    int getBalance(node *N);
    int getHeight(node *N);
    bool cmp(std::string s1, std::string s2);
    node *insert(node *AVLTreeNode, std::string key, float value, float type);
    node *load(node *AVLTreeNode, std::string key, float *value, float *type);
    node *par(node *treeNode, std::string key);
    void elementBreakdowm(std::string inputLine, std::string *command, std::string *argument);
    void opStackPop(float *value, float *type);
    void opStackPush(float value, float type);
    node *localVarSpaceLoad(std::string key, float *value, float *type);
    node *localVarSpaceStore(std::string key, float value, float type);
    int commandSpecification(std::string *command);
    void commandExecution(std::string command, std::string argument);
    bool valueType(std::string tar);
    void run(std::string filename);
};

#endif // !__STACK_FRAME_H__