/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim $
   ======================================================================== */
#include <sol.hpp>
#include <iostream>

struct Test {
  Test(int id_) : id(id_) {};
  int id;
};



int main()
{
   sol::state lua;
  lua.new_simple_usertype<Test>("Test",
  sol::constructors<sol::types<int>>(),
  "id", &Test::id);
  return 0;
}
