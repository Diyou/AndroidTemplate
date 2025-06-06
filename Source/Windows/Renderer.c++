module;
#ifndef CMAKE_IMPORT_STD
#  include <format>
#  include <unordered_map>
#  include <variant>
#endif
#include <SDL3/SDL.h>
module App:Renderer;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import :Windows;
import :Window;
import :App;
import Logger;

namespace Windows {
using namespace std;

struct Renderer : Window
{
  static inline int DEFAULT_WIDTH  = 600;
  static inline int DEFAULT_HEIGHT = 800;

  SDL_Renderer     *renderer;

  Renderer(SDL_Window *window, SDL_Renderer *renderer)
  : Window{window}
  , renderer{renderer}
  {
    SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
  }

  // Override Window Event
  void
  OnResized(SDL_WindowEvent &event) final
  {
    auto const &width  = event.data1;
    auto const &height = event.data2;
    Debug(format("Resized to ({}:{})", width, height));
  }

  [[nodiscard]]
  SDL_AppResult
  Iterate() const
  {
    SDL_SetRenderDrawColor(renderer, 0x00, 0xd0, 0xd0, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return Window::Iterate();
  }

  static Container::iterator
  Create(
    string const   &title,
    SDL_WindowFlags flags  = 0,
    int             width  = DEFAULT_WIDTH,
    int             height = DEFAULT_HEIGHT)
  {
    auto *window = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (window == nullptr) [[unlikely]] {
      return Container::end();
    }

    auto *renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) [[unlikely]] {
      return Container::end();
    }

    auto [it, emplaced] =
      Container::Emplace< Renderer >(window, window, renderer);

    return it;
  }

  ~Renderer() { SDL_DestroyRenderer(renderer); }
};
}
