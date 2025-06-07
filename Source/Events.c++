module;
#ifndef CMAKE_IMPORT_STD
#  include <string_view>
#  include <unordered_map>
#  include <variant>
#endif
#include <SDL3/SDL.h>
module App:Events;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

using namespace std;

namespace Windows {
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
  Emplace(Args &&...args)
  {
    auto window = make_unique< Variant >(std::forward< Args >(args)...);
    return instances.emplace(window->GetID(), std::move(window));
  }

  static iterator
  end()
  {
    return instances.end();
  }
};

using Default = Renderer;
}

using EventType = SDL_EventType;

struct WindowEvents
{
  virtual void
  OnShow(SDL_WindowEvent &) {};

  virtual void
  OnHide(SDL_WindowEvent &) {};

  virtual void
  OnExposed(SDL_WindowEvent &) {};

  virtual void
  OnMoved(SDL_WindowEvent &) {};

  virtual void
  OnResized(SDL_WindowEvent &) {};

  virtual void
  OnPixelSizeChanged(SDL_WindowEvent &) {};

  virtual void
  OnMetalViewResized(SDL_WindowEvent &) {};

  virtual void
  OnMinimized(SDL_WindowEvent &) {};

  virtual void
  OnMaximized(SDL_WindowEvent &) {};

  virtual void
  OnRestored(SDL_WindowEvent &) {};

  virtual void
  OnMouseEnter(SDL_WindowEvent &) {};

  virtual void
  OnMouseLeave(SDL_WindowEvent &) {};

  virtual void
  OnFocusGained(SDL_WindowEvent &) {};

  virtual void
  OnFocusLost(SDL_WindowEvent &) {};

  virtual void
  OnCloseRequest(SDL_WindowEvent &) {};

  template< typename Self >
  SDL_AppResult
  Event(this Self &&self, SDL_WindowEvent &event)
  {
    using Type = SDL_EventType;

    switch (event.type) {
      case Type::SDL_EVENT_WINDOW_SHOWN  : self.OnShow(event); break;
      case Type::SDL_EVENT_WINDOW_HIDDEN : self.OnHide(event); break;
      case Type::SDL_EVENT_WINDOW_EXPOSED: self.OnExposed(event); break;
      case Type::SDL_EVENT_WINDOW_MOVED  : self.OnMoved(event); break;
      case Type::SDL_EVENT_WINDOW_RESIZED: self.OnResized(event); break;
      case Type::SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        self.OnPixelSizeChanged(event);
        break;
      case Type::SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
        self.OnMetalViewResized(event);
        break;
      case Type::SDL_EVENT_WINDOW_MINIMIZED  : self.OnMinimized(event); break;
      case Type::SDL_EVENT_WINDOW_MAXIMIZED  : self.OnMaximized(event); break;
      case Type::SDL_EVENT_WINDOW_RESTORED   : self.OnRestored(event); break;
      case Type::SDL_EVENT_WINDOW_MOUSE_ENTER: self.OnMouseEnter(event); break;
      case Type::SDL_EVENT_WINDOW_MOUSE_LEAVE: self.OnMouseLeave(event); break;
      case Type::SDL_EVENT_WINDOW_FOCUS_GAINED:
        self.OnFocusGained(event);
        break;
      case Type::SDL_EVENT_WINDOW_FOCUS_LOST: self.OnFocusLost(event); break;
      case Type::SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        self.OnCloseRequest(event);
        break;
      case Type::SDL_EVENT_WINDOW_HIT_TEST:
      case Type::SDL_EVENT_WINDOW_ICCPROF_CHANGED:
      case Type::SDL_EVENT_WINDOW_DISPLAY_CHANGED:
      case Type::SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
      case Type::SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
      case Type::SDL_EVENT_WINDOW_OCCLUDED:
      case Type::SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
      case Type::SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
      case Type::SDL_EVENT_WINDOW_DESTROYED:
      case Type::SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
      default                                          : break;
    }
    return SDL_APP_CONTINUE;
  }
};

struct MainEventHandler
{
  SDL_AppResult
  Event(SDL_Event *event);
};
