#ifndef SQL_UTIL_HPP
#define SQL_UTIL_HPP
#include <QSqlQuery>
#include <QString>

QT_BEGIN_NAMESPACE
class QSqlDatabase;
QT_END_NAMESPACE

extern QString const SQL_DB_DRIVER;

extern QString const SQL_INSERT_DATA;
extern QString const SQL_INSERT_TEXT;

QSqlQuery buildQuery(QSqlDatabase& db, QString const& stmt);
void execQuery(QSqlQuery& q, bool doFirst = false);

#endif // SQL_UTIL_HPP
