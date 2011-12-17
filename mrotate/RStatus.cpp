#include "StdAfx.h"
#include "RStatus.h"

//#include <Poco\Util\IniFileConfiguration.h>

#include <Poco\AutoPtr.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco\Path.h>
//#include <Poco\DirectoryIterator.h>
#include <Poco\NumberParser.h>
#include <Poco\NumberFormatter.h>
#include <Poco\DateTimeFormatter.h>
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
	//save();
	//pConf->release();
}
//======================================================
//! ������������� ������� ���������� ������ ������������
void RStatus::setStatusFileName(const std::string &fName)
{
	statusFileName=fName;
}
//------------------------------------------------------------------------
//! ��������� �������
void RStatus::save()
{
 try
	{
	pConf->save(statusFileName);
	}
	catch (exception e)
	{
	poco_error_f1(*log,"Exception then saving status. %s",statusFileName);
	}
}
//------------------------------------------------------------------------
//! ���������� ���� ��������� �������
Poco::DateTime RStatus::getDate(const std::string &confName,const std::string &entryName)
{
	//Timestamp stampDate;
	DateTime rDate; // ������� ����
	
	if (pConf.isNull()) return rDate;
	//if (pConf==0) return rDate;
	rDate.assign(2001,1,1); // ������ 2001 ����
	string sDate;
	Int64 iDate;

	string KeyName=confName+"."+entryName;

	sDate=pConf->getString(KeyName,"");
	
	if (sDate.empty()) return rDate;

	

	if (!NumberParser::tryParse64(sDate,iDate)) return rDate;

	Timestamp tsDate=Timestamp::fromEpochTime(iDate); //fromUtcTime(iDate);// fromEpochTime(iDate);
	
	DateTime retDate(tsDate);
	//rDate=iDate;
	//Timestamp tsDate(iDate);
	
	//Timestamp rDate(iDate);

	return retDate;


}
//------------------------------------------------------------------------
//! ��������� ���� ��������� �������. ���� ���������� �������
void RStatus::setDate(const std::string &confName,const std::string &entryName)
{
	if (pConf.isNull()) return;
	//if (pConf==0) return;
	Timestamp nowDate;
	Int64 iDate=nowDate.epochTime();// utcTime();// epochTime();// epochMicroseconds();
	string strDate=NumberFormatter::format(iDate);
	string strmanDate=DateTimeFormatter::format(nowDate,"%Y-%m-%d %H:%M"); 

	string KeyName=confName+"."+entryName;

	pConf->setString(KeyName,strDate);
	pConf->setString(KeyName+".Date",strmanDate);
	save();
}
//------------------------------------------------------------------------
//! �������� ���������� �� �����
void RStatus::load()
{
// ����������� ���� ����� �� ����������
	//Poco::Path pPath(fileName);
	Poco::File pFile(statusFileName);
	
	if (!pFile.exists()) // ����� ���, ������� ������ ������������
	{
		pConf=new PropertyFileConfiguration();
		return;
	}
	
	if (!pFile.canWrite()) 
	{
		poco_error_f1(*log,"Can't write into status file. %s",statusFileName);
		return; // ���� �� ����� ���� ��������/�������
	}

	//AutoPtr<AbstractConfiguration> pConf;
	
	// ������� ������������ �� �����
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
