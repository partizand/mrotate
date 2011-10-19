#include "StdAfx.h"
#include "LogRotator.h"


#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\Path.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco\NumberParser.h>
#include <Poco\Glob.h>
#include <Poco\Timestamp.h>
#include <Poco\DateTime.h>

using namespace std;
using namespace Poco;
using namespace Util;

LogRotator::LogRotator(void)
{
}


LogRotator::~LogRotator(void)
{
}
//------------------------------------------------------------------------
//! ���������� ����� (�������� �������)
void LogRotator::rotate()
{
	int i;
	vector<string> fileList;
	// ���������� ��� ������
	for (i=0;i<items.size();++i)
	{
		currIndex=i;
		// �������� ������ ������ ��� ���������
		getFileList(fileList);

	}
}
//------------------------------------------------------------------------
//! �������� ������ ������ ��� ���������
void LogRotator::getFileList(std::vector<std::string> &fileList)
{
	set<string> files;
	fileList.clear();
	Glob::glob(items.at(currIndex).source,files); // ��������� ������� ��������� ������ ������ �� �����
	// ���������� ��� �����
	set<string>::iterator it=files.begin();
	for (;it!=files.end();++it)
	{
		// *it - ��� �����
		if (isRotateFile(*it)) // ��������� ���� ���� ��������
		{
			fileList.push_back(*it);
		}
	}

}
//! ���������� �������� ����
//------------------------------------------------------------------------
void rotateFile(const std::string &fileName)
{

}

//! ��������� ����� �� ���������� ������ ����
//------------------------------------------------------------------------
bool LogRotator::isRotateFile(const std::string &fileName)
{
Poco::File pFile(fileName);

	if (!pFile.exists()) return false; // ����� ���
	if (!pFile.isFile()) return false; // ��� �� ����
	if (!pFile.canRead()) return false; // ���� �� ����� ���� ��������

	if (items.at(currIndex).period!=0) //����� ������ ��������
	{
		//Timestamp periodTime;  //������� �����
		DateTime pTime; //����� ��� ���������
		//DateTime fTime; // ����� �����
		Timespan diffTime(items.at(currIndex).period-1,23,0,0,0);  //������� ����� ������
		pTime-=diffTime; 
		//periodTime=periodTime-diffTime; // 

		// ���� �������� ����� ������� ���� ������ period - 1 ���
		DateTime fTime(pFile.created());
		if (fTime<=pTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (items.at(currIndex).limitSize!=0)
	{
		File::FileSize fsize;
		fsize=pFile.getSize();
		if (fsize > items.at(currIndex).limitSize)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
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
				tmpItem.period=pConf->getInt(KeyName,0);
				
				// ������

				items.push_back(tmpItem);
				//cout<< *it<<endl;
			 
			}
 }
}
//! �������������� ������� � int64
long int LogRotator::convertSize(std::string &strSize)
{
	Int64 iSize;
	// ����������� ������ � �����
	
	if (NumberParser::tryParse64(strSize,iSize))
	{
		return iSize;
	}
	else
	{
		return 0;
	}
}
//! �������������� ���������� ������� � int � ����������� ���� �������
/*
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
*/

