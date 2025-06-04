module;
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

module App;

struct App::Main
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
