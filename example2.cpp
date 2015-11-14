#include <iostream>
#include "colorcode.h"
using namespace std;
using namespace colorcode;

int main()
{
    cout << fore(COLOR_BLUE,bold_on) << "Hello"
         << back(COLOR_YELLOW) << ','
         << back(COLOR_DEFAULT) << fore(COLOR_MAGENTA) << "World!\n"
         << fore(COLOR_RED) << "My name is Ludwig\n"
         << fore(111) << "This is the color 0x87afff.\n";
}
