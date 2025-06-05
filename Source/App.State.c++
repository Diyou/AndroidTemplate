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

struct App
{
  filesystem::path const             arg0;
  span< char * > const               args;
  unordered_set< string_view > const videoDrivers;

  constexpr static string_view       NAME       = PROJECT_NAME;
  constexpr static string_view       VERSION    = PROJECT_VERSION;
  constexpr static string_view       IDENTIFIER = PROJECT_ID;

  static App const &
  State()
  {
    return *instance;
  }

  static filesystem::path
  Executable()
  {
    return State().arg0.filename();
  }

  static filesystem::path
  Directory()
  {
    return State().arg0.parent_path();
  }

private:
  static inline App *instance = nullptr;

  App(int argc, char **argv)
  : arg0{filesystem::path{argv[0]}.u8string()}
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
  }

  static decltype(videoDrivers)
  GetVideoDrivers()
  {
    size_t const count = SDL_GetNumVideoDrivers();
    remove_const_t< decltype(GetVideoDrivers()) > drivers;
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
