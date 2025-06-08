module;
#ifndef CMAKE_IMPORT_STD
#  include <unordered_map>
#  include <variant>
#endif
#include <SDL3/SDL.h>
export module App;

import :Windows;
import :Events;

using namespace std;

// Implementations
namespace Windows {
// These need to know the sizes of the variants and couldn't be inlined
decltype(Container::instances) Container::instances;

decltype(Container::Get(0))
Container::Get(SDL_WindowID windowID)
{
  return instances.at(windowID);
}

decltype(Container::Remove(0))
Container::Remove(SDL_WindowID windowID)
{
  auto const item = instances.find(windowID);
  if (item != instances.end()) [[likely]] {
    return instances.erase(item);
  }
  return End();
}

template< typename Variant, typename... Args >
pair< Container::iterator, bool >
Container::Emplace(Args &&...args)
{
  auto window = make_unique< Variants >(
    in_place_type< Variant >, std::forward< Args >(args)...);

  SDL_WindowID const windowID =
    visit([](auto const &window) { return window.GetID(); }, *window);

  return instances.emplace(windowID, std::move(window));
}

}

// Events.c++
SDL_AppResult
MainEventHandler::Event(SDL_Event *event) const
{
  using TYPE = SDL_EventType;
  switch (event->type) {
    case TYPE::SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
    default:
      if (
        event->type >= TYPE::SDL_EVENT_WINDOW_FIRST
        && event->type <= TYPE::SDL_EVENT_WINDOW_LAST)
      {
        return visit(
          [&event](auto &window) { return window.WindowEvent(event->window); },
          *Windows::Container::Get(event->window.windowID));
      }
      break;
  }
  return SDL_APP_CONTINUE;
}
