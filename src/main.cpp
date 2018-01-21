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
#include <Corrade/Utility/System.h>
#include <Corrade/TestSuite/Tester.h>

using namespace Corrade::Utility;

#include <json.hpp>

#include "ScriptManager.cpp"

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



bool addEnemy(EntityManager *entityManager, const int value, const std::string name) {
    //auto ent = entityManager.create_entity();
    //auto[comp, val] = ent.add_component<TestComponent>(value, name);
    return (entityManager->create_entity(TestComponent{value, name}).get_status() == entity_status::OK);
}

std::pair<json, bool> openJson(const std::string &filePath) {
    bool isValid = false;
    json j;

    if(Directory::fileExists(filePath)) {
        try {
            j = json::parse(Directory::readString(filePath));
        } catch (json::parse_error& e)
        {
            std::cout << ": error" << e.what() << '\n'
                      << "Failed to parse script: " << Directory::filename(filePath)
                      << " - (" << Directory::join(Directory::path(Directory::executableLocation()), filePath)
                      << ")\n" << std::endl;
            //TODO: A create a path variable
            goto exit;
        }
        isValid = true;
        goto exit;
    }
    Debug{} << "File not found: " << filePath;
exit:
    return {j, isValid};
}

namespace Corrade {
    struct MyTest : public TestSuite::Tester {
        explicit MyTest();

        void metas() {
            static_assert(metas::isRegistered<Person>(), "Person class is not registered!");
            static_assert(metas::getMemberCount<Person>() == 4, "Person does not have 4 members registered?");
            static_assert(metas::isRegistered<TestComponent>(), "TestComponent class is not registered!");
            static_assert(metas::getMemberCount<TestComponent>() == 2, "TestComponent does not have 2 members registered?");
            CORRADE_VERIFY(true);
        }

        void luaFunctions() {

            ScriptManager scriptManager;
            auto game = scriptManager.createTable("game");
        
            game["printMessage"] = printMessage;


            EntityManager entityManager;

            //game["entityManager"] = entityManager;
            //game["addEnemy"] = addEnemy;
            scriptManager.addFunction("printMessage", &printMessage);
        
            //Resource rs{"shadow-data"};
            //lua.script(rs.get("script.lua"));
/* NOTE: Works with Corrade ResourceManager,
   but defeats the purpose of runtime scripting */
        
            CORRADE_VERIFY(scriptManager.executeScript("../../src/script.lua"));
        
            Function function = scriptManager.getFunction("sumNumbers");

            CORRADE_VERIFY(function.valid());
            CORRADE_COMPARE(static_cast<double>(function(3,7)), 10.);
        }


        void jsonSerializeAndDeserialize() {
            json j;
            Person p(13, "K");
            j = p;
            p = j;

            CORRADE_VERIFY(j["age"] == p.getAge());
            j = "{ \"value\": 14, \"name\": \"Ja\" }"_json;
            CORRADE_VERIFY(j["value"] == 14);
            TestComponent tc = j;
            CORRADE_VERIFY(j["value"] == tc.getValue());

            Debug{} << j.dump(4);

            Directory::writeString("tc.json", j.dump(4));

            auto [loaded_json, valid] = openJson("tc.json");
            CORRADE_VERIFY(valid);
            CORRADE_VERIFY(loaded_json["value"] == tc.getValue());
        }

        void entitySystem() {
            json j;
            auto tc = TestComponent{1,"J"};

            #if 0
            EntityManager entityManager;

            metas::doForAllMembers<TestComponent>(
                [&tc, &j](const auto& member) {
                    auto& valueName = j[member.getName()];
                    if(member.canGetConstRef()) {
                        valueName = member.get(tc);
                    } else if (member.hasGetter()) {
                        valueName = member.getCopy(tc);
                    }
                    std::cout << member.getName() << valueName << std::endl;
                });

            auto entity = entityManager.create_entity();
            auto [pperson, valid] = entity.add_component<TestComponent>(15, "John");
            CORRADE_COMPARE(entity.get_component<TestComponent>().getValue(), 15);

            CORRADE_VERIFY(addEnemy(&entityManager, 70, "Hakin"));

            entityManager.for_each<TestComponent>([&](auto ent, auto &person) {
                    j = person;std::cout << std::setw(4) << j << std::endl;
                    TestComponent testc = j;
                    CORRADE_COMPARE(j["value"], testc.getValue());
                });
            #endif

            Person p(13, "K");
            j = p;
            p = j;

            CORRADE_COMPARE(j["age"], p.getAge());
            
            j = "{ \"value\": 14, \"name\": \"Ja\" }"_json;
            CORRADE_COMPARE(j["value"], 14);
            tc = j;
            CORRADE_COMPARE(j["value"], tc.getValue());

            //std::cout << j.dump(4) << std::endl;
            Debug{} << j.dump(4);
            
            Directory::writeString("tc.json", j.dump(4));
            j = Directory::readString("../../src/enemy.json");

            //assert(j["value"] == 24);
            //j = Directory::readString("tc.json");
            //assert(j["value"] == 14);


        }

    };

    MyTest::MyTest() {
        addTests({
                &MyTest::metas,
                    &MyTest::luaFunctions,
                    &MyTest::jsonSerializeAndDeserialize,
                    &MyTest::entitySystem
                    });
    }
} // end of namespace Corrade
        
CORRADE_TEST_MAIN(Corrade::MyTest)
#if 0
int main() {
    Resource rs{"shadow-data"};
    Debug{} << Directory::executableLocation();
    json j;

    static_assert(metas::isRegistered<Person>(), "Person class is not registered!");
    static_assert(metas::getMemberCount<Person>() == 4, "Person does not have 4 members registered?");
    static_assert(metas::isRegistered<TestComponent>(), "TestComponent class is not registered!");
    static_assert(metas::getMemberCount<TestComponent>() == 2, "TestComponent does not have 2 members registered?");

    auto tc = TestComponent{1,"J"};
#if 0
    metas::doForAllMembers<TestComponent>(
        [&tc, &j](const auto& member) {
            auto& valueName = j[member.getName()];
            if(member.canGetConstRef()) {
                valueName = member.get(tc);
            } else if (member.hasGetter()) {
                valueName = member.getCopy(tc);
            }
            std::cout << member.getName() << valueName << std::endl;
        });

    auto entity = entityManager.create_entity();
    auto [pperson, valid] = entity.add_component<TestComponent>(15, "John");
    addEnemy(70, "Hakin");

    entityManager.for_each<TestComponent>([&](auto ent, auto &person) {
            j = person;std::cout << std::setw(4) << j << std::endl;
            //TestComponent tc = j;
            //assert(j["value"] == tc.getValue());
        });

    Person p(13, "K");
    j = p;
    p = j;

    assert(j["age"] == p.getAge());
#endif
    j = "{ \"value\": 14, \"name\": \"Ja\" }"_json;
    assert(j["value"] == 14);
    tc = j;
    assert(j["value"] == tc.getValue());

    //std::cout << j.dump(4) << std::endl;
    Debug{} << j.dump(4);

    Directory::writeString("tc.json", j.dump(4));
    j = Directory::readString("../../src/enemy.json");

    //assert(j["value"] == 24);
    //j = Directory::readString("tc.json");
    //assert(j["value"] == 14);

    return 0;
}
#endif
