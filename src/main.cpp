/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim $
   ======================================================================== */

#include <iostream>
#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/Resource.h>
using namespace Corrade;

#include <json.hpp>

#include <sol.hpp>

typedef sol::protected_function Function;

#include "example/JsonCast.h"
#include "example/Person.h"

using json = nlohmann::json;

void printMessage(const std::string& s);
void printMessage(const std::string& s) {
    std::cout << s << std::endl;
}

#include <entityplus/entity.h>
using namespace entityplus;

using CompList = component_list<Person>;
using TagList = tag_list<struct TA, struct TB>;

//entity_manager<CompList, TagList> entityManager;

using EntityManager = entity_manager<CompList, TagList>;
using entity_t = typename EntityManager::entity_t;

EntityManager entityManager;
sol::state lua;
json j;

int main() {
    
    lua["printMessage"] = printMessage;

    Utility::Resource rs{"shadow-data"};

    sol::table game = lua.create_named_table("game");
    game["printMessage"] = printMessage;

    lua.script(rs.get("script.lua"));

    auto function_obj = lua["sumNumbers"];
    Function function = lua["sumNumbers"];
    std::cout << static_cast<double>(function(3,7)) << std::endl;

    auto entity = entityManager.create_entity();
#if 0
    Person person(20, "John");
    bool valid;
    std::tie(person, valid) = entity.add_component<Person>(15, "John");

    person.name = "John";
    person.age  = 25;
    //auto person = Person(30, "John");
    static_assert(metas::isRegistered<Person>(), "Person class is not registered!");
	static_assert(metas::getMemberCount<Person>() == 4, "Person does not have 4 members registered?");
/*
    metas::doForAllMembers<Person>([&obj, &object](auto& member)
                                   std::cout << member << std::endl;
                                   );
*/
    j = person;std::cout << std::setw(4) << j << std::endl;
#endif
    Person *person;
    bool valid;
    /*
    std::tie(person, valid) = entity.add_component<Person>(15, "John");

    person->name = "John";
    person->age = 19;

    assert(person.age == entity.get_component<Person>().age);
    */

    return 0;
}
