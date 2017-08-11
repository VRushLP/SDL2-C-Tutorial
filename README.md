# LazyFooC
Adaptation of Lazy Foo' Productions' SDL2 Tutorial, translated from C++ to C.

The code starts to vary significantly starting lesson 10.

Since C has no classes, instead I translated the C++ class into a C struct and the member methods into function calls.

Then, because I placed the new struct and function call into a new set of files (`l_texture.c` and `l_texture.h`), that also meant two functions had to receive a reference to the global gRenderer pointer in `main.c`.