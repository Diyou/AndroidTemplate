#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

import App;

using Pointer = App *;

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppInit(void **appstate, int argc, char *argv[])
{
  *appstate = new App{argc, argv};
  return Pointer(*appstate)->Init();
}

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppEvent(void *appstate, SDL_Event *event)
{
  return Pointer(appstate)->Event(event);
}

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppIterate(void *appstate)
{
  return Pointer(appstate)->Iterate();
}

SDLMAIN_DECLSPEC void SDLCALL
SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  delete Pointer(appstate);
}
