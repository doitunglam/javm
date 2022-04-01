#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE)
{
}

void StackFrame::elementBreakdowm(string inputLine, string *command, string *argument)
{
   string element[2] = {};
    int flag = 0;
    for (int i = 0; i < inputLine.length(); i++)
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
    // if(opStackIndex<0) throw error
    *type = opStack[opStackIndex];
    opStackIndex--;
    // no need to add another error indicator
    *value = opStack[opStackIndex];
}
void StackFrame::opStackPush(float value, float type)
{
    // if opStackIndex larger than maximum, throw error
    opStack[opStackIndex] = value;
    opStackIndex++;
    opStack[opStackIndex] = type;
    opStackIndex++;
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
        if (type == 0 && (int)type1 == 1)
            throw std::invalid_argument("Type Error");
        float type2, value2;
        opStackPop(&value2, &type2);
        if (type == 0 && (int)type2 == 1)
            throw std::invalid_argument("Type Error");
        int ans;
        if (command == "add")
            ans = value1 + value2;
        if (command == "sub")
            ans = value2 - value1;
        if (command == "mul")
            ans = value1 * value2;
        if (command == "div")
        {
            if (value1 == 0)
                throw std::invalid_argument("DivideByZero");
            if (type == 0)
                ans = (int)value2 / (int)value1;
            if (type == 1)
                ans = value2 / value1;
        }
        if (command == "rem")
        {
            if (value1 == 0)
                throw std::invalid_argument("DivideByZero");
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
            ans = (int)value2 == (int)value1;
            returnType = 0;
        }
        if (command == "neq")
        {
            ans = (int)value2 != (int)value1;
            returnType = 0;
        }
        if (command == "lt")
        {
            ans = (int)value2 < (int)value1;
            returnType = 0;
        }
        if (command == "gt")
        {
            ans = (int)value2 > (int)value1;
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
            throw std::invalid_argument("Type Error");
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
            throw std::invalid_argument("Type Error");
        if (command == "2f")
            opStackPush(value1, 1);
        if (command == "2i")
            opStackPush(value1, 0);
    }
    // nhom lenh nap va luu
    if (command == "const" || command == "load" || command == "store")
    {
        if (command == "const")
            opStackPush(atof(charArr), type);
        if (command == "load")
        {
            int index = atoi(charArr);
            if (index < 0 || index > 256)
                throw std::invalid_argument("Tru cap khong ton tai");
            if (localVarArr[index] != type)
                throw std::invalid_argument("Type Error");
            opStackPush(localVarArr[index + 1], type);
        }
        if (command == "store")
        {
            float value1, type1;
            opStackPop(&value1, &type1);
            int index = atoi(charArr);
            if (index < 0 || index > 256)
                throw std::invalid_argument("Tru cap khong ton tai");
            if ((int)type1 != type)
                throw std::invalid_argument("Type Error");
            localVarArr[index] = type;
            localVarArr[index + 1] = value1;
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
        opStackPush(value1,type1);
    }
    if(command =="val")
    {
        cout<<localVarArr[atoi(charArr)+1]<<'\n';
    }
}
void StackFrame::run(string filename)
{
    ifstream file(filename);
    string inputLine;
    while (getline(file, inputLine))
    {
        string command = "",
               argument = "";
        elementBreakdowm(inputLine, &command, &argument);
        commandExecution(command, argument);
    }
}