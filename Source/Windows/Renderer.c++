module;
#ifndef CMAKE_IMPORT_STD
#  include <format>
#  include <unordered_map>
#  include <variant>
#endif
#include <SDL3/SDL.h>
#include <cassert>
module App:Renderer;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import :Window;
import :App;
import Logger;

namespace Windows {
using namespace std;

struct Renderer : BasicWindow
{
  static inline int DEFAULT_WIDTH  = 600;
  static inline int DEFAULT_HEIGHT = 800;

  SDL_Renderer     *renderer;
  bool              minimizeOnClose = false;

  Renderer(
    string const   &title,
    SDL_WindowFlags flags  = 0,
    int             width  = DEFAULT_WIDTH,
    int             height = DEFAULT_HEIGHT)
  : BasicWindow{title, flags, width, height}
  , renderer{SDL_CreateRenderer(handle, nullptr)}
  {
    assert(renderer != nullptr);
    SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
  }

  // Override Window Event
  void
  OnResized(SDL_WindowEvent &event) final
  {
    auto const &width  = event.data1;
    auto const &height = event.data2;
  }

  void
  OnCloseRequest(SDL_WindowEvent &event) final
  {
    if (minimizeOnClose) {
      Minimize();
    }
    else {
      Destroy();
    }
  }

  [[nodiscard]]
  SDL_AppResult
  Iterate() const final
  {
    SDL_SetRenderDrawColor(renderer, 0x00, 0xd0, 0xd0, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return BasicWindow::Iterate();
  }

  void
  Destroy() const
  {
    SDL_DestroyRenderer(renderer);
    BasicWindow::Destroy();
  }
};
}
