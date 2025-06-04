module;
#ifndef CMAKE_IMPORT_STD
#  include <format>
#  include <memory>
#endif
#include <SDL3/SDL.h>
module App:Window;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import :Events;

namespace Windows {
using namespace std;

template< typename T > struct Window : WindowEvents< T >
{
  static constexpr int DEFAULT_WIDTH  = 720;
  static constexpr int DEFAULT_HEIGHT = 480;

  SDL_Window          *window;

private:
  Window(SDL_Window *window)
  : window{window}
  {}

public:
  [[nodiscard]]
  SDL_AppResult
  Iterate() const
  {
    return SDL_APP_CONTINUE;
  }

  static unique_ptr< T >
  Create(string const &title, int width, int height, SDL_WindowFlags flags)
  {
    auto *window = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (window == nullptr) [[unlikely]] {
      return nullptr;
    }

    auto pointer = make_unique< T >(window);
    pointer->Link(window);
    return pointer;
  }

  ~Window() { SDL_DestroyWindow(window); }

  friend T;
};

struct BasicWindow : Window< BasicWindow >
{
  BasicWindow(SDL_Window *window)
  : Window{window}
  {}

  static unique_ptr< BasicWindow >
  Create(string const &title, int width, int height, SDL_WindowFlags flags)
  {
    return Window::Create(title, width, height, flags);
  }
};
}
