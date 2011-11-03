#include "StdAfx.h"
#include "ReplVar.h"

#include <map>

#include <Poco/DateTimeFormatter.h>
#include <Poco\String.h>
#include <Poco\Path.h>
#include <Poco\NumberFormatter.h>

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
void ReplVar::replaceFileAndDate(std::vector<std::string> &strVect,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime,int index)
{
	vector<string>::iterator it=strVect.begin();
	
	for (;it!=strVect.end();++it)
	{
		*it=replaceFileAndDate(*it,FileName,ArhFileName,dateTime);
	}
}
//! �������� ��� ����� � ���� � ������
std::string ReplVar::replaceFileAndDate(const std::string &str,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime,int index)
{
string ret(str);
ret=replaceFile(ret,FileName,ArhFileName);
ret=replaceDate(ret,dateTime);
return ret;
}
//------------------------------------------------------------------------
//! �������� ��� ����� � ��� ������ � ������
/* index = -1 - %Index=???
   index = -2 - %Index=%Index (��������� �� ����������)
   index >= 0 - %Index=index
*/
std::string ReplVar::replaceFile(const std::string &str,const std::string &FileName,const std::string &ArhFileName,int index)
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
	
	
	From="%Index"; // ������
	if (index>=0)
	{
		To=NumberFormatter::format0(index,zeroPad);
	}
	else
	{
		To=string(zeroPad,'?');
	}
	if (index!=-2)
	{
	tmpStr=replace (tmpStr,From,To);
	}
	
	return tmpStr;
}

//------------------------------------------------------------------------
//! �������� ����/����� � ������, ���� dateTime=0 - �� ���� ���������� �� *
std::string ReplVar::replaceDate(const std::string &str,Poco::DateTime dateTime)
{
string tmpStr;
//DateTime pDateTime;
if (dateTime==0) 
{
	tmpStr=replaceDateMask(str); // ������ �� *
	
}
else
{
	tmpStr=DateTimeFormatter::format(dateTime,str); // ������ �� ����
	// DateTimeFormatter::format(dateTime,str);
}
return tmpStr;
}
//------------------------------------------------------------------------
//! �������� ����/����� � ������ �� *
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
	map<string,string> Masks; // ��������� �� ������
	Masks["%w"]="???"; //%w - abbreviated weekday (Mon, Tue, ...)
	Masks["%W"]="*"; // %W - full weekday (Monday, Tuesday, ...)
	Masks["%b"]="???"; // %b - abbreviated month (Jan, Feb, ...) 
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
	Masks["%s"]="??.??????"; //%s - seconds and microseconds (equivalent to %S.%F) 
	Masks["%i"]="???"; //%i - millisecond (000 .. 999) 
	Masks["%c"]="?"; // %c - centisecond (0 .. 9) 
	Masks["%F"]="??????"; // %F - fractional seconds/microseconds (000000 - 999999) 
	Masks["%z"]="*"; //%z - time zone differential in ISO 8601 format (Z or +NN.NN) 
	Masks["%Z"]="??"; //%Z - time zone differential in RFC format (GMT or +NNNN)
	

	map<string,string>::const_iterator it=Masks.begin();
	for (;it!=Masks.end();++it)
	{
	//	From="%FileExt"; // ���������� �����
	//To=pPath.getExtension();
	tmpStr=replace (tmpStr,it->first,it->second);
	}
	return tmpStr;
}
