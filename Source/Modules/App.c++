module;
#ifndef CMAKE_IMPORT_STD
#  include <filesystem>
#  include <span>
#  include <unordered_set>
#endif
#define SDL_MAIN_HANDLED
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
export module App;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import dotcmake;

import Windows;
import Events;

export using Window_Type = Windows::Default;

using namespace std;

export struct Main;

export struct App : private GlobalEventHandler< Window_Type >
{
  unique_ptr< Window_Type >          mainWindow = nullptr;

  filesystem::path const             arg0;
  span< char * > const               args;
  unordered_set< string_view > const videoDrivers;

  constexpr static string_view       NAME       = PROJECT_NAME;
  constexpr static string_view       VERSION    = PROJECT_VERSION;
  constexpr static string_view       IDENTIFIER = PROJECT_ID;

  static auto
  State()
  {
    return instance;
  }

  static Window_Type const *
  Window()
  {
    return instance->mainWindow.get();
  }

  static auto
  Location()
  {
    return instance->arg0.parent_path();
  }

private:
  static inline App *instance = nullptr;

  App(int argc, char **argv)
  : arg0{argv[0]}
  , args{argv + 1, argv + argc}
  , videoDrivers{GetVideoDrivers()}
  {
    SDL_SetAppMetadata(NAME.data(), VERSION.data(), IDENTIFIER.data());

    if constexpr (dotcmake::Compiler::DEBUG) {
      SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    }

    // Define hints
    if constexpr (dotcmake::Platform::Linux) {
      if (videoDrivers.contains("wayland")) {
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland");
      }
    }
    instance = this;
  }

  SDL_AppResult
  Init()
  {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
      [[unlikely]]
    {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    SDL_Rect bounds{
      0, 0, Window_Type::DEFAULT_WIDTH, Window_Type::DEFAULT_HEIGHT};

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

    mainWindow = Window_Type::Create(
      arg0.filename(),
      bounds.w,
      bounds.h,
      dotcmake::Platform::MOBILE
        ? SDL_WINDOW_FULLSCREEN
        : SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT);

    if (mainWindow == nullptr) {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "Window::Create() failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
  }

  [[nodiscard]]
  SDL_AppResult
  Iterate() const
  {
    return mainWindow->Iterate();
  }

  ~App() { SDL_Quit(); }

  static unordered_set< string_view >
  GetVideoDrivers()
  {
    size_t const                count = SDL_GetNumVideoDrivers();
    decltype(GetVideoDrivers()) drivers;
    drivers.reserve(count);

    for (int i = 0; i < count; i++) {
      drivers.emplace(SDL_GetVideoDriver(i));
    }
    return drivers;
  }

  friend struct Main;
};

struct Main
{
  using Pointer = App *;

  static SDL_AppResult
  Init(void **appstate, int argc, char **argv)
  {
    *appstate = new App(argc, argv);
    return Pointer(*appstate)->Init();
  }

  static SDL_AppResult
  Event(void *appstate, SDL_Event *event)
  {
    return Pointer(appstate)->Event(event);
  }

  static SDL_AppResult
  Iterate(void *appstate)
  {
    return Pointer(appstate)->Iterate();
  }

  static void
  Quit(void *appstate, SDL_AppResult result)
  {
    delete Pointer(appstate);
  }
};
