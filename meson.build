project('pong', 'c')
sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('SDL2_image')
sources = ['main.c', 'object.h', 'object.c', 'util.h', 'util.c', 'game.h', 'game.c']
executable('pong', sources,
    c_args: ['-std=c99', '-Wall' ,'-Wextra','-Werror', '-Wpedantic'],
    link_args: ['-lm'],
    dependencies: [sdl2_dep, sdl2_image_dep])
