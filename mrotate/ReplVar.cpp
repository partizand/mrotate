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
void ReplVar::replaceFileAndDate(std::vector<std::string> &strVect,const std::string &FileName,const std::string &ArhFileName="",Poco::DateTime dateTime=0)
{
	vector<string>::iterator it=strVect.begin();
	
	for (;it!=strVect.end();++it)
	{
		replaceFile(*it,FileName,ArhFileName);
		replaceDate(*it,dateTime);
	}
}
//! �������� ��� ����� � ���� � ������
std::string ReplVar::replaceFileAndDate(const std::string &str,const std::string &FileName,const std::string &ArhFileName="",Poco::DateTime dateTime=0)
{
string ret(str);
ret=replaceFile(ret,FileName,ArhFileName);
ret=
}
//------------------------------------------------------------------------
//! �������� ��� ����� � ��� ������ � ������
std::string ReplVar::replaceFile(const std::string &str,const std::string &FileName,const std::string &ArhFileName="")
{
	string From,To;
	string tmpStr(FileName);
	Path pPath(FileName);
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
std::string ReplVar::replaceDate(const std::string &str,Poco::DateTime dateTime=0)
{
string tmpStr;
DateTime pDateTime;
if (dateTime!=0) pDateTime=dateTime;
tmpStr=DateTimeFormatter::format(pDateTime,str);
return tmpStr;
}
