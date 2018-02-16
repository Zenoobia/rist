#include "Test.cpp"

int main()
{
  sol::state lua;
  TestNS::init(lua);
  return 0;
}
