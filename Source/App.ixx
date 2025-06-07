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

decltype(Container::Remove(0))
Container::Remove(SDL_WindowID windowID)
{
  auto const item = instances.find(windowID);
  if (item != instances.end()) [[likely]] {
    return instances.erase(item);
  }
  return instances.end();
}
}

// Events.c++
SDL_AppResult
MainEventHandler::Event(SDL_Event *event) const
{
  using TYPE = SDL_EventType;
  switch (event->type) {
    case TYPE::SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    default:
      if (
        event->type >= TYPE::SDL_EVENT_WINDOW_FIRST
        && event->type <= TYPE::SDL_EVENT_WINDOW_LAST)
      {
        return visit(
          [&event](auto &&window) { return window->Event(event->window); },
          Windows::Container::Get(event->window.windowID));
      }
      break;
  }
  return SDL_APP_CONTINUE;
}
