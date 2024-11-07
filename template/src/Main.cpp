#include "pch.hpp"
#include "Platform/Platform.hpp"

int main()
{
    using namespace dat;
    using namespace dat::platform;
    using namespace dat::graphics;

    logger::InitializeLogger();
    InitializePlatform();

    Window window;
    WindowProperties properties;
    GraphicsSpecification specification{ GraphicsAPI::OPENGL, 4, 5 };

    gl::SetupOpenGLWindowing(window, properties, specification);

    while (window.IsOpen())
    {
        window.PollEvents();
    }

    return 0;
}