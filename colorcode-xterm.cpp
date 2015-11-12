// colorcode-xterm.cpp
#include "colorcode.h"
#include <sstream>
#include <unistd.h>
using namespace std;
using namespace colorcode;

// return valid descriptor only if it is a terminal
inline int get_term_fd()
{
    static int fd = -2;
    if (fd == -2)
        fd = isatty(STDOUT_FILENO) ? STDOUT_FILENO : -1;
    return fd;
}

// get Xterm color code
constexpr int color_context::get_color_code(color clr,color_mode mode)
{
    return (clr == color_black)   ? ( mode == foreground ? 30 : 40 )
        :  (clr == color_red)     ? ( mode == foreground ? 31 : 41 )
        :  (clr == color_green)   ? ( mode == foreground ? 32 : 42 )
        :  (clr == color_yellow)  ? ( mode == foreground ? 33 : 43 )
        :  (clr == color_blue)    ? ( mode == foreground ? 34 : 45 )
        :  (clr == color_magenta) ? ( mode == foreground ? 35 : 45 )
        :  (clr == color_cyan)    ? ( mode == foreground ? 36 : 46 )
        :  (clr == color_white)   ? ( mode == foreground ? 37 : 47 )
        : 0;
}

const int colorcode::FORE_DEFAULT = color_context::get_color_code(color_default,foreground);
const int colorcode::FORE_BLACK = color_context::get_color_code(color_black,foreground);
const int colorcode::FORE_RED = color_context::get_color_code(color_red,foreground);
const int colorcode::FORE_GREEN = color_context::get_color_code(color_green,foreground);
const int colorcode::FORE_YELLOW = color_context::get_color_code(color_yellow,foreground);
const int colorcode::FORE_BLUE = color_context::get_color_code(color_blue,foreground);
const int colorcode::FORE_MAGENTA = color_context::get_color_code(color_magenta,foreground);
const int colorcode::FORE_CYAN = color_context::get_color_code(color_cyan,foreground);
const int colorcode::FORE_WHITE = color_context::get_color_code(color_white,foreground);
const int colorcode::BACK_DEFAULT = color_context::get_color_code(color_default,background);
const int colorcode::BACK_BLACK = color_context::get_color_code(color_black,background);
const int colorcode::BACK_RED = color_context::get_color_code(color_red,background);
const int colorcode::BACK_GREEN = color_context::get_color_code(color_green,background);
const int colorcode::BACK_YELLOW = color_context::get_color_code(color_yellow,background);
const int colorcode::BACK_BLUE = color_context::get_color_code(color_blue,background);
const int colorcode::BACK_MAGENTA = color_context::get_color_code(color_magenta,background);
const int colorcode::BACK_CYAN = color_context::get_color_code(color_cyan,background);
const int colorcode::BACK_WHITE = color_context::get_color_code(color_white,background);

static void apply(int fd,const color_spec& spec)
{
    string buf("\033[0m"); // restore to default first
    stringstream ss(ios_base::out | ios_base::ate);

    // handle bold command
    if (get<2>(spec))
        buf += "\033[1m";

    // handle foreground command
    if (get<0>(spec) != FORE_DEFAULT) {
        ss.str("\033[");
        ss << get<0>(spec) << 'm';
        buf += ss.str();
    }

    // handle background command
    if (get<1>(spec) != BACK_DEFAULT) {
        ss.str("\033[");
        ss << get<1>(spec) << 'm';
        buf += ss.str();
    }

    // send command to terminal
    write(fd,buf.data(),buf.length());
}

stack<color_spec> color_context::stk;
color_context::color_context(ostream& stream,const color_spec& spec)
    : os(stream)
{
    // change terminal color to the specified settings; take no action
    // if we aren't printing to a terminal
    int fd = get_term_fd();
    if (fd != -1) {
        // flush the stream so that any output is applied before we
        // load new changes to the terminal attributes
        os.flush();

        // push new color spec onto top of stack and then apply it
        stk.push(spec);
        apply(fd,spec);
    }
}
color_context::~color_context()
{
    // restore terminal context to last saved context OR the default
    // if no context was left on the stack; 
    int fd = get_term_fd();
    if (fd != -1) {
        // flush the stream so that any output is applied before we
        // unload the changes to the terminal attributes
        os.flush();

        // the top context is always ourself
        if (stk.size() > 0)
            stk.pop();

        if (stk.empty()) {
            static const auto def = make_tuple(FORE_DEFAULT,BACK_DEFAULT,false);
            apply(fd,def);
        }
        else {
            // restore previous 
            color_spec& last = stk.top();
            apply(fd,last);
        }
    }
}
