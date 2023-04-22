qt_staticdep = dependency('qt5', modules : ['PlatformSupport', 'windows', 'pcre', 'freetype'])

compiler = meson.get_compiler('cpp')
ws2_32_dep = compiler.find_library('ws2_32', required: true)
Winmm_dep = compiler.find_library('Winmm', required: true)
Imm32_dep = compiler.find_library('Imm32', required: true)

static_dep += [qt_staticdep, ws2_32_dep, Winmm_dep, Imm32_dep]

src_files += files('windows_platform.cpp')