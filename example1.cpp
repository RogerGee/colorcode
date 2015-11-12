#include <iostream>
#include "colorcode.h"
using namespace std;
using namespace colorcode;

const auto BLUE = make_tuple(FORE_BLUE,BACK_DEFAULT,false);
const auto REDBOLD = make_tuple(FORE_RED,BACK_DEFAULT,true);
const auto BLUEBOLD = make_tuple(FORE_BLUE,BACK_DEFAULT,true);
const auto REVERSE = make_tuple(FORE_YELLOW,BACK_BLUE,true);

void do_stuff()
{
    string user;
    color_context cntx(cout,REDBOLD);
    cout << "Please enter your name: ";

    {
        color_context userCntx(cout,BLUEBOLD);
        cin >> user;
    }

    cout << "Thank you, ";
    {
        color_context other(cout,REVERSE);
        cout << user;
    }
    cout << ", for using this program.\n";
}

int main()
{
    color_context cntx(cout,BLUE);

    cout << "Hello, World!\n";
    do_stuff();

    cout << "Good bye!\n";
}
