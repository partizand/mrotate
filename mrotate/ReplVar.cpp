#include "StdAfx.h"
#include "ReplVar.h"

#include <Poco/DateTimeFormatter.h>
#include <Poco\String.h>
#include <Poco\Path.h>

using namespace std;
using namespace Poco;

ReplVar::ReplVar(void)
{
}
//------------------------------------------------------------------------
ReplVar::~ReplVar(void)
{
}
//------------------------------------------------------------------------
//! Заменить имя файла и дату в векторе
void ReplVar::replaceFileAndDate(std::vector<std::string> &strVect,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime)
{
	vector<string>::iterator it=strVect.begin();
	
	for (;it!=strVect.end();++it)
	{
		*it=replaceFileAndDate(*it,FileName,ArhFileName,dateTime);
	}
}
//! Заменить имя файла и дату в строке
std::string ReplVar::replaceFileAndDate(const std::string &str,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime)
{
string ret(str);
ret=replaceFile(ret,FileName,ArhFileName);
ret=replaceDate(ret,dateTime);
return ret;
}
//------------------------------------------------------------------------
//! Заменить имя файла и имя архива в строке
std::string ReplVar::replaceFile(const std::string &str,const std::string &FileName,const std::string &ArhFileName)
{
	string From,To;
	string tmpStr(str);
	Path pPath(FileName);
	pPath.makeFile();
	//Замена полного имени файла
	From="%FullFileName"; // Только имя файла
	To=pPath.toString();
	tmpStr=replace (tmpStr,From,To);
	//Замена имени файла
	From="%FileName"; // Только имя файла
	To=pPath.getFileName();
	tmpStr=replace (tmpStr,From,To);
	From="%FileBaseName"; // Имя файла без расширения
	To=pPath.getBaseName();
	tmpStr=replace (tmpStr,From,To);
	From="%FileExt"; // Расширение файла
	To=pPath.getExtension();
	tmpStr=replace (tmpStr,From,To);

	if (!ArhFileName.empty())
	{
		From="%ArhFileName"; // имя файла архива
		tmpStr=replace (tmpStr,From,ArhFileName);
	}
	return tmpStr;
}

//------------------------------------------------------------------------
//! Заменить дату/время в строке, по умолчанию на текущие
std::string ReplVar::replaceDate(const std::string &str,Poco::DateTime dateTime)
{
string tmpStr;
DateTime pDateTime;
if (dateTime!=0) pDateTime=dateTime;
tmpStr=DateTimeFormatter::format(pDateTime,str);
return tmpStr;
}
