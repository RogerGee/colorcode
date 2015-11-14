#include "colorcode.h"
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;
using namespace colorcode;

static class init_program
{
public:
    init_program()
    {
        set_color_mode(color_mode_ANSI);
    }
} _init_;

int main()
{
    int n = 0;
    double s = 0;
    color_context ctx(cout,make_tuple(ANSI_BLACK,ANSI_WHITE,bold_off));

    cout << fixed << setprecision(3) << "This program computes averages.\n";
    while (true) {
        double i;
        cout << "Enter number: ";
        cin >> i;
        if (cin.eof()) {
            cout.put('\n');
            break;
        }
        if (cin.fail()) {
            cerr << fore(ANSI_RED) << "The input was incorrect. Try again.\n" << fore(ANSI_CURRENT) << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        s += i;
        ++n;
    }

    cout << "The average is " << s/n << ".\n";
}
