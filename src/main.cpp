/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim $
   ======================================================================== */

#include <iostream>
#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/Resource.h>
#include <Corrade/Utility/Directory.h>
using namespace Corrade::Utility;

#include <json.hpp>

#pragma warning(push, 0) // Prevents c++17 Deprecation warnings
#include <sol.hpp>
#pragma warning(pop)
typedef sol::protected_function Function;

#include "example/JsonCast.h"
#include "example/Person.h"
#include "TestComponent.hpp"

using json = nlohmann::json;

void printMessage(const std::string& s);
void printMessage(const std::string& s) {
    std::cout << s << std::endl;
}

#include <entityplus/entity.h>
using namespace entityplus;

using CompList = component_list<Person, TestComponent>;
using TagList = tag_list<struct TA, struct TB>;

//entity_manager<CompList, TagList> entityManager;

using EntityManager = entity_manager<CompList, TagList>;
using entity_t = typename EntityManager::entity_t;

EntityManager entityManager;
sol::state lua;
json j;

bool addEnemy(const int value, const std::string name) {
    auto ent = entityManager.create_entity();
    auto[comp, val] = ent.add_component<TestComponent>(value, name);
    return val;
}

bool executeScript(const std::string &scriptPath ){

    if(Directory::fileExists(scriptPath)) {
        lua.script_file(scriptPath);
        return true;
    }
    Debug{} << "File not found: " << scriptPath;
    return false;
}

int main() {
    Debug{} << Directory::executableLocation();
    
    lua["printMessage"] = printMessage;

    Resource rs{"shadow-data"};

    sol::table game = lua.create_named_table("game");

    game["printMessage"] = printMessage;
    game["addEnemy"] = addEnemy;

    //lua.script(rs.get("script.lua"));
    
    //lua.script_file("../../src/script.lua");
    assert(executeScript("../../src/script.lua"));

    auto function_obj = lua["sumNumbers"];
    Function function = lua["sumNumbers"];
    std::cout << static_cast<double>(function(3,7)) << std::endl;

    auto entity = entityManager.create_entity();

    static_assert(metas::isRegistered<Person>(), "Person class is not registered!");
    static_assert(metas::getMemberCount<Person>() == 4, "Person does not have 4 members registered?");

/*
    metas::doForAllMembers<Person>([&obj, &object](auto& member)
                                   std::cout << member << std::endl;
                                   );
*/

    Person pperson;

    bool valid;
    std::tie(pperson, valid) = entity.add_component<Person>(15, "John");
    addEnemy(70, "Hakin");

    entityManager.for_each<TestComponent>([&](auto ent, auto &person) {
            j = person;std::cout << std::setw(4) << j << std::endl;
        });
    //pperson.setName("John");
    //pperson.setAge(99);

    //assert(pperson.getAge() == entity.get_component<Person>().getAge());

    return 0;
}
