module;
#ifndef CMAKE_IMPORT_STD
#  include <memory>
#  include <string>
#endif
#include <SDL3/SDL.h>
module App:Window;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import :Events;

namespace Windows {
using namespace std;

struct Window : WindowEvents
{
  static inline int DEFAULT_WIDTH  = 720;
  static inline int DEFAULT_HEIGHT = 480;

  SDL_Window       *window;

  Window(SDL_Window *window)
  : window{window}
  {}

  [[nodiscard]]
  SDL_AppResult
  Iterate() const
  {
    return SDL_APP_CONTINUE;
  }

  template< typename T >
  static unique_ptr< T >
  Create(
    string const   &title,
    SDL_WindowFlags flags  = 0,
    int             width  = DEFAULT_WIDTH,
    int             height = DEFAULT_HEIGHT)
  {
    auto *window = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (window == nullptr) [[unlikely]] {
      return nullptr;
    }

    auto pointer = make_unique< T >(window);
    return pointer;
  }

  ~Window() { SDL_DestroyWindow(window); }
};

struct BasicWindow : Window
{
  BasicWindow(SDL_Window *window)
  : Window{window}
  {}

  static unique_ptr< BasicWindow >
  Create(
    string const   &title,
    SDL_WindowFlags flags  = 0,
    int             width  = DEFAULT_WIDTH,
    int             height = DEFAULT_HEIGHT)
  {
    return Window::Create< BasicWindow >(title, flags, width, height);
  }
};
}
