module;
#ifndef CMAKE_IMPORT_STD
#  include <format>
#  include <source_location>
#endif
#include <SDL3/SDL.h>
export module Logger;

#ifdef CMAKE_IMPORT_STD
import std;
#endif
import dotcmake;

using namespace std;

export
{
  template< auto F >
  void constexpr Log(
    string_view const &text,
    source_location    current = source_location::current())
  {
    using VoidFunction = void (*)();
    SDL_Log(
      "%s",
      format(
        "[{}:{}][{}({})]\t{}\n",
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
    SDL_Log(
      "%s",
      format(
        "[{}:{}][{}]\t{}\n",
        current.line(),
        current.column(),
        current.function_name(),
        text)
        .c_str());
  }

  void constexpr Debug(
    string_view const &text,
    source_location    current = source_location::current())
  {
    if constexpr (dotcmake::Compiler::DEBUG) {
      Log(text, current);
    }
  }
}
