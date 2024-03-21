project('datacorn', 'cpp',
	default_options : ['cpp_std=c++17', 'cpp_eh=none', 'cpp_rtti=false']
)

is_windows = host_machine.system() == 'windows'

qt_used_mods = ['Core', 'Gui', 'Widgets', 'Sql']

qt_dep = dependency('qt5', 'qt6', modules : qt_used_mods, main : true)
qt_mod = import('qt@0@'.format(qt_dep.version().split('.')[0]))

win_rc_file = []
if is_windows
	win = import('windows')
	win_rc_file = win.compile_resources(
		'res/res.rc',
		depend_files : 'res/icon.ico',
	)
endif

src_files = files([
	'src/gui/confirm_dialog.cpp',
	'src/gui/database_editor_widget.cpp',
	'src/gui/main_window.cpp',
	'src/gui/new_card_dialog.cpp',
	'src/gui/sql_util.cpp',
	'src/main.cpp',
])

moc_files = qt_mod.compile_moc(headers : files([
	'src/gui/confirm_dialog.hpp',
	'src/gui/database_editor_widget.hpp',
	'src/gui/main_window.hpp',
	'src/gui/new_card_dialog.hpp',
]),
sources : files([
	'src/gui/database_editor_widget.cpp',
]))

ui_files = qt_mod.compile_ui(sources : files([
	'src/gui/confirm_dialog.ui',
	'src/gui/database_editor_widget.ui',
	'src/gui/main_window.ui',
	'src/gui/new_card_dialog.ui',
]))

subdir('res') # Provides ts_files and res_files

executable(
	'datacorn',
	[src_files, moc_files, ui_files, ts_files, res_files, win_rc_file],
	dependencies : [qt_dep],
	gui_app : true,
	link_args : is_windows ? ['-static', '-static-libgcc', '-static-libstdc++'] : []
)
