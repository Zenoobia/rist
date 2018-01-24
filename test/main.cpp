#include <iostream>
#include <assert.h>

#include <sol.hpp>


void printMessage(const std::string &s) {
    std::cout << "Message from lua: "<< s << std::endl;
}

void printMessage_i(const std::string &s, const int &v) {
    std::cout << s << v << std::endl;
}

void printMessage_b(const std::string &s, const bool v) {
    std::cout << s << v << std::endl;
}

#include "../src/example/JsonCast.h"
#include "../src/TestComponent.hpp"

struct Test {
  Test() : id(0), name("") {};
  Test(const int id, const std::string name) : id(id), name(name) {};

  int id;
  std::string name;
};

#include <json.hpp>
using json = nlohmann::json;

#include <entityplus/entity.h>
using namespace entityplus;

using CompList = component_list<int, TestComponent, Test>;
using TagList = tag_list<struct TA, struct TB>;

using entity_manager_t = entity_manager<CompList, TagList>; 

using Entity = typename entity_manager_t::entity_t;

class Manager : public entity_manager_t {
public:
  int getValue() {
    return _value;
  }


  void createEntity_v() noexcept {
    std::cout << "Created entity!" << std::endl;
    create_entity();
  }

  Entity createEntity() noexcept {
    std::cout << "Created entity!" << std::endl;
    return create_entity();
  }

private:
  int _value;
};

namespace sol {
template <>
struct is_to_stringable<Manager> : std::false_type {};
template <>
struct is_to_stringable<entity_manager_t> : std::false_type {};
template <>
struct is_to_stringable<Entity> : std::false_type {};
  /*
template <>
        struct unique_usertype_traits {
  
        }
  */
}

#include <memory>

int main() {
    std::cout << "start" << std::endl;
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua["printMessage"] = sol::overload(&printMessage_b, &printMessage_i);

    lua.new_usertype<Entity> ("Entity",
			      "new", sol::no_constructor);
			      //"addTest", sol::overload(&Entity::add_component);

    lua.new_usertype<Manager> ("Manager",
			       //		       sol::bases<entity_manager_t>(),
			       "new", sol::no_constructor,
			       "create_entity", sol::overload(&Manager::create_entity<>),
			       "createEntity", &Manager::createEntity);

    lua.new_usertype<Test> ("Test",
			    "id", &Test::id,
			    "name", &Test::name);

    lua.new_usertype<TestComponent> ("TestComponent",
				     "value", sol::property(&TestComponent::getValue, &TestComponent::setValue),
				     "name", &TestComponent::name);

    //lua.new_usertype<Manager::component_t> ("GenComp");


    
    auto tc = TestComponent(10, "TC");
    json j = tc;
    std::cout << j.dump(4) << std::endl;
      
    lua.set_function("addComponents", [](Entity &e, sol::variadic_args va) {
	for (auto v : va) {
	  e.add_component(static_cast<int>(v));
	  //std::cout << static_cast<int>(v) << std::endl;
	}
      });

    auto test = Test{1, "J"};

    Manager manager;
    Manager *shared_manager = &manager;

    lua["manager"] = shared_manager;
    lua["test"] = &test;

    lua.script("local test2 = Test.new();local ent = manager:createEntity();\
addComponents(ent,500); local ent2 = manager:create_entity();");

    auto sum = 0;
    manager.for_each<int>(
			  [&](Entity ent, int i) {
			    sum++;
			  });
   
    std::cout << sum << "\n";


#if 0
    lua.script("function f ( a, b ) printMessage(\"a = \", a); printMessage(\"b = \", b); return a, b end");

    // get the function out of Lua
    sol::function f = lua["f"];

    //two_things things = f(two_things{24, true});
    int v1, v2 = f(24, true);
    std::cout << v1 << std::endl;

    assert(v1 == 24);
    assert(v2 == true);

#include "two_things.hpp"
    two_things things = two_things({24, true});
    std::cout << "things.a = " << things.a << std::endl;
    assert(things.a == 24);
    assert(things.b == true);
    
#endif

    return 0;
}
