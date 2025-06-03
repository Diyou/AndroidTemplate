#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

import App;

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppInit(void **appstate, int argc, char *argv[])
{
  return Main::Init(appstate, argc, argv);
}

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppEvent(void *appstate, SDL_Event *event)
{
  return Main::Event(appstate, event);
}

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppIterate(void *appstate)
{
  return Main::Iterate(appstate);
}

SDLMAIN_DECLSPEC void SDLCALL
SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  Main::Quit(appstate, result);
}
