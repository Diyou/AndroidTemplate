#ifndef CMAKE_IMPORT_STD
#  include <iostream>
#  include <source_location>
#  include <span>
#endif

#ifdef CMAKE_IMPORT_STD
import std;
#endif

import dotcmake;

using namespace std;

template< auto F >
void constexpr Log(
  string_view const &text,
  source_location    current = source_location::current())
{
  using VoidFunction = void (*)();
  cout << format(
    "[{}:{}][{}({})] {}\n",
    current.line(),
    current.column(),
    VoidFunction(F),
    dotcmake::GetFunctionName< F >(),
    text);
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
  cout << format(
    "[{}:{}][{}]\t{}\n",
    current.line(),
    current.column(),
    current.function_name(),
    text);
}

void constexpr Debug(
  string_view const &text,
  source_location    current = source_location::current())
{
#ifndef NDEBUG
  Log(text, current);
#endif
}

int
main(int argc, char *argv[])
{
  span< char * > args{argv, static_cast< size_t >(argc)};
  for (auto const &arg : args) {
    cout << arg << endl;
  }
  Debug< main >("Hello World");
}
