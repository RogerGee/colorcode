// colorcode.h
#ifndef COLORCODE_H
#define COLORCODE_H
#include <ostream>
#include <tuple>
#include <stack>

namespace colorcode
{
    // enumerate colors: not all of these colors are supported on
    // Win32 platforms
    enum color
    {
        color_default,
        color_black,
        color_red,
        color_green,
        color_yellow,
        color_blue,
        color_magenta,
        color_cyan,
        color_white
    };

    // enumerate color modes: foreground is the text color and
    // background is the screen color
    enum color_mode
    {
        foreground,
        background
    };

    // define constants for each color setting
    extern const int FORE_DEFAULT; extern const int BACK_DEFAULT;
    extern const int FORE_BLACK;   extern const int BACK_BLACK;
    extern const int FORE_RED;     extern const int BACK_RED;
    extern const int FORE_GREEN;   extern const int BACK_GREEN;
    extern const int FORE_YELLOW;  extern const int BACK_YELLOW;
    extern const int FORE_BLUE;    extern const int BACK_BLUE;
    extern const int FORE_MAGENTA; extern const int BACK_MAGENTA;
    extern const int FORE_CYAN;    extern const int BACK_CYAN;
    extern const int FORE_WHITE;   extern const int BACK_WHITE;

    // define 'color_spec'; tuple of foreground color, background
    // color and bold flag
    typedef std::tuple<int,int,bool> color_spec;

    // color_context: defines a new color context that exists for the
    // object's lifetime; it is a good idea to only have one of these
    // at a time since they write to the same terminal
    class color_context
    {
    public:
        color_context(std::ostream& stream,const color_spec& spec);
        ~color_context();

        static constexpr int get_color_code(color clr,color_mode mode);
    private:
        std::ostream& os;
        static std::stack<color_spec> stk;
    };
}

#endif
