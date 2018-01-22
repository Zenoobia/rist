#include <iostream>
#include <assert.h>

#include "two_things.hpp"

void printMessage(const std::string &s) {
    std::cout << "Message from lua: "<< s << std::endl;
}

void printMessage_i(const std::string &s, const int &v) {
    std::cout << s << v << std::endl;
}

void printMessage_b(const std::string &s, const bool v) {
    std::cout << s << v << std::endl;
}

int main() {
    std::cout << "start" << std::endl;
    sol::state lua;
    lua["printMessage"] = sol::overload(&printMessage_b, &printMessage_i);

    // Create a pass-through style of function
    lua.script("function f ( a, b ) printMessage(\"a = \", a); printMessage(\"b = \", b); return a, b end");

    // get the function out of Lua
    sol::function f = lua["f"];

    //two_things things = f(two_things{24, true});
#if 1
    int v1, v2 = f(24, true);
    std::cout << v1 << std::endl;

    assert(v1 == 24);
    assert(v2 == true);
#endif

    two_things things = two_things({24, true});
    std::cout << "things.a = " << things.a << std::endl;
    assert(things.a == 24);
    assert(things.b == true);
    
    return 0;
}
