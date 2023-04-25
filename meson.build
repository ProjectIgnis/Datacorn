project('datacorn', 'cpp',
	default_options : ['cpp_std=c++17', 'cpp_eh=none', 'cpp_rtti=false']
)

is_windows = host_machine.system() == 'windows'

qt_used_mods = ['Core', 'Gui', 'Widgets', 'Sql']
qt_used_mods += is_windows ? ['PlatformSupport', 'windows', 'pcre', 'freetype'] : []

qt_mod = import('qt5')
qt_dep = dependency('qt5', modules : qt_used_mods, main : true)

win_rc_file = []
win_deps = []
if is_windows
	win = import('windows')
	win_rc_file = win.compile_resources(
		'res/res.rc',
		depend_files : 'res/icon.ico',
	)
	compiler = meson.get_compiler('cpp')
	win_deps += compiler.find_library('ws2_32', required : true)
	win_deps += compiler.find_library('Winmm', required : true)
	win_deps += compiler.find_library('Imm32', required : true)
endif

src_files = files([
	'src/main.cpp',
	'src/gui/main_window.cpp'
])

moc_header_files = files([
	'src/gui/main_window.hpp'
])

ui_files = files([
	'src/gui/main_window.ui'
])

processed_src_files = qt_mod.preprocess(
	moc_headers : moc_header_files,
	ui_files : ui_files,
	qresources : 'res/res.qrc'
)

executable(
	'datacorn',
	[src_files, processed_src_files, win_rc_file],
	dependencies : [qt_dep, win_deps],
	gui_app : true,
	link_args : is_windows ? ['-static', '-static-libgcc', '-static-libstdc++'] : []
)
