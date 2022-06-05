project('datacorn', 'cpp', default_options : ['cpp_std=c++17'])

qt_mod = import('qt5')
qt_dep = dependency('qt5', modules : ['Core', 'Gui', 'Widgets', 'Sql'])

src_files = files([
	'src/main.cpp',
	'src/gui/flow_layout.cpp',
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

is_windows = host_machine.system() == 'windows'

rc_file = []
if is_windows
	win = import('windows')
	rc_file = win.compile_resources(
		'res/res.rc',
		depend_files : 'res/icon.ico',
	)
endif

executable(
	'datacorn',
	[src_files, processed_src_files, rc_file],
	dependencies : qt_dep,
	gui_app : true,
	link_args : is_windows ? ['-static', '-static-libgcc', '-static-libstdc++'] : []
)
