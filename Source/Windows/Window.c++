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

  template< typename Variant, typename... Args >
  static pair< Container::iterator, bool >
  Create(SDL_Window *window, Args &&...args)
  {
    return Container::Emplace< Variant >(window, std::forward< Args >(args)...);
  }

  ~Window() { SDL_DestroyWindow(window); }
};

struct BasicWindow : Window
{
  BasicWindow(SDL_Window *window)
  : Window{window}
  {}
};
}
