module;
#ifndef CMAKE_IMPORT_STD
#  include <filesystem>
#  include <span>
#  include <unordered_set>
#endif
#include <SDL3/SDL.h>
module App:App;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import dotcmake;

using namespace std;
using path = filesystem::path;

struct App
{
  path const                         arg0;
  vector< string_view > const        args;
  unordered_set< string_view > const videoDrivers;

  constexpr static string_view       NAME       = PROJECT_NAME;
  constexpr static string_view       VERSION    = PROJECT_VERSION;
  constexpr static string_view       IDENTIFIER = PROJECT_ID;

  static path const &
  Path()
  {
    return State().arg0;
  }

  static string
  Executable()
  {
    return Path().filename().string();
  }

  static string
  Directory()
  {
    return Path().parent_path().string();
  }

  static decltype(args) &
  Arguments()
  {
    return State().args;
  }

  static App const &
  State()
  {
    return *instance;
  }

private:
  static inline App *instance = nullptr;

  App(int argc, char **argv)
  : arg0{argv[0]}
  , args{CreateArgsVector(argc - 1, argv + 1)}
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
  }

  static decltype(args)
  CreateArgsVector(int argc, char **argv)
  {
    span                             list{argv, argv + argc};
    remove_const_t< decltype(args) > views;
    views.reserve(argc);
    for (auto const &arg : list) {
      views.emplace_back(arg);
    }
    return views;
  }

  static decltype(videoDrivers)
  GetVideoDrivers()
  {
    size_t const                             count = SDL_GetNumVideoDrivers();
    remove_const_t< decltype(videoDrivers) > drivers;
    drivers.reserve(count);

    for (int i = 0; i < count; i++) {
      drivers.emplace(SDL_GetVideoDriver(i));
    }
    return drivers;
  }

public:
  // Give public access to the SDL_main callbacks
  struct Main;
  // Singleton
  App()            = delete;
  App(App const &) = delete;
  App(App &&)      = delete;
  void
  operator=(App const &) = delete;
  App &
  operator=(App &&other) = delete;
};
