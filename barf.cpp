#include "colorcode.h"
#include <iostream>
#include <signal.h>
using namespace std;
using namespace colorcode;

void intr(int)
{
    color_context ctx(cout,make_tuple(COLOR_BLUE,COLOR_DEFAULT,bold_on));
    cout << "Thanks for watching!\n";
    exit(0);
}

int main()
{
    color_context ctx(cout,make_tuple(COLOR_CURRENT,COLOR_CURRENT,bold_on));
    static const
        char* line = "!!@@##$$%%$$##@@!!@@##$$%%$$##@@!!@@##$$%%!!@@##$$%%$$##@@!!@@##$$%%$$##@@!!@@##$$%%";

    signal(SIGINT,intr);

    while (true)
        for (int c = 1;c < 256;++c)
            cout << fore(c) << line << '\n';
}
