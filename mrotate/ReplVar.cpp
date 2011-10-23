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
//! �������� ��� ����� � ���� � �������
void ReplVar::replaceFileAndDate(std::vector<std::string> &strVect,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime)
{
	vector<string>::iterator it=strVect.begin();
	
	for (;it!=strVect.end();++it)
	{
		*it=replaceFileAndDate(*it,FileName,ArhFileName,dateTime);
	}
}
//! �������� ��� ����� � ���� � ������
std::string ReplVar::replaceFileAndDate(const std::string &str,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime)
{
string ret(str);
ret=replaceFile(ret,FileName,ArhFileName);
ret=replaceDate(ret,dateTime);
return ret;
}
//------------------------------------------------------------------------
//! �������� ��� ����� � ��� ������ � ������
std::string ReplVar::replaceFile(const std::string &str,const std::string &FileName,const std::string &ArhFileName)
{
	string From,To;
	string tmpStr(str);
	Path pPath(FileName);
	pPath.makeFile();
	//������ ������� ����� �����
	From="%FullFileName"; // ������ ��� �����
	To=pPath.toString();
	tmpStr=replace (tmpStr,From,To);
	//������ ����� �����
	From="%FileName"; // ������ ��� �����
	To=pPath.getFileName();
	tmpStr=replace (tmpStr,From,To);
	From="%FileBaseName"; // ��� ����� ��� ����������
	To=pPath.getBaseName();
	tmpStr=replace (tmpStr,From,To);
	From="%FileExt"; // ���������� �����
	To=pPath.getExtension();
	tmpStr=replace (tmpStr,From,To);

	if (!ArhFileName.empty())
	{
		From="%ArhFileName"; // ��� ����� ������
		tmpStr=replace (tmpStr,From,ArhFileName);
	}
	return tmpStr;
}

//------------------------------------------------------------------------
//! �������� ����/����� � ������, �� ��������� �� �������
std::string ReplVar::replaceDate(const std::string &str,Poco::DateTime dateTime)
{
string tmpStr;
DateTime pDateTime;
if (dateTime!=0) pDateTime=dateTime;
tmpStr=DateTimeFormatter::format(pDateTime,str);
return tmpStr;
}
