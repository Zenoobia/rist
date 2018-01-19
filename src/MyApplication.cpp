#include "MyApplication.h"

using namespace Magnum;

MyApplication::MyApplication(const Arguments &args)
        : Platform::Application{ args, Configuration{}
                                 .setTitle("")
                                 .setFlags(Configuration::Flag::Debug) }
{

}

void MyApplication::drawEvent() {
    defaultFramebuffer.clear(FramebufferClear::Color);

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(MyApplication)
