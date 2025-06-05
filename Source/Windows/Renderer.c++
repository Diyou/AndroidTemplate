module;
#ifndef CMAKE_IMPORT_STD
#  include <filesystem>
#  include <format>
#  include <memory>
#endif
#include <SDL3/SDL.h>
module App:Renderer;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import :Window;
import :App;
import Logger;

namespace Windows {
using namespace std;

struct Renderer : Window< Renderer >
{
  static constexpr int DEFAULT_WIDTH  = 600;
  static constexpr int DEFAULT_HEIGHT = 800;

  SDL_Renderer        *renderer;

  Renderer(SDL_Window *window, SDL_Renderer *renderer)
  : Window{window}
  , renderer{renderer}
  {
    Debug(format("{} Launched Window", App::Executable().c_str()));
  }

  // Override Window Event
  void
  OnResized(SDL_WindowEvent &event) final
  {
    auto const &width  = event.data1;
    auto const &height = event.data2;
    Debug(format("Resized: ({}:{})", width, height));
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

  static unique_ptr< Renderer >
  Create(string const &title, int width, int height, SDL_WindowFlags flags)
  {
    auto *window = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (window == nullptr) [[unlikely]] {
      return nullptr;
    }

    auto *renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) [[unlikely]] {
      return nullptr;
    }

    auto pointer = make_unique< Renderer >(window, renderer);
    pointer->Link(window);
    return pointer;
  }

  ~Renderer() { SDL_DestroyRenderer(renderer); }
};
}
