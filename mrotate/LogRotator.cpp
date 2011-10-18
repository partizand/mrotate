#include "StdAfx.h"
#include "LogRotator.h"


#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\Path.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco/NumberParser.h>

using namespace Poco;
using namespace Util;

LogRotator::LogRotator(void)
{
}


LogRotator::~LogRotator(void)
{
}
//! �������� ���������� ������� �� �����
void LogRotator::load(const std::string &fileName)
{
// ����������� ���� ����� �� ����������
	Poco::Path pPath(fileName);
	Poco::File pFile(pPath);

	if (!pFile.exists()) return; // ����� ���
	if (!pFile.canRead()) return; // ���� �� ����� ���� ��������

	string Ext=pPath.getExtension();
	toLowerInPlace(Ext); // ���������� ���������� �������

	AutoPtr<AbstractConfiguration> pConf;

	
	if (Ext.compare("ini")==0) // ������������ ini
	{
		pConf=new IniFileConfiguration(fileName);
	}
	if (Ext.compare("properties")==0) // ������������ prop
	{
		pConf=new PropertyFileConfiguration(fileName);
	}
	if (pConf.isNull()) return; // ��� ������� ����������
	
	load(pConf);
}

//! �������� �������� �������
void LogRotator::load(const Poco::Util::AbstractConfiguration *pConf)
{
RotateEntry tmpItem;
AbstractConfiguration::Keys RootKeys;
pConf-> keys("",RootKeys); // ������ �������� ������
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	int i;
	for (i=0;i<RootKeys.size();++i)
	
	 //for (AbstractConfiguration::Keys::const_iterator it = RootKeys.begin(); it != RootKeys.end(); ++it)
			{
				//KeyName=*it+".source";
				// ��������
				KeyName=RootKeys.at(i)+".source";
				tmpItem.source=pConf->getString(KeyName,"");
				if (tmpItem.source=="") 
				{
					//poco_error_f1(*AppLogger,"Source missing in config %s",RootKeys.at(i));
					continue;
				}
				//������ � ���
				KeyName=RootKeys.at(i)+".period";
				KeyValue=pConf->getString(KeyName,"0");
				tmpItem.period=convertPeriod(KeyValue,tmpItem.type);
				// ������

				KeyName=RootKeys.at(i)+".nametask";
				tmpTask.NameTask=pConf->getString(KeyName,"");
				if (tmpTask.NameTask=="") 
				{
					tmpTask.NameTask=RootKeys.at(i);
				}
				//cout<< *it<<endl;
			addTask(tmpTask);
			}
 }
}
//! �������������� ������� � int64
Int64 LogRotator::convertSize(std::string &strSize)
{
	strSize
}
//! �������������� ���������� ������� � int � ����������� ���� �������
int LogRotator::convertPeriod(std::string &strPeriod,Rotate::RotateType &rType)
{
	int iPeriod;
	//string sPeriod;
	trimInPlace(strPeriod); // �������� �������
	// ���� ������ 
	rType=Rotate::RotateType.Single; 
	if (icompare(strPeriod,"Daily")==0)
	{
		
		return 1;
	}
	if (icompare(strPeriod,"weekly")==0)
	{
		
		return 7;
	}
	if (icompare(strPeriod,"monthly")==0)
	{
		
		return 30;
	}
	// ����������� ������ � �����
	rType=Rotate::RotateType.Multiple;
	if (NumberParser::tryParse(strPeriod,iPeriod))
	{
		return iPeriod;
	}
	else
	{
		return 0;
	}

}

