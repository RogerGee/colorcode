#include <iostream>
#include "colorcode.h"
using namespace std;
using namespace colorcode;

const auto BLUE = make_tuple(COLOR_BLUE,COLOR_DEFAULT,bold_off);
const auto REDBOLD = make_tuple(COLOR_RED,COLOR_DEFAULT,bold_on);
const auto BLUEBOLD = make_tuple(COLOR_BLUE,COLOR_DEFAULT,bold_on);
const auto YELLOWBOLD = make_tuple(COLOR_YELLOW,COLOR_DEFAULT,bold_on);
const auto REVERSE = make_tuple(COLOR_YELLOW,COLOR_BLUE,bold_on);

void do_stuff()
{
    string user;
    color_context cntx(cout,REDBOLD);
    cout << "Please enter your name: ";

    {
        color_context userCntx(cout,YELLOWBOLD);
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
