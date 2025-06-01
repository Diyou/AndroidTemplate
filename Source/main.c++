#ifndef CMAKE_IMPORT_STD
#  include <filesystem>
#  include <format>
#  include <span>
#  include <unordered_set>
#endif

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import dotcmake;
import Logger;
import Window;

using namespace std;

struct AppState
{
  span< char * > const               args;
  filesystem::path const             arg0;
  unordered_set< string_view > const videoDrivers;

  unique_ptr< Window >               mainWindow = nullptr;

  AppState(size_t argc, char **argv)
  : args(argv, argc)
  , arg0{args.front()}
  , videoDrivers{GetVideoDrivers()}
  {
    if constexpr (dotcmake::Compiler::DEBUG) {
      SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    }
  }

  SDL_AppResult
  Init()
  {
    if constexpr (dotcmake::Platform::Linux) {
      if (videoDrivers.contains("wayland")) {
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland");
      }
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
      [[unlikely]]
    {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    SDL_Rect bounds{0, 0, 600, 800};
    if constexpr (dotcmake::Platform::MOBILE) {
      auto const primaryDisplay = SDL_GetPrimaryDisplay();
      if (primaryDisplay == 0 || !SDL_GetDisplayBounds(primaryDisplay, &bounds))
      {
        SDL_LogError(
          SDL_LOG_CATEGORY_ERROR,
          "SDL_GetDisplayBounds failed: %s",
          SDL_GetError());
        return SDL_APP_FAILURE;
      }
    }

    auto *window = SDL_CreateWindow(
      arg0.filename().c_str(), bounds.w, bounds.h, SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    auto *renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR,
        "SDL_CreateRenderer failed: %s",
        SDL_GetError());

      return SDL_APP_FAILURE;
    }

    mainWindow = make_unique< Window >(window, renderer);

    return SDL_APP_CONTINUE;
  }

  [[nodiscard]]
  SDL_AppResult
  Iterate() const
  {
    return mainWindow->Iterate();
  }

  SDL_AppResult
  OnEvent(SDL_Event *event)
  {
    switch (event->type) {
      case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
      default            : return SDL_APP_CONTINUE;
    }
  }

  ~AppState() { SDL_Quit(); }

  static unordered_set< string_view >
  GetVideoDrivers()
  {
    size_t const                driver_count = SDL_GetNumVideoDrivers();
    decltype(GetVideoDrivers()) drivers;
    drivers.reserve(driver_count);

    for (int i = 0; i < driver_count; i++) {
      drivers.emplace(SDL_GetVideoDriver(i));
    }
    return drivers;
  }
};

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppInit(void **appstate, int argc, char *argv[])
{
  auto *app = new AppState{static_cast< size_t >(argc), argv};
  *appstate = app;
  return app->Init();
}

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppEvent(void *appstate, SDL_Event *event)
{
  return static_cast< AppState * >(appstate)->OnEvent(event);
}

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppIterate(void *appstate)
{
  return static_cast< AppState * >(appstate)->Iterate();
}

SDLMAIN_DECLSPEC void SDLCALL
SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  delete static_cast< AppState * >(appstate);
}
