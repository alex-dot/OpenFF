#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

#include "graphics/BackgroundBillboard.h"

using namespace Magnum;

class OpenFF: public Platform::Application {
    public:
        explicit OpenFF(const Arguments& arguments);

    private:
        void drawEvent() override;

        BackgroundBillboard*  _bb;
};

OpenFF::OpenFF(const Arguments& arguments):
    Platform::Application{arguments, Configuration{}.setTitle("OpenFF")}
{
    _bb = new BackgroundBillboard();
}

void OpenFF::drawEvent() {
    GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

    _bb->draw();

    swapBuffers();
}

MAGNUM_APPLICATION_MAIN(OpenFF)
