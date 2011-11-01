#include "StdAfx.h"
#include "ReplVar.h"

#include <map>

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
//------------------------------------------------------------------------
//! Заменить дату/время в строке на *
/*
%w - abbreviated weekday (Mon, Tue, ...) 
%W - full weekday (Monday, Tuesday, ...) 
%b - abbreviated month (Jan, Feb, ...) 
%B - full month (January, February, ...) 
%d - zero-padded day of month (01 .. 31) 
%e - day of month (1 .. 31) 
%f - space-padded day of month ( 1 .. 31) 
%m - zero-padded month (01 .. 12) 
%n - month (1 .. 12) 
%o - space-padded month ( 1 .. 12) 
%y - year without century (70) 
%Y - year with century (1970) 
%H - hour (00 .. 23) 
%h - hour (00 .. 12) 
%a - am/pm 
%A - AM/PM 
%M - minute (00 .. 59) 
%S - second (00 .. 59) 
%s - seconds and microseconds (equivalent to %S.%F) 
%i - millisecond (000 .. 999) 
%c - centisecond (0 .. 9) 
%F - fractional seconds/microseconds (000000 - 999999) 
%z - time zone differential in ISO 8601 format (Z or +NN.NN) 
%Z - time zone differential in RFC format (GMT or +NNNN) 
%% - percent sign
*/
std::string ReplVar::replaceDateMask(const std::string &str)
{
	string tmpStr(str);
	map<string,string> Masks; // Параметры на замену
	Masks["%w"]="???";
	Masks["%W"]="*";
	Masks["%b"]="???";
	Masks["%B"]="*"; //full month (January, February, ...) 
	Masks["%d"]="??"; //- zero-padded day of month (01 .. 31) 
	Masks["%e"]="*"; //day of month (1 .. 31) 
	Masks["%f"]="??"; //- space-padded day of month ( 1 .. 31) 
	Masks["%m"]="??"; //- zero-padded month (01 .. 12) 
	Masks["%n"]="*"; //- month (1 .. 12) 
	Masks["%o"]="??"; //space-padded month ( 1 .. 12) 
	Masks["%y"]="??"; //year without century (70) 
	Masks["%Y"]="????"; //%Y - year with century (1970) 
	Masks["%H"]="??"; //%H - hour (00 .. 23) 
	Masks["%h"]="??"; //%h - hour (00 .. 12) 
	Masks["%a"]="??"; //%a - am/pm 
	Masks["%A"]="??"; //%A - AM/PM 
	Masks["%M"]="??"; //%M - minute (00 .. 59) 
	Masks["%S"]="??"; //%S - second (00 .. 59) 
	Masks["%s"]="??."; //%s - seconds and microseconds (equivalent to %S.%F) 
	Masks["%M"]="??"; //
	Masks["%M"]="??"; //
	Masks["%M"]="??"; //
	


%i - millisecond (000 .. 999) 
%c - centisecond (0 .. 9) 
%F - fractional seconds/microseconds (000000 - 999999) 
%z - time zone differential in ISO 8601 format (Z or +NN.NN) 
%Z - time zone differential in RFC format (GMT or +NNNN) 
%% - percent sign


	map<string,string>::const_iterator it=Masks.begin();
	for (;it!=Masks.end();++it)
	{
	//	From="%FileExt"; // Расширение файла
	//To=pPath.getExtension();
	tmpStr=replace (tmpStr,it->first,it->second);
	}
}
