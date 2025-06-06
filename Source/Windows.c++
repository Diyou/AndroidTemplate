module;
#ifndef CMAKE_IMPORT_STD
#  include <memory>
#  include <unordered_map>
#  include <variant>
#endif
#include <SDL3/SDL.h>
module App:Windows;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

namespace Windows {
using namespace std;
using Variants =
  variant< unique_ptr< struct BasicWindow >, unique_ptr< struct Renderer > >;

class Container
{
  static unordered_map< SDL_WindowID, Variants > instances;

public:
  using iterator = decltype(instances)::iterator;

  static Variants &
  Get(SDL_WindowID windowID);

  static iterator
  Remove(SDL_WindowID windowID);

  template< typename Variant, typename... Args >
  static pair< iterator, bool >
  Emplace(SDL_Window *window, Args &&...args)
  {
    return instances.emplace(
      SDL_GetWindowID(window),
      Variants(
        in_place_type< unique_ptr< Variant > >,
        make_unique< Variant >(std::forward< Args >(args)...)));
  }

  static iterator
  end()
  {
    return instances.end();
  }
};

using Default = Renderer;
}
