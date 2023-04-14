#ifndef ARCHETYPES_HPP
#define ARCHETYPES_HPP
#include <QMap>
#include <QtGlobal> // quint16

using Archetype = std::pair<quint16, char const*>;

static QMap<quint16, char const*> const ARCHETYPES_MAP{
	Archetype{0x1, QT_TR_NOOP("Ally of Justice")},
	Archetype{0x2, QT_TR_NOOP("Genex")},
	Archetype{0x1002, QT_TR_NOOP("R-Genex")},
	Archetype{0x2002, QT_TR_NOOP("Genex Ally")},
	Archetype{0x3, QT_TR_NOOP("Horus the Black Flame Dragon")},
	Archetype{0x4, QT_TR_NOOP("Amazoness")},
	Archetype{0x5, QT_TR_NOOP("Arcana Force")},
	Archetype{0x6, QT_TR_NOOP("Dark World")},
	Archetype{0x7, QT_TR_NOOP("Ancient Gear")},
	Archetype{0x8, QT_TR_NOOP("HERO")},
	Archetype{0x3008, QT_TR_NOOP("Elemental HERO")},
	Archetype{0x5008, QT_TR_NOOP("Vision HERO")},
	Archetype{0x6008, QT_TR_NOOP("Evil HERO")},
	Archetype{0xa008, QT_TR_NOOP("Masked HERO")},
	Archetype{0xc008, QT_TR_NOOP("Destiny HERO")},
};

#endif // ARCHETYPES_HPP
