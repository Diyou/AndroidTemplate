module;
#ifndef CMAKE_IMPORT_STD
#  include <filesystem>
#  include <span>
#  include <unordered_set>
#  include <vector>
#endif
#include <SDL3/SDL.h>
module App:App;

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import dotcmake;

using namespace std;
using path = filesystem::path;

class App
{
  path const                         arg0;
  vector< string_view > const        args;
  unordered_set< string_view > const videoDrivers;

public:
  constexpr static string_view NAME       = PROJECT_NAME;
  constexpr static string_view VERSION    = PROJECT_VERSION;
  constexpr static string_view IDENTIFIER = PROJECT_ID;

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
  App(span< char * > const &argv)
  : arg0{argv.front()}
  , args{CreateArgsVector(argv.size() - 1, argv.data() + 1)}
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

  App(int argc, char **argv)
  : App{span< char * >{argv, argv + argc}}
  {}

  static decltype(args)
  CreateArgsVector(size_t argc, char **argv)
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

  static inline App *instance = nullptr;

public:
  // Give public access to the SDL_main callbacks
  struct Main;
};
