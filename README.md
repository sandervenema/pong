# A barebones Pong

This is a very barebones, basic version of the classic game Pong, written in C.
It was mostly written to play around with SDL2 and C. Don't use this in
production.

## Build instructions

**Important:** Make sure you have installed the dependencies listed below. 

The build system is CMake, you can build it as follows:

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. 
make
```

and then you can simply start it with the following command:

```bash
./Pong
```

You can also start it in full-screen, as follows:

```bash
./Pong -f
```

## Controls

* `UP` and `DOWN` arrows move the brick for player 1;
* `W` and `S` moves the brick for player 2;
* `R` resets the game;
* `F` toggles full-screen rendering.

## Dependencies

* SDL2, version 2.0.14
* SDL2_image, version 2.0.5
* SDL2_ttf, version 2.20.2

## License

This game is released under the MIT license.
