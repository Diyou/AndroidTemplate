#ifndef CMAKE_IMPORT_STD
#  include <filesystem>
#  include <format>
#  include <source_location>
#  include <span>
#endif

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import dotcmake;

using namespace std;

void constexpr Log(string const &view)
{
  SDL_Log("%s", view.c_str());
}

template< auto F >
void constexpr Log(
  string_view const &text,
  source_location    current = source_location::current())
{
  using VoidFunction = void (*)();
  Log(format(
    "[{}:{}][{}({})] {}\n",
    current.line(),
    current.column(),
    VoidFunction(F),
    dotcmake::GetFunctionName< F >(),
    text));
}

template< auto F >
void constexpr Debug(
  string_view const &text,
  source_location    current = source_location::current())
{
  if constexpr (dotcmake::Compiler::DEBUG) {
    Log< F >(text, current);
  }
}

void constexpr Log(
  string_view const &text,
  source_location    current = source_location::current())
{
  Log(format(
    "[{}:{}][{}]\t{}\n",
    current.line(),
    current.column(),
    current.function_name(),
    text));
}

void constexpr Debug(
  string_view const &text,
  source_location    current = source_location::current())
{
  if constexpr (dotcmake::Compiler::DEBUG) {
    Log(text, current);
  }
}

struct AppState
{
  span< char * > const Args;
  SDL_Window          *MainWindow = nullptr;
  SDL_Renderer        *Renderer   = nullptr;

  [[nodiscard]]
  auto
  Command() const
  {
    return filesystem::path{Args.front()};
  }

  SDL_AppResult
  OnInit()
  {
    if constexpr (dotcmake::Platform::Linux) {
      SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland");
    }
    if constexpr (dotcmake::Platform::Android) {
      // Prevent keyboard popup
      SDL_SetHint(SDL_HINT_ENABLE_SCREEN_KEYBOARD, "0");
    }

    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

    if (!SDL_Init(SDL_INIT_VIDEO)) [[unlikely]] {
      SDL_LogError(
        SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
      return SDL_APP_FAILURE;
    }

    auto const cmd = Command();

    MainWindow =
      SDL_CreateWindow(cmd.filename().c_str(), 600, 800, SDL_WINDOW_RESIZABLE);

    Renderer = SDL_CreateRenderer(MainWindow, nullptr);

    return SDL_APP_CONTINUE;
  }

  [[nodiscard]]
  SDL_AppResult
  Iterate() const
  {
    SDL_SetRenderDrawColor(Renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderClear(Renderer);
    SDL_RenderPresent(Renderer);

    return SDL_APP_CONTINUE;
  }

  SDL_AppResult
  OnEvent(SDL_Event *event)
  {
    switch (event->type) {
      case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
      default            : return SDL_APP_CONTINUE;
    }
  }

  ~AppState()
  {
    if (Renderer != nullptr) {
      SDL_DestroyRenderer(Renderer);
    }
    if (MainWindow != nullptr) {
      SDL_DestroyWindow(MainWindow);
    }
    SDL_Quit();
  }
};

SDLMAIN_DECLSPEC SDL_AppResult SDLCALL
SDL_AppInit(void **appstate, int argc, char *argv[])
{
  auto *app = new AppState{span< char * >{argv, argc + argv}};
  *appstate = app;
  return app->OnInit();
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
