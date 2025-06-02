[![Linux](https://github.com/Diyou/SDLTemplate/actions/workflows/linux.yml/badge.svg)](https://github.com/Diyou/SDLTemplate/actions/workflows/linux.yml)
[![macOS](https://github.com/Diyou/SDLTemplate/actions/workflows/macos.yml/badge.svg)](https://github.com/Diyou/SDLTemplate/actions/workflows/macos.yml)
[![Windows](https://github.com/Diyou/SDLTemplate/actions/workflows/windows.yml/badge.svg)](https://github.com/Diyou/SDLTemplate/actions/workflows/windows.yml)<br>
[![Android](https://github.com/Diyou/SDLTemplate/actions/workflows/android.yml/badge.svg)](https://github.com/Diyou/SDLTemplate/actions/workflows/android.yml)
[![Web](https://github.com/Diyou/SDLTemplate/actions/workflows/emscripten.yml/badge.svg)](https://github.com/Diyou/SDLTemplate/actions/workflows/emscripten.yml)

# SDLTemplate

A template to start a c++ module based multiplatform project with SDL3.<br>
Toolchain abstractions via [dotcmake](https://github.com/Diyou/.cmake)

### Contents:

- [Platform agnostic main](Source/main.c++) (via SDL_MAIN_USE_CALLBACKS)
- [Global App instance](Source/Modules/App.c++)
- [Event-Mapping](Source/Modules/Events.c++)
- [Basic Window class with default Renderer](Source/Modules/Windows/Default.c++)
