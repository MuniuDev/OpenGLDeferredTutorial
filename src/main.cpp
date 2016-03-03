/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Common.hpp>
#include <Renderer.hpp>
#include <WindowEventHandler.hpp>

int main(int argc, char* argv[]) {
    auto renderer = std::make_shared<RendererContext>(800,600,"OpenGLDemo");
    auto eventHandler = std::make_shared<WindowEventHandler>(renderer);

    renderer->InitGL();
    bool running = true;
    while(running) {
      eventHandler->PollEvents();
      renderer->Tic();
      running = !eventHandler->ShouldClose();
    }
    return 0;
}
