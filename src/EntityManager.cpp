#if !defined(ENTITYMANAGER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim Planstedt $
   ======================================================================== */

#define ENTITYMANAGER_H

#include "example/Person.h"
#include "TestComponent.hpp"

#include <entityplus/entity.h>
using namespace entityplus;

using CompList = component_list<Person, TestComponent>;
using TagList = tag_list<struct TA, struct TB>;

using entity_manager_t = entity_manager<CompList, TagList>; 

using Entity = typename entity_manager_t::entity_t;


class EntityManager {
public:

  EntityManager(const int value) : _value(value) {};
  EntityManager() : _value(0){};

  bool createEntity() {
    auto e =  _entity_manager.create_entity();
    return true;
  }

private:
  entity_manager_t _entity_manager;
  int _value;
};

struct two_things {
        int a;
        bool b;
};

#include <sol.hpp>
#include <type_traits>
namespace sol {

  //usertype<Entity> EntityType("Entity", sol::bases<entity>);

#if 0
  usertype<Entity> EntityType("Entity",
			      "new", sol::no_constructor,
			      "get_component", &Entity::get_component
			      );

  usertype<EntityManager> EntityManagerType("EntityManager",
					    "new", sol::no_constructor,
					    "create_entity", &EntityManager::create_entity,
					    "add_component", &EntityManager::add_component
					    );
#endif

}
#endif
