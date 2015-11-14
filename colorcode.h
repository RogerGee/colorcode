// colorcode.h
#ifndef COLORCODE_H
#define COLORCODE_H
#include <ostream>
#include <tuple>
#include <stack>

namespace colorcode
{
    // enumerate color modes: these only apply to some implementations
    // that support multiple color modes
    enum color_mode
    {
        color_mode_ANSI,
        color_mode_256
    };
    void set_color_mode(color_mode mode);

    // enumerate colors: these do not represent the full extent of the
    // colors available; it depends on the terminal
    enum color
    {
        COLOR_CURRENT = -2,
        COLOR_DEFAULT = -1,
        COLOR_BLACK,
        COLOR_DARK_RED,
        COLOR_DARK_GREEN,
        COLOR_DARK_YELLOW,
        COLOR_DARK_BLUE,
        COLOR_DARK_MAGENTA,
        COLOR_DARK_CYAN,
        COLOR_GRAY,
        COLOR_DARK_GRAY,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW,
        COLOR_BLUE,
        COLOR_MAGENTA,
        COLOR_CYAN,
        COLOR_WHITE
    };

    // enumerate ANSI colors
    enum ansi_color
    {
        ANSI_CURRENT = -2,
        ANSI_DEFAULT = -1,
        ANSI_BLACK,
        ANSI_RED,
        ANSI_GREEN,
        ANSI_YELLOW,
        ANSI_BLUE,
        ANSI_MAGENTA,
        ANSI_CYAN,
        ANSI_WHITE
    };

    // enumerate bold mode
    enum bold_mode
    {
        bold_on,
        bold_off,
        bold_current
    };

    // define 'color_spec'; tuple of foreground color, background
    // color and bold flag
    typedef std::tuple<int,int,bold_mode> color_spec;

    // color_context: defines a new color context that exists for the
    // object's lifetime; it is a good idea to only have one of these
    // at a time since they write to the same terminal
    class color_context
    {
    public:
        color_context(std::ostream& stream,const color_spec& spec);
        ~color_context();
    private:
        std::ostream& os;
        static std::stack<color_spec> stk;
    };

    // _color_manip: used by the implementation to update color using
    // stream manipulator-like objects
    class _color_manip
    {
        friend std::ostream& operator<<(std::ostream&,const _color_manip&);
    protected:
        _color_manip(const color_spec& clrSpec);
    private:
        color_spec spec;
    };

    class _foreground : public _color_manip
    {
    public:
        _foreground(int clr,bold_mode bold = bold_current);
    };
    class _background : public _color_manip
    {
    public:
        _background(int clr);
    };
    class _bothground : public _color_manip
    {
    public:
        _bothground(int fore,int back,bold_mode bold = bold_current);
    };

    // foreground, background and bothground: these are stream
    // manipulators that really just place terminal codes into the
    // stream
    template<typename... Targs>
    _background back(Targs... args)
    { return _background(args...); }

    template<typename... Targs>
    _foreground fore(Targs... args)
    { return _foreground(args...); }

    template<typename... Targs>
    _bothground both(Targs... args)
    { return _bothground(args...); }

    std::ostream& operator<<(std::ostream&,const _color_manip&);

    // initializer for library
    static class __colorcode_init__
    {
    public:
        __colorcode_init__();
        ~__colorcode_init__();
    private:
        static int ref;
    } __colorcode_initializer__;
}

#endif
