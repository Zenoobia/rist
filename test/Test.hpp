
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim Planstedt $
   ======================================================================== */

#pragma once

#include <sol.hpp>

namespace TestNS {

inline
 void init(sol::state &lua);

struct Test {
  Test(int id_) : id(id_) {};
  int id;
};
}
