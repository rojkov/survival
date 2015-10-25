{
    'target_defaults': {
        'default_configuration': 'Release',
        'configurations': {
            'Release': {},
            'Coverage': {
                'cflags': [
                    '-fprofile-arcs',
                    '-ftest-coverage',
                    '-O0'
                ],
                'ldflags': [
                    '-lgcov',
                    '-coverage'
                ]
            }
        }
    },
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
                'src/tile.cpp',
                'src/tile.h',
                'src/viewport.cpp',
                'src/viewport.h',
                'src/geometry.cpp',
                'src/geometry.h',
                'src/worldposition.h',
                'src/worldposition.cpp',
                'src/graphalg/a_star_search.h',
                'src/graphalg/gridgraph.h',
                'src/graphalg/gridlocation.h',
                'src/graphalg/gridlocation.cpp',
                'src/commands/command.h',
                'src/commands/command.cpp',
                'src/commands/move_command.h',
                'src/commands/move_command.cpp',
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
        {
            'target_name': 'tst_geometry',
            'type': 'executable',
            'sources': [
                'tests/tst_geometry/tst_geometry.cpp',
                'src/geometry.cpp',
                'src/geometry.h',
                'src/worldposition.h',
                'src/worldposition.cpp',
            ],
            'include_dirs': [
                'src'
            ],
            'dependencies': [
                'gtest'
            ],
            'cflags': [
                '<!@(<(pkg-config) --cflags sdl2)',
                '-std=c++11',
                '-pedantic',
                '-g',
            ],
            'libraries': [
                '<!@(<(pkg-config) --libs-only-l sdl2)',
            ]
        },
        {
            'target_name': 'gtest',
            'type': 'static_library',
            'sources': [
                'gtest-1.7.0/src/gtest-all.cc',
            ],
            'cflags!': [ # Don't do coverage for gtest's code
                '-fprofile-arcs',
                '-ftest-coverage',
                '-O0'
            ],
            'include_dirs': [
                'gtest-1.7.0',
                'gtest-1.7.0/include'
            ],
            'direct_dependent_settings': {
                'libraries': [
                    '-pthread'
                ],
                'include_dirs': [
                    'gtest-1.7.0',
                    'gtest-1.7.0/include'
                ],
            }
        }
    ],
}
