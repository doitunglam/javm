#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE)
{
    opStackIndex = 0;
    lineCount = 0;
    localVarArrIndex = 0;
    for (int i = 0; i < localVarArrSize; i += 2)
        localVarArr[i] = -1;
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
void StackFrame::localVarArrLoad(int index, float *value, float *type)
{
    if (index < 0 || index >= localVarArrSize)
        throw ArrayOutOfRange(lineCount);
    if (localVarArr[index] == -1)
        throw UndefinedVariable(lineCount);
    (*value) = localVarArr[index + 1];
    (*type) = localVarArr[index];
}
void StackFrame::localVarArrStore(int index, float value, float type)
{
    if (index < 0 || index >= localVarArrSize)
        throw ArrayOutOfRange(lineCount);
    localVarArr[index] = type;
    localVarArr[index + 1] = value;
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
                int value1i=(int)value1;
                opStackPush(value1, 0);
            }
    }
    // nhom lenh nap va luu
    if (command == "const" || command == "load" || command == "store")
    {
        if (command == "const")
        {
            if (valueType(argument)!=(int)type)
                throw TypeMisMatch(lineCount);
            opStackPush(atof(charArr), type);
        }
        if (command == "load")
        {
            int index = atoi(charArr);
            float value, type;
            localVarArrLoad(index, &value, &type);
            opStackPush(value, type);
        }
        if (command == "store")
        {
            float value1, type1;
            opStackPop(&value1, &type1);
            if (type!=(int)type1)
                throw TypeMisMatch(lineCount);
            int index = atoi(charArr);
            localVarArrStore(index, value1, type1);
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
        int index = atoi(charArr);
        float value, type;
        localVarArrLoad(index, &value, &type);
        if (type == 0)
            cout << (int)value << '\n';
        if (type == 1)
            cout << value << '\n';
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