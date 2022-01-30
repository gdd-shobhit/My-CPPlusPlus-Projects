#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>
#include "OgStack.h"

using namespace std;

void Wrapper() {

    // actual tests
    cout << "------------- Actual Test -----------------" << endl;
    OgStack<int> testStack1 = OgStack<int>();
    OgStack<double> testStack2 = OgStack<double>();
    OgStack<float> testStack3 = OgStack<float>();
    
    cout << "Test 1 of Int -." << endl;
    testStack1.Push(1);
    testStack1.Push(2);
    testStack1.Push(3);
    testStack1.Pop();
    cout << "Popped: " <<testStack1.Pop() << endl;
    testStack1.Push(234214);
    testStack1.Print();
    cout << endl;
    cout << "Test 2 of Double -." << endl;
    testStack2.Push(1.01);
    testStack2.Push(2.01);
    testStack2.Push(3.01);
    testStack2.Pop();
    cout << "Popped: " << testStack2.Pop() << endl;
    testStack2.Push(5.21);
    testStack2.Print();
    cout << endl;
    cout << "Test 3 of Floats -." << endl;
    testStack3.Push(1.23f);
    testStack3.Push(2.23f);
    testStack3.Push(3.2134f);
    testStack3.Pop();
    cout << "Popped: " << testStack3.Pop() << endl;
    testStack3.Push(23.3214f);
    testStack3.Print();
    testStack3.Pop();
    testStack3.Pop();
    // returning null when there is nothing in stack
    cout << testStack3.Pop() << endl;
    cout << endl;
    cout << "------ copyConstrucotr and AssignmentOperator ------" << endl;
    OgStack<int> copyConstruc = OgStack<int>(testStack1);
    copyConstruc.Print();
    cout << endl;
    copyConstruc.Pop();
    copyConstruc.Print();
    cout << endl;
    copyConstruc = testStack1;
    copyConstruc.Print();

}
int main()
{
    Wrapper();
    _CrtDumpMemoryLeaks();
}

