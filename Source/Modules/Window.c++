module;
#include <SDL3/SDL.h>
export module Window;

export struct Window
{
  SDL_Window   *window;
  SDL_Renderer *renderer;

  [[nodiscard]]
  SDL_AppResult
  Iterate() const
  {
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
  }

  ~Window()
  {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
};
