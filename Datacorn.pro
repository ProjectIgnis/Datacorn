greaterThan(QT_MINOR_VERSION, 11)|greaterThan(QT_MAJOR_VERSION, 5) {
	#setting c++17 as standard this way is only possible on qt 5.12+
	#on older versions the specific compiler flag has to be used
	CONFIG += c++17
} else {
	win32-msvc* {
		QMAKE_CXXFLAGS += -std:c++17
	} else {
		QMAKE_CXXFLAGS += -std=c++17
	}
}
CONFIG += rtti_off exceptions_off
QT += core gui widgets sql

TARGET = Datacorn
TEMPLATE = app

OBJECTS_DIR = obj
MOC_DIR = obj
DESTDIR = binary

INCLUDEPATH += \
	src/ \
	src/gui

SOURCES += \
	src/main.cpp \
	src/gui/confirm_dialog.cpp \
	src/gui/database_editor_widget.cpp \
	src/gui/main_window.cpp \
	src/gui/new_card_dialog.cpp \
	src/gui/sql_util.cpp

HEADERS += \
	src/archetypes.hpp \
	src/gui/confirm_dialog.hpp \
	src/gui/database_editor_widget.hpp \
	src/gui/main_window.hpp \
	src/gui/new_card_dialog.hpp \
	src/gui/sql_util.hpp \
	src/gui/trim_on_paste_line_edit.hpp

FORMS += \
	src/gui/confirm_dialog.ui \
	src/gui/database_editor_widget.ui \
	src/gui/main_window.ui \
	src/gui/new_card_dialog.ui

TRANSLATIONS += \
		$$files(./res/*.ts, true)

#generating translations automatically is only supported since qt 5.12
greaterThan(QT_MINOR_VERSION, 11)|greaterThan(QT_MAJOR_VERSION, 5) {
	CONFIG += lrelease embed_translations
	QM_FILES_RESOURCE_PREFIX=/
} else {
	CONFIG(release, debug|release) {
		TRANSLATION_TARGET_DIR = $${OUT_PWD}/release/translations
		LANGREL_OPTIONS = -compress -nounfinished -removeidentical
	} else {
		TRANSLATION_TARGET_DIR = $${OUT_PWD}/debug/translations
		LANGREL_OPTIONS =
	}

	isEmpty(QMAKE_LRELEASE) {
		win32:LANGREL = $$[QT_INSTALL_BINS]\lrelease.exe
		else:LANGREL = $$[QT_INSTALL_BINS]/lrelease
	}

	langrel.input = TRANSLATIONS
	langrel.output = $$TRANSLATION_TARGET_DIR/${QMAKE_FILE_BASE}.qm
	langrel.commands = \
		$$LANGREL $$LANGREL_OPTIONS ${QMAKE_FILE_IN} -qm $$TRANSLATION_TARGET_DIR/${QMAKE_FILE_BASE}.qm
	langrel.CONFIG += no_link

	QMAKE_EXTRA_COMPILERS += langrel

	# Compile an initial version of translation files when running qmake
	# the first time and generate the resource file for translations.
	!exists($$TRANSLATION_TARGET_DIR) {
		mkpath($$TRANSLATION_TARGET_DIR)
	}
	qrc_entry = "<RCC>"
	qrc_entry += '  <qresource prefix="/">'
	write_file($$TRANSLATION_TARGET_DIR/translations.qrc, qrc_entry)
	for(tsfile, TRANSLATIONS) {
		qmfile = $$TRANSLATION_TARGET_DIR/$$basename(tsfile)
		qmfile ~= s/.ts$/.qm/
		system($$LANGREL $$LANGREL_OPTIONS $$tsfile -qm $$qmfile)
		qrc_entry = "	<file>$$basename(qmfile)</file>"
		write_file($$TRANSLATION_TARGET_DIR/translations.qrc, qrc_entry, append)
	}
	qrc_entry = "  </qresource>"
	qrc_entry += "</RCC>"
	write_file($$TRANSLATION_TARGET_DIR/translations.qrc, qrc_entry, append)
	RESOURCES += $$TRANSLATION_TARGET_DIR/translations.qrc
}

CONFIG += debug_and_release

RC_ICONS = res/icon.ico

ICON = res/icon.icns
