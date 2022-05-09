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
    struct treeNode
    {
        float value;
        float type;
        std::string key;
        treeNode *left;
        treeNode *right;
        int height;
        // Construction With Data
        treeNode(std::string key, float value, float type);
    };
    treeNode *treeRoot;

public:
    /*
    Constructor of StackFrame
    */
    StackFrame();

    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    treeNode *rightRotate(treeNode *y);
    treeNode *leftRotate(treeNode *x);
    int getBalance(treeNode *N);
    int getHeight(treeNode *N);
    bool cmp(std::string s1,std::string s2);
    treeNode *insert(treeNode *AVLTreeNode, std::string key, float value, float type);
    treeNode *load(treeNode *AVLTreeNode, std::string key, float *value, float *type);
    treeNode *par(treeNode *treeNode, std::string key);
    void elementBreakdowm(std::string inputLine, std::string *command, std::string *argument);
    void opStackPop(float *value, float *type);
    void opStackPush(float value, float type);
    treeNode* localVarSpaceLoad(std::string key, float *value, float *type);
    treeNode* localVarSpaceStore(std::string key, float value, float type);
    int commandSpecification(std::string *command);
    void commandExecution(std::string command, std::string argument);
    bool valueType(std::string tar);
    void run(std::string filename);
};

#endif // !__STACK_FRAME_H__