#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

/*
StackFrame declaration
*/
class StackFrame
{
    int opStackMaxSize;  // max size of operand stack
    int localVarArrSize; // size of local variable array
    float opStack[1000];
    float localVarArr[1000];
    int opStackIndex;
    int localVarArrIndex;
    int lineCount;
 public :
        /*
        Constructor of StackFrame
        */
        StackFrame();

    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void elementBreakdowm(string inputLine, string *command, string *argument);
    void opStackPop(float *value, float *type);
    void opStackPush(float value, float type);
    void localVarArrLoad(int index,float *value, float *type);
    void localVarArrStore(int index,float value, float type);
    int commandSpecification(string *command);
    void commandExecution(string command, string argument);
    void run(std::string filename);
};

#endif // !__STACK_FRAME_H__