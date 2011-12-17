#include "StdAfx.h"
#include "RStatus.h"

//#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco\Path.h>
//#include <Poco\DirectoryIterator.h>
#include <Poco\NumberParser.h>
#include <Poco\NumberFormatter.h>
//#include <Poco\Glob.h>
#include <Poco\Timestamp.h>


using namespace std;
using namespace Poco;
using namespace Util;

RStatus::RStatus(Poco::Logger &logger):
statusFileName("mrotate.status")

{
	log=&logger;
	//load();
}


RStatus::~RStatus(void)
{
}
//======================================================
//! ������������� ������� ���������� ������ ������������
void RStatus::setStatusFileName(const std::string &fName)
{
	statusFileName=fName;
}

//------------------------------------------------------------------------
//! ���������� ���� ��������� �������
Poco::Timestamp RStatus::getDate(const std::string &confName,const std::string &entryName)
{
	Timestamp rDate;
	if (pConf.isNull()) return rDate;
	
	string sDate;
	Int64 iDate;

	string KeyName=confName+"."+entryName;

	sDate=pConf->getString(KeyName,"");
	if (sDate.empty()) return rDate;

	if (!NumberParser::tryParse64(sDate,iDate)) return rDate;

	rDate.fromEpochTime(iDate);
	//rDate=iDate;
	//Timestamp tsDate(iDate);
	
	//Timestamp rDate(iDate);

	return rDate;


}
//------------------------------------------------------------------------
//! ��������� ���� ��������� �������. ���� ���������� �������
void RStatus::setDate(const std::string &confName,const std::string &entryName)
{
	if (pConf.isNull()) return;
	Timestamp nowDate;
	Int64 iDate=nowDate.epochTime();// epochMicroseconds();
	string strDate=NumberFormatter::format(iDate);

	string KeyName=confName+"."+entryName;

	pConf->setString(KeyName,strDate);
	
}
//------------------------------------------------------------------------
//! �������� ���������� �� �����
void RStatus::load()
{
// ����������� ���� ����� �� ����������
	//Poco::Path pPath(fileName);
	Poco::File pFile(statusFileName);

	if (!pFile.exists()) return; // ����� ���
	if (!pFile.canWrite()) return; // ���� �� ����� ���� ��������/�������

	//AutoPtr<AbstractConfiguration> pConf;

	pConf=new PropertyFileConfiguration(statusFileName);
	
	/*
	AbstractConfiguration::Keys RootKeys;
	pConf-> keys("",RootKeys); // ������ �������� ������
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	//RotateEntry re;
	int i;
	for (i=0;i<RootKeys.size();++i)
	{
			AbstractConfiguration::Keys level2Keys;
			pConf-> keys(RootKeys.at(i),RootKeys); // ������ �������� ������
				
				KeyName=RootKeys.at(i)+".Source";
				KeyValue=pConf->getString(KeyName,"");
				rDates[RootKeys.at(i)]
	}
}
*/
	
}
