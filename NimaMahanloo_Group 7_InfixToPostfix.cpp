// Nima Mahanloo
// Infix to Postfix Converter
// CISP430
#include <iostream>
#include <fstream>
using namespace std;
// A structure as a node of stack
struct node
{
    char content = '\0';
    node* next = nullptr;
    node* prev = nullptr;
};
// A module to add a node to top of stack
void addNode (node* (&top))
{
    top->next = new node;
    top->next->prev = top;
    top = top->next;
}
// A module to push read operator on top of stack as necessary
void pushStack (node* (&top), int (&stackPointer), char pushData)
{
    if (top->content != '\0')
    {
        addNode(top);
        stackPointer++;
    }
    top->content = pushData;
}
// A module to pop top of stack as necessary
void popStack (node* (&top), int (&stackPointer))
{
    if (stackPointer > 0)
    {
        node* temp = top;
        top = top->prev;
        top->next = nullptr;
        delete temp;
        temp = nullptr;
        stackPointer--;
    }
    else if (stackPointer == 0)
    {
        top->content = '\0';
    }
}
// A module to convert infix character to postfix
void infixToPosfix (node* (&top), int (&stackPointer), char readData, char* (&outData))
{
    int i;
    for (i = 0; outData[i] != '\0'; i++){}
    int outSize = i;
    char topData = top->content;
    if (readData == '^')
    {
        pushStack(top, stackPointer, readData);
    }
    else if ((readData == '*') || (readData == '/'))
    {
        if ((topData == '^') || (topData == '*') || (topData == '/'))
        {
            outData[outSize] = topData;
            outSize++;
            popStack (top, stackPointer);
            pushStack(top, stackPointer, readData);
        }
        else
        {
            pushStack(top, stackPointer, readData);
        }
    }
    else if ((readData == '+') || (readData == '-'))
    {
        if ((topData == '^') || (topData == '*') || (topData == '/') || (topData == '+') || (topData == '-'))
        {
            outData[outSize] = topData;
            outSize++;
            popStack (top, stackPointer);
            pushStack(top, stackPointer, readData);
        }
        else
        {
            pushStack(top, stackPointer, readData);
        }
    }
    else if (readData == '(')
    {
        pushStack(top, stackPointer, readData);
    }
    else if (readData == ')')
    {
        while (!(topData == '('))
        {
            outData[outSize] = topData;
            popStack (top, stackPointer);
            topData = top->content;
            outSize++;
        }
        if (topData == '(')
        {
            popStack (top, stackPointer);
        }
    }
    else
    {
        outData[outSize] = readData;
        outSize++;
    }
    outData[outSize] = '\0';
}
// A module to pop and clear stack completely as necessary
void stackFree (node* (&top), int (&stackPointer), char* (&outData))
{
    bool sFlag = false;
    int i;
    for (i = 0; outData[i] != '\0'; i++){}
    int outSize = i;
    while ((stackPointer >= 0)&&(sFlag == false))
    {
        if (stackPointer == 0)
        {
            sFlag = true;
        }
        outData[outSize] = top->content;
        popStack(top, stackPointer);
        outSize++;
    }
    outData[outSize] = '\0';
}
// A module to read infix expression from input file and send it to postfix converter
void readFile (node* (&top), int (&stackPointer), char* (&outData))
{
    char data;
    ifstream read("input.txt");
    if (read.is_open())
    {
        cout << "Infix is : ";
        while (read.eof() == false)
        {
            read >> data;
            if (data != '\0')
            {
                infixToPosfix (top, stackPointer, data, outData);
                cout << data;
            }
            data = '\0';
        }
        cout << endl;
    }
    else
    {
        cout << "Error: Can not open file!" << endl;
    }
    read.close();
}

int main()
{
    // An integer variable to hold stack pointer position
    int stackPointer = -1;
    // An object from structure 'node' as stack
    node* stackSim = new node;
    // An object from node as stack's top
    node* stackTop = stackSim;
    // Increase stack pointer by one
    stackPointer++;
    // A character pointer as dynamic array for postfix calculation expression
    char* outData = (char*)malloc(sizeof(char));
    // character array pointer initial by null character
    outData[0] = '\0';
    // Call a module to read infix expression from the input file then send it to postfix converter
    readFile(stackTop, stackPointer, outData);
    // Call a module to pop remain operators in stack to finish postfix expression calculation
    stackFree(stackTop, stackPointer, outData);
    // Display calculated postfix expression
    cout << "Postfix is : " << outData << endl;
    // Clear heap and memory
    delete stackTop;
    stackTop = nullptr;
    delete stackSim;
    stackSim = nullptr;
    // Reset stack pointer
    stackPointer--;
    return 0;
}
