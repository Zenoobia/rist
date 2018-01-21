#if !defined(TESTCOMPONENT_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim Planstedt $
   ======================================================================== */

#define TESTCOMPONENT_H

#include <string>

class TestComponent
{
public:
    TestComponent() {};
    TestComponent(const int value, const std::string name) : value(value), name(name) {};

    int getValue() const { return value; };  // NOTE: const very important with MetaStuff !!
    void setValue(const int newValue) { value = newValue; };

    friend auto metas::registerMembers<TestComponent>(); // Allowing private access for member registering
private:
    int value;
public:
    std::string name;
};

#include <Meta.h>

namespace metas {
    template<>
    inline auto registerMembers<TestComponent>()
    {
        return members(
            member("value", &TestComponent::getValue, &TestComponent::setValue),
            member("name", &TestComponent::name)
                       );
    }
}
#endif
