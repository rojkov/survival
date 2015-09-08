{
    'variables': {
        'pkg-config': 'pkg-config',
    },
    'targets': [
        {
            'target_name': 'survival',
            'type': 'executable',
            'sources': [
                'src/survival.cpp',
                'src/app.cpp',
                'src/app.h',
                'src/world.cpp',
                'src/world.h',
                'src/lifeform.cpp',
                'src/lifeform.h',
                'src/terrain.cpp',
                'src/terrain.h',
                'src/viewport.cpp',
                'src/viewport.h',
                'src/geometry.cpp',
                'src/geometry.h',
                'src/graphalg/a_star_search.h',
                'src/graphalg/gridgraph.h',
            ],
            'cflags': [
                '<!@(<(pkg-config) --cflags sdl2)',
                '<!@(<(pkg-config) --cflags SDL2_image)',
                '-std=c++11',
                '-Wall',
                '-pedantic',
                '-g',
            ],
            'ldflags': [
            ],
            'libraries': [
                '<!@(<(pkg-config) --libs-only-l sdl2)',
                '<!@(<(pkg-config) --libs-only-l SDL2_image)'
            ],
        },
    ],
}
