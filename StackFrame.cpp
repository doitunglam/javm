#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarSpaceSize(LOCAL_VARIABLE_SPACE_SIZE)
{
    opStackIndex = 0;
    lineCount = 0;
    opStack = new float[opStackMaxSize];
    currentLocalVarSpaceSize = 0;
    treeRoot = NULL;
}
StackFrame::node::node(string key, float value, float type)
{
    this->height = 1;
    this->key = key;
    this->left = NULL;
    this->right = NULL;
    this->type = type;
    this->value = value;
}
StackFrame::node *StackFrame::rightRotate(node *y)
{
    node *x = y->left;
    node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right)) + 1;
    x->height = (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right)) + 1;

    // Return new root
    return x;
}
StackFrame::node *StackFrame::leftRotate(node *x)
{
    node *y = x->right;
    node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    y->height = (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right)) + 1;
    x->height = (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right)) + 1;

    // Return new root
    return y;
}
int StackFrame::getHeight(node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int StackFrame::getBalance(node *N)
{
    if (N == NULL)
        return 0;
    return getHeight(N->left) - getHeight(N->right);
}
bool StackFrame::cmp(string s1, string s2)
{
    int n = s1.length() > s2.length() ? s1.length() : s2.length();
    for (int i = 0; i < n; i++)
    {
        if (s1[i] - s2[i] > 0)
            return 1;
        if (s1[i] - s2[i] < 0)
            return 0;
    }
    if (s1.length() > s2.length())
        return 1;
    else
        return 0;
}
StackFrame::node *StackFrame::insert(node *node, string key, float value, float type)
{
    currentLocalVarSpaceSize++;
    if (currentLocalVarSpaceSize > localVarSpaceSize)
        throw LocalSpaceFull(lineCount);
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
        return (new StackFrame::node(key, value, type));
    if (cmp(key, node->key) == 0)
        node->left = insert(node->left, key, value, type);
    else if (cmp(key, node->key) == 1)
        node->right = insert(node->right, key, value, type);
    else // Equal keys are not allowed in BST
        return node;

    /* 2. Update height of this ancestor node */
    node->height = (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right)) + 1;

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) AVLTrieeNode pointer */
    return node;
}
bool StackFrame::valueType(string tar)
{
    std::size_t found = tar.find('.');
    if (found != std::string::npos)
        return 1;
    return 0;
}
void StackFrame::elementBreakdowm(string inputLine, string *command, string *argument)
{
    string element[2] = {};
    int flag = 0;
    for (int i = 0; i < (int)inputLine.length(); i++)
        if (inputLine[i] == ' ')
            flag++;
        else
            element[flag] = element[flag] + inputLine[i];
    *command = element[0];
    *argument = element[1];
}
void StackFrame::opStackPop(float *value, float *type)
{
    opStackIndex--;
    if (opStackIndex < 0)
        throw StackEmpty(lineCount);
    *type = opStack[opStackIndex];
    opStackIndex--;
    *value = opStack[opStackIndex];
}
void StackFrame::opStackPush(float value, float type)
{
    if (opStackIndex > opStackMaxSize - 1)
        throw StackFull(lineCount);
    opStack[opStackIndex] = value;
    opStackIndex++;
    opStack[opStackIndex] = type;
    opStackIndex++;
}
StackFrame::node *StackFrame::load(node *node, string key, float *value, float *type)
{
    if(node==NULL) return NULL;
    if (node->key == key)
    {
        (*value) = node->value;
        (*type) = node->type;
        return node;
    }
    if (cmp(node->key, key) == 0)
        return load(node->right, key, value, type);
    else
        return load(node->left, key, value, type);
}
StackFrame::node *StackFrame::par(node *node, string key)
{
    if(node==NULL) return NULL;
    if (node->left != NULL)
        if (node->left->key == key)
            return node;
    if (node->right != NULL)
        if (node->right->key == key)
            return node;
    if (node == NULL)
        return NULL;
    if (cmp(node->key, key) == 0)
        return par(node->left, key);
    else
        return par(node->right, key);
}
StackFrame::node *StackFrame::localVarSpaceStore(string key, float value, float type)
{
    this->treeRoot = insert(this->treeRoot, key, value, type);
    return treeRoot;
}
StackFrame::node *StackFrame::localVarSpaceLoad(string key, float *value, float *type)
{
    return load(this->treeRoot, key, value, type);
}
int StackFrame::commandSpecification(string *command)
{
    if ((*command)[0] == 'i')
    {
        command->erase(command->begin());
        return 0;
    }
    if ((*command)[0] == 'f')
    {
        command->erase(command->begin());
        return 1;
    }
    return 2;
}
void StackFrame::commandExecution(string command, string argument)
{
    char *charArr;
    charArr = &argument[0];
    int type = commandSpecification(&command);
    int returnType = type;
    // khoi cac toan tu 2 ngoi
    if (command == "add" || command == "sub" || command == "mul" || command == "div" || command == "rem" || command == "and" || command == "or" || command == "eq" || command == "neq" || command == "lt" || command == "gt")
    {
        float type1, value1;
        opStackPop(&value1, &type1);
        float type2, value2;
        opStackPop(&value2, &type2);
        if (type == 0 && (int)type1 == 1)
            throw TypeMisMatch(lineCount);
        if (type == 0 && (int)type2 == 1)
            throw TypeMisMatch(lineCount);
        float ans;
        if (command == "add")
            ans = value1 + value2;
        if (command == "sub")
            ans = value2 - value1;
        if (command == "mul")
            ans = value1 * value2;
        if (command == "div")
        {
            if (value1 == 0)
                throw DivideByZero(lineCount);
            if (type == 0)
                ans = (int)value2 / (int)value1;
            if (type == 1)
                ans = value2 / value1;
        }
        if (command == "rem")
        {
            if (value1 == 0)
                throw DivideByZero(lineCount);
            ans = (int)value2 % (int)value1;
        }
        if (command == "and")
        {
            ans = (int)value2 & (int)value1;
            returnType = 0;
        }
        if (command == "or")
        {
            ans = (int)value2 | (int)value1;
            returnType = 0;
        }
        if (command == "eq")
        {
            ans = value2 == value1;
            returnType = 0;
        }
        if (command == "neq")
        {
            ans = value2 != value1;
            returnType = 0;
        }
        if (command == "lt")
        {
            ans = value2 < value1;
            returnType = 0;
        }
        if (command == "gt")
        {
            ans = value2 > value1;
            returnType = 0;
        }
        opStackPush(ans, returnType);
    }
    // nhom cac toan tu 1 ngoi
    if (command == "neg" || command == "bnot")
    {
        float type1, value1;
        opStackPop(&value1, &type1);
        if (type == 0 && type1 == 1)
            throw TypeMisMatch(lineCount);
        float ans;
        if (command == "neg")
            ans = -value1;
        if (command == "bnot")
        {
            ans = value1 == 0 ? 1 : 0;
            returnType = 0;
        }
        opStackPush(ans, returnType);
    }
    // nhom chuyen doi kieu
    if (command == "2f" || command == "2i")
    {
        float type1, value1;
        opStackPop(&value1, &type1);
        if (type != type1)
            throw TypeMisMatch(lineCount);
        if (command == "2f")
            opStackPush(value1, 1);
        if (command == "2i")
        {
            int value1i = (int)value1;
            float value1f = (float)value1i;
            opStackPush(value1f, 0);
        }
    }
    // nhom lenh nap va luu
    if (command == "const" || command == "load" || command == "store")
    {
        if (command == "const")
        {
            if (valueType(argument) != (int)type)
                throw TypeMisMatch(lineCount);
            opStackPush(atof(charArr), type);
        }
        if (command == "load")
        {
            float value, type;
            node *tn = localVarSpaceLoad(argument, &value, &type);
            if (tn == NULL)
                throw UndefinedVariable(lineCount);
            opStackPush(value, type);
        }
        if (command == "store")
        {
            float value1, type1;
            opStackPop(&value1, &type1);
            if (type != (int)type1)
                throw TypeMisMatch(lineCount);
            localVarSpaceStore(argument, value1, type1);
        }
    }
    if (command == "top")
    {
        float value1, type1;
        opStackPop(&value1, &type1);
        if (type1 == 0)
            cout << (int)value1 << '\n';
        if (type1 == 1)
            cout << value1 << '\n';
        opStackPush(value1, type1);
    }
    if (command == "val")
    {
        float value, type;
        node *tn = localVarSpaceLoad(argument, &value, &type);
        if (tn == NULL)
            throw UndefinedVariable(lineCount);
        if (type == 0)
            cout << (int)value << '\n';
        if (type == 1)
            cout << value << '\n';
    }
    if (command == "par")
    {
        if(treeRoot==NULL) throw TypeMisMatch(lineCount);
        if (argument == treeRoot->key)
            cout << "null\n";
        else if (par(treeRoot, argument) == NULL)
            throw UndefinedVariable(lineCount);
            else
        cout << par(treeRoot, argument)->key;
    }
}
void StackFrame::run(string filename)
{
    ifstream file(filename);
    string inputLine;
    while (getline(file, inputLine))
    {
        lineCount++;
        string command = "",
               argument = "";
        elementBreakdowm(inputLine, &command, &argument);
        commandExecution(command, argument);
    }
}