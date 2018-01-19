#if !defined(MYAPPLICATION_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Joaqim Planstedt $
   ======================================================================== */

#define MYAPPLICATION_H

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Platform/GlfwApplication.h>

using namespace Magnum;

class MyApplication: public Platform::Application {
public:
    explicit MyApplication(const Arguments& args);
    void drawEvent() override;
private:
};



#endif
