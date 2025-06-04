module;
#ifndef CMAKE_IMPORT_STD
#  include <string_view>
#endif
#include <SDL3/SDL.h>
export module Events;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

using namespace std;

export template< typename Window > struct WindowEvents
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

  // Helpers for GlobalEventHandler
  template< typename Self >
  auto
  Link(this Self &&self, SDL_Window *window)
  {
    auto const properties = SDL_GetWindowProperties(window);
    return SDL_SetPointerProperty(properties, WindowAddress.data(), &self);
  }

  static auto
  Get(SDL_WindowID const &window)
  {
    auto const properties =
      SDL_GetWindowProperties(SDL_GetWindowFromID(window));

    return static_cast< Window * >(
      SDL_GetPointerProperty(properties, WindowAddress.data(), nullptr));
  }

  constexpr static string_view WindowAddress = "🪟";
};

export template< typename Window >
  requires derived_from< Window, WindowEvents< Window > >
struct GlobalEventHandler
{
  SDL_AppResult
  Event(SDL_Event *event)
  {
    auto window = [&event]() {
      return WindowEvents< Window >::Get(event->window.windowID);
    };
    switch (event->type) {
      // Display Events
      case SDL_EventType::SDL_EVENT_DISPLAY_ORIENTATION:
      case SDL_EventType::SDL_EVENT_DISPLAY_ADDED:
      case SDL_EventType::SDL_EVENT_DISPLAY_REMOVED:
      case SDL_EventType::SDL_EVENT_DISPLAY_MOVED:
      case SDL_EventType::SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
      case SDL_EventType::SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
      case SDL_EventType::SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: break;
      // Window Events
      case SDL_EventType::SDL_EVENT_WINDOW_SHOWN:
        window()->OnShow(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_HIDDEN:
        window()->OnHide(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_EXPOSED:
        window()->OnExposed(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_MOVED:
        window()->OnMoved(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_RESIZED:
        window()->OnResized(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        window()->OnPixelSizeChanged(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
        window()->OnMetalViewResized(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_MINIMIZED:
        window()->OnMinimized(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_MAXIMIZED:
        window()->OnMaximized(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_RESTORED:
        window()->OnRestored(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_ENTER:
        window()->OnMouseEnter(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_LEAVE:
        window()->OnMouseLeave(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_FOCUS_GAINED:
        window()->OnFocusGained(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_FOCUS_LOST:
        window()->OnFocusLost(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        window()->OnCloseRequest(event->window);
        break;
      case SDL_EventType::SDL_EVENT_WINDOW_HIT_TEST:
      case SDL_EventType::SDL_EVENT_WINDOW_ICCPROF_CHANGED:
      case SDL_EventType::SDL_EVENT_WINDOW_DISPLAY_CHANGED:
      case SDL_EventType::SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
      case SDL_EventType::SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
      case SDL_EventType::SDL_EVENT_WINDOW_OCCLUDED:
      case SDL_EventType::SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
      case SDL_EventType::SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
      case SDL_EventType::SDL_EVENT_WINDOW_DESTROYED:
      case SDL_EventType::SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
      // Keyboard events
      case SDL_EventType::SDL_EVENT_KEY_DOWN:
      case SDL_EventType::SDL_EVENT_KEY_UP:
      case SDL_EventType::SDL_EVENT_TEXT_EDITING:
      case SDL_EventType::SDL_EVENT_TEXT_INPUT:
      case SDL_EventType::SDL_EVENT_KEYMAP_CHANGED:
      case SDL_EventType::SDL_EVENT_KEYBOARD_ADDED:
      case SDL_EventType::SDL_EVENT_KEYBOARD_REMOVED:
      case SDL_EventType::SDL_EVENT_TEXT_EDITING_CANDIDATES:
      // Mouse events
      case SDL_EventType::SDL_EVENT_MOUSE_MOTION:
      case SDL_EventType::SDL_EVENT_MOUSE_BUTTON_DOWN:
      case SDL_EventType::SDL_EVENT_MOUSE_BUTTON_UP:
      case SDL_EventType::SDL_EVENT_MOUSE_WHEEL:
      case SDL_EventType::SDL_EVENT_MOUSE_ADDED:
      case SDL_EventType::SDL_EVENT_MOUSE_REMOVED:
      // Gamepad events
      case SDL_EventType::SDL_EVENT_GAMEPAD_AXIS_MOTION:
      case SDL_EventType::SDL_EVENT_GAMEPAD_BUTTON_DOWN:
      case SDL_EventType::SDL_EVENT_GAMEPAD_BUTTON_UP:
      case SDL_EventType::SDL_EVENT_GAMEPAD_ADDED:
      case SDL_EventType::SDL_EVENT_GAMEPAD_REMOVED:
      case SDL_EventType::SDL_EVENT_GAMEPAD_REMAPPED:
      case SDL_EventType::SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
      case SDL_EventType::SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
      case SDL_EventType::SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
      case SDL_EventType::SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
      case SDL_EventType::SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
      case SDL_EventType::SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
      // Touch events
      case SDL_EventType::SDL_EVENT_FINGER_DOWN:
      case SDL_EventType::SDL_EVENT_FINGER_UP:
      case SDL_EventType::SDL_EVENT_FINGER_MOTION:
      case SDL_EventType::SDL_EVENT_FINGER_CANCELED:
      // Clipboard & drop events
      case SDL_EventType::SDL_EVENT_CLIPBOARD_UPDATE:
      case SDL_EventType::SDL_EVENT_DROP_FILE:
      case SDL_EventType::SDL_EVENT_DROP_TEXT:
      case SDL_EventType::SDL_EVENT_DROP_BEGIN:
      case SDL_EventType::SDL_EVENT_DROP_COMPLETE:
      case SDL_EventType::SDL_EVENT_DROP_POSITION:
      // Audio hotplug events
      case SDL_EventType::SDL_EVENT_AUDIO_DEVICE_ADDED:
      case SDL_EventType::SDL_EVENT_AUDIO_DEVICE_REMOVED:
      case SDL_EventType::SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED : break;
      case SDL_EVENT_QUIT                                       : return SDL_APP_SUCCESS;
      default                                                   : return SDL_APP_CONTINUE;
    }
    return SDL_APP_CONTINUE;
  }
};
