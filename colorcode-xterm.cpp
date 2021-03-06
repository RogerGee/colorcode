// colorcode-xterm.cpp
#include "colorcode.h"
#include <sstream>
#include <unistd.h>
using namespace std;
using namespace colorcode;

// return valid descriptor only if it is a terminal
static inline int get_term_fd()
{
    static int fd = -2;
    if (fd == -2)
        fd = isatty(STDOUT_FILENO) ? STDOUT_FILENO : -1;
    return fd;
}

static color_mode colorMode = color_mode_256; // default for xterm
static const color_spec* currentSpec = NULL;
void colorcode::set_color_mode(color_mode mode)
{
    colorMode = mode;
}

// generate color codes
static string generate(const color_spec& spec)
{
    string buf;
    stringstream ss(ios_base::out | ios_base::ate);

    // handle bold command
    if (get<2>(spec) == bold_on)
        buf += "\033[1m";
    else if (get<2>(spec) == bold_off)
        buf += "\033[22m";

    int f = get<0>(spec), b = get<1>(spec);
    if (f == COLOR_CURRENT && currentSpec != NULL)
        f = get<0>(*currentSpec);
    if (b == COLOR_CURRENT && currentSpec != NULL)
        b = get<1>(*currentSpec);

    // handle foreground command
    if (f != COLOR_CURRENT) {
        if (f == COLOR_DEFAULT)
            buf += "\033[39m"; // reset foreground
        else {
            if (colorMode == color_mode_ANSI) {
                ss.str("\033[");
                ss << 30+f << 'm';
            }
            else if (colorMode == color_mode_256) {
                ss.str("\033[38;5;");
                ss << f << 'm';
            }
            buf += ss.str();
        }
    }

    // handle background command
    if (b != COLOR_CURRENT) {
        if (b == COLOR_DEFAULT)
            buf += "\033[49m"; // reset foreground
        else {
            if (colorMode == color_mode_ANSI) {
                ss.str("\033[");
                ss << 40+b << 'm';
            }
            else if (colorMode == color_mode_256) {
                ss.str("\033[48;5;");
                ss << b << 'm';
            }
            buf += ss.str();
        }
    }

    return buf;
}

// apply string directly to terminal
static void apply(int fd,const color_spec& spec)
{
    // send command to terminal
    string buf(generate(spec));
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
        currentSpec = &stk.top();
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
            // special case: nothing to reset; the static initializers
            // will clean up our changes later; there could still be
            // changes outstanding caused by the manipulators

            // no color context specified by the library
            currentSpec = NULL;
        }
        else {
            // restore previous 
            color_spec& last = stk.top();
            apply(fd,last);
            currentSpec = &last;
        }
    }
}

// color manipulator objects
_color_manip::_color_manip(const color_spec& clrSpec)
    : spec(clrSpec)
{
}
_foreground::_foreground(int clr,bold_mode bold)
    : _color_manip(make_tuple(clr,COLOR_CURRENT,bold))
{
}
_background::_background(int clr)
    : _color_manip(make_tuple(COLOR_CURRENT,clr,bold_current))
{
}
_bothground::_bothground(int fore,int back,bold_mode bold)
    : _color_manip(make_tuple(fore,back,bold))
{
}

// color manipulator
ostream& colorcode::operator<<(ostream& os,const _color_manip& m)
{
    // make sure the terminal is the standard output; however we sent
    // the terminal codes to the stream so that
    int fd = get_term_fd();
    if (fd != -1)
        os << generate(m.spec);

    return os;
}

// initialization - we must preserve the original terminal colors
/*static*/ int __colorcode_init__::ref = 0;
__colorcode_init__::__colorcode_init__()
{
    ++ref;
}
__colorcode_init__::~__colorcode_init__()
{
    if (--ref <= 0) {
        int fd = get_term_fd();
        if (fd != -1) {
            write(fd,"\033[0m",4);
        }
    }
}
