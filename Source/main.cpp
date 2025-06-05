module;
#ifndef CMAKE_IMPORT_STD
#  include <memory>
#endif
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>

module App;
import :App;

import :Windows;
import :Events;

using namespace std;

struct App::Main
{
  using Window = Default;

  struct MainWindow : MainEventHandler< Window >
  {
    unique_ptr< Window > window = nullptr;
  };

  static inline pair< App *, unique_ptr< MainWindow > > runtime{};

  static SDL_AppResult
  Init(void **appstate, int argc, char **argv)
  {
    auto &[app, handler] = runtime;
    App::instance        = new App{argc, argv};
    app                  = App::instance;
    handler              = make_unique< MainWindow >();

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

    handler->window = Window::Create(
      App::Executable(),
      bounds.w,
      bounds.h,
      dotcmake::Platform::MOBILE
        ? SDL_WINDOW_FULLSCREEN
        : SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT);

    if (handler->window == nullptr) {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "Window::Create() failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    *appstate = &runtime;

    return SDL_APP_CONTINUE;
  }

  using Pointer = decltype(runtime) *;

  static SDL_AppResult
  Event(void *appstate, SDL_Event *event)
  {
    auto const &[app, handler] = *Pointer(appstate);
    return handler->Event(event);
  }

  static SDL_AppResult
  Iterate(void *appstate)
  {
    auto const &[app, handler] = *Pointer(appstate);
    return handler->window->Iterate();
  }

  static void
  Quit(void *appstate, SDL_AppResult result)
  {
    auto const &[app, handler] = *Pointer(appstate);
    SDL_Quit();
    delete app;
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
