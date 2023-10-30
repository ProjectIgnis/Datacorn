#include "sql_util.hpp"

#include <QDebug>
#include <QSqlError>

QString const SQL_DB_DRIVER("QSQLITE");

QString const SQL_INSERT_DATA(R"(
INSERT INTO datas (id,alias,setcode,type,atk,def,level,race,attribute,ot,category)
VALUES (?,?,?,?,?,?,?,?,?,?,?);
)");

QString const SQL_INSERT_TEXT(R"(
INSERT INTO texts (id,name,desc,str1,str2,str3,str4,str5,str6,str7,str8,str9,str10,str11,str12,str13,str14,str15,str16)
VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);
)");

QSqlQuery buildQuery(QSqlDatabase& db, QString const& stmt)
{
	QSqlQuery q(db);
	bool const qPrepareResult = q.prepare(stmt);
	Q_ASSERT(qPrepareResult);
	return q;
}

void execQuery(QSqlQuery& q, bool doFirst)
{
	bool const qExecResult = q.exec();
	if(!qExecResult)
		qDebug() << q.lastError();
	Q_ASSERT(qExecResult);
	if(doFirst)
	{
		bool const qFirstResult = q.first();
		Q_ASSERT(qFirstResult);
	}
}
