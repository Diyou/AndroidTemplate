module;
#ifndef CMAKE_IMPORT_STD
#  include <memory>
#  include <unordered_map>
#  include <variant>
#endif
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>

module App;
import :App;

import :Events;

using namespace std;

struct App::Main
{
  using Window = Windows::Default;

  struct MainWindow : MainEventHandler
  {
    Windows::Container::iterator window;
  };

  // avoids heap allocation
  alignas(App) static inline array< byte, sizeof(App) > appStackBuffer;
  static inline tuple< App *, MainWindow, bool > runtime;

  static SDL_AppResult
  Init(void **appstate, int argc, char **argv)
  {
    auto &[app, handler, running] = runtime;
    app                           = new (appStackBuffer.data()) App{argc, argv};
    App::instance                 = app;
    running                       = true;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
      [[unlikely]]
    {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    SDL_Rect bounds{0, 0, Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT};

    if constexpr (dotcmake::Platform::MOBILE) {
      auto const primaryDisplay = SDL_GetPrimaryDisplay();
      if (primaryDisplay == 0 || !SDL_GetDisplayBounds(primaryDisplay, &bounds))
        [[unlikely]]
      {
        SDL_LogError(
          SDL_LOG_CATEGORY_ERROR,
          "SDL_GetDisplayBounds failed: %s",
          SDL_GetError());
        return SDL_APP_FAILURE;
      }
    }

    auto const &[it, created] = Window::Create< Window >(
      App::Executable(),
      dotcmake::Platform::MOBILE
        ? SDL_WINDOW_FULLSCREEN
        : SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT,
      bounds.w,
      bounds.h);

    if (!created) [[unlikely]] {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "Window::Create() failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    handler.window = it;

    *appstate      = &runtime;

    return SDL_APP_CONTINUE;
  }

  using Pointer = decltype(runtime) *;

  static SDL_AppResult
  Event(void *appstate, SDL_Event *event)
  {
    auto &[app, handler, running] = *Pointer(appstate);
    switch (event->type) {
      case SDL_EventType::SDL_EVENT_WINDOW_DESTROYED:
        if (handler.window->first == event->window.windowID) [[unlikely]] {
          running = false;
        }
      default:
        return handler.Event(event);
    }
  }

  static SDL_AppResult
  Iterate(void *appstate)
  {
    auto &[app, handler, running] = *Pointer(appstate);
    if (!running) [[unlikely]] {
      return SDL_APP_SUCCESS;
    }
    return visit(
      [](auto &arg) { return arg.Iterate(); }, *handler.window->second);
  }

  static void
  Quit(void *appstate, SDL_AppResult result)
  {
    auto &[app, handler, running] = *Pointer(appstate);
    SDL_Quit();
    app->~App();
  }
};

extern "C"
{
  SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
  SDL_AppInit(void **appstate, int argc, char *argv[])
  {
    return App::Main::Init(appstate, argc, argv);
  }

  SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
  SDL_AppEvent(void *appstate, SDL_Event *event)
  {
    return App::Main::Event(appstate, event);
  }

  SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
  SDL_AppIterate(void *appstate)
  {
    return App::Main::Iterate(appstate);
  }

  SDLMAIN_DECLSPEC void SDLCALL
  SDL_AppQuit(void *appstate, SDL_AppResult result)
  {
    App::Main::Quit(appstate, result);
  }
}
