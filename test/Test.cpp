/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim $
   ======================================================================== */
#include "Test.hpp"

namespace TestNS {

void init(sol::state &lua)
{
  lua.new_simple_usertype<Test>("Test",
				sol::constructors<sol::types<int>>(),
				"id", &Test::id);
}

}
