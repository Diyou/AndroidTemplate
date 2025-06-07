module;
#ifndef CMAKE_IMPORT_STD
#  include <unordered_map>
#  include <variant>
#endif
#include <SDL3/SDL.h>
export module App;

import :Events;

// Make Window implementations visible
import :Windows;

using namespace std;

// Implementations
namespace Windows {
decltype(Container::instances) Container::instances;

decltype(Container::Get(0)) &
Container::Get(SDL_WindowID windowID)
{
  return Container::instances.at(windowID);
};

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
MainEventHandler::Event(SDL_Event *event)
{
  switch (event->type) {
    case EventType::SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    default:
      if (
        event->type >= EventType::SDL_EVENT_WINDOW_FIRST
        && event->type <= EventType::SDL_EVENT_WINDOW_LAST)
      {
        return visit(
          [&event](auto &&window) { return window->Event(event->window); },
          Windows::Container::Get(event->window.windowID));
      }
      break;
  }
  return SDL_APP_CONTINUE;
}
