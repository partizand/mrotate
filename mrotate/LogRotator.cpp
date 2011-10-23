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

const string ver="0.1"; // ������ ��������

LogRotator::LogRotator(Poco::Logger &logger):
	archiver(logger)
{
	log=&logger;
}


LogRotator::~LogRotator(void)
{
}
//------------------------------------------------------------------------
//! ���������� ����� (�������� �������)
void LogRotator::rotate()
{
	int i;
	bool suc;
	string oldArhMask;
	vector<string> fileList;
	// ���������� ��� ������
	for (i=0;i<items.size();++i)
	{
		currIndex=i;
		suc=archiver.setOptions(items[i].archiverName,items[i].targetPath);
		if (!suc) continue; // ������ � ����������
		poco_information_f1(*log,"Start rotate entry %s.",items[i].source);
		// �������� ������ ������ ��� ���������
		getFileList(fileList,items.at(currIndex).source);
		// �������� �����
		rotateFile(fileList);
		// ������� ������ ������
		if (items[i].keepPeriod>0)
		{
			Path pPath(items.at(currIndex).targetDir);
			pPath.setFileName("*"+archiver.getArhExtention()); // ������ �� ���������� ������
			oldArhMask=pPath.toString();
			getFileList(fileList,oldArhMask,items[i].keepPeriod); // ������ ������ ��� ��������
			removeFile(fileList); 
		}

	}
}
//------------------------------------------------------------------------
//! �������� ������ ������ �� �����, ���������� �� ������� ��� �������, ���� period � lSize �� ������, ������� �� ��������� ������� �������
void LogRotator::getFileList(std::vector<std::string> &fileList, const std::string &pathMask,int Period,unsigned long int lSize)
{
set<string> files;
fileList.clear();
Glob::glob(pathMask,files); // ��������� ������� ��������� ������ ������ �� �����
/*
if (Period==0 && lSize==0) 
{
	fileList.assign(files.begin(),files.end());
	return;
}
*/
// ���������� ��� �����
	set<string>::iterator it=files.begin();
	for (;it!=files.end();++it)
	{
		if (isRotateFile(*it,Period,lSize)) // ��������� ���� ���� ��������
		{
			fileList.push_back(*it);
		}
	}
}
//------------------------------------------------------------------------
//! �������� ������ ������ ��� ���������
/*
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
*/
//------------------------------------------------------------------------
//! ���������� ������ ������
void LogRotator::rotateFile(const std::vector<std::string> &listFiles)
{
	int i;
	for (i=0;i<listFiles.size();++i)
	{
		rotateFile(listFiles[i]);
	}
	
}
//------------------------------------------------------------------------
//! ���������� �������� ����
void LogRotator::rotateFile(const std::string &fileName)
{
	// ���������� ����
	bool suc=archiver.archiveFile(fileName);
	if (suc) // ������� ���������������
	{
		removeFile(fileName);
	}
}
//------------------------------------------------------------------------
//! ������� ������ ������
void LogRotator::removeFile(const std::vector<std::string> &listFiles)
{
int i;
	for (i=0;i<listFiles.size();++i)
	{
		removeFile(listFiles[i]);
	}
}
//------------------------------------------------------------------------
//! ������� ����
void LogRotator::removeFile(const std::string &fileName)
{
Poco::File pFile(fileName);
try
{
pFile.remove(); // ������� ���
}
catch(...) 
{
	log->error("������ �������� �����");
}
}
//------------------------------------------------------------------------
//! ��������� ����� �� ���������� ������ ����, ���� period � lSize �� ������, ������� �� ��������� ������� �������
bool LogRotator::isRotateFile(const std::string &fileName,int Period/*=0*/,unsigned long int lSize/*=0*/)
{
Poco::File pFile(fileName);

	if (!pFile.exists()) return false; // ����� ���
	if (!pFile.isFile()) return false; // ��� �� ����
	if (!pFile.canRead()) return false; // ���� �� ����� ���� ��������

	int iPeriod(Period);
	unsigned long int iSize(lSize);
	if (Period==0 && lSize==0) // ��������� �� ������, ����� �� ������� ��������� �������
	{
		iPeriod=items.at(currIndex).period;
		iSize=items.at(currIndex).limitSize;
	}
	if (iPeriod==0 && iSize==0) return false; // ��� ����� �� ������
	if (iPeriod>0) //����� ������ ��������
	{
		//Timestamp periodTime;  //������� �����
		DateTime pTime; //����� ��� ���������
		//DateTime fTime; // ����� �����
		Timespan diffTime(iPeriod-1,23,0,0,0);  //������� ����� ������
		pTime-=diffTime; 
		//periodTime=periodTime-diffTime; // 
		Timestamp fileTime;
		switch (items.at(currIndex).dateMode)
		{
		case Rotate::Created: 
			fileTime=pFile.created();
			break;
		case Rotate::Modified: 
			fileTime=pFile.getLastModified();
			break;
		}
		// ���� ����� ������� ���� ������ period - 1 ���
		//DateTime fTime(pFile.created());
		// ���� �����������
		DateTime fTime(fileTime);
		if (fTime<=pTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (iSize>0)
	{
		File::FileSize fsize;
		fsize=pFile.getSize();
		if (fsize > iSize)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
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
//RotateEntry tmpItem;
string Source,ArchiverName,TargetDir,TargetMask,FDateMode;
int Period,KeepPeriod;
unsigned long int LimitSize;

AbstractConfiguration::Keys RootKeys;
pConf-> keys("",RootKeys); // ������ �������� ������
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	int i;
	for (i=0;i<RootKeys.size();++i)
	
	 //for (AbstractConfiguration::Keys::const_iterator it = RootKeys.begin(); it != RootKeys.end(); ++it)
			{
				// ������� �����
				//KeyName=RootKeys.at(i);
				if (icompare(RootKeys.at(i),"application")==0) continue;
				if (icompare(RootKeys.at(i),"system")==0) continue;
				if (icompare(RootKeys.at(i),"logging")==0) continue;
				
				poco_debug_f1(*log,"Loading entry %s.",RootKeys.at(i));

				//KeyName=*it+".source";
				// ��������
				KeyName=RootKeys.at(i)+".source";
				Source=pConf->getString(KeyName,"");
				if (Source.empty()) 
				{
					poco_error_f1(*log,"Skip entry %s. Source missing",RootKeys.at(i));
					continue;
				}
				//������
				KeyName=RootKeys.at(i)+".period";
				Period=pConf->getInt(KeyName,0);
				
				// ������
				KeyName=RootKeys.at(i)+".size";
				KeyValue=pConf->getString(KeyName,"");
				LimitSize=convertSize(KeyValue);
				if (LimitSize==0 && Period==0) 
				{
					poco_error_f1(*log,"Skip entry %s. Period and size is null",RootKeys.at(i));
					continue;
				}
				// Keep ������
				KeyName=RootKeys.at(i)+".keepPeriod";
				KeepPeriod=pConf->getInt(KeyName,0);
				// ���������
				KeyName=RootKeys.at(i)+".compress";
				ArchiverName=pConf->getString(KeyName,"");
				if (ArchiverName.empty()) 
				{
					poco_error_f1(*log,"Skip entry %s. Archiver is empty",RootKeys.at(i));
					continue;
				}
				// Target Dir
				KeyName=RootKeys.at(i)+".targetDir";
				TargetDir=pConf->getString(KeyName,"");
				// Target Mask
				KeyName=RootKeys.at(i)+".targetMask";
				TargetMask=pConf->getString(KeyName,"");
				// Date mode
				KeyName=RootKeys.at(i)+".dateMode";
				FDateMode=pConf->getString(KeyName,"");

				//RotateEntry tmpItem(Source,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask);


				//items.push_back(tmpItem);
				items.push_back(RotateEntry(Source,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask,FDateMode));
				//cout<< *it<<endl;
			 
			}
 }
}
//------------------------------------------------------------------------
//! ������������� target � ������� ����
/*
std::string LogRotator::getFullTarget(const std::string &targetPath,const std::string &Source)
{
	string shortFileName; // �������� ��� ����� target
	Path sPath(Source);
	sPath.makeFile();
	
	sPath.setFileName("");
	Path tPath(targetPath);
	shortFileName=tPath.getFileName();
	if (shortFileName.empty()) shortFileName="%FileName";
	Path rPath; // �������������� 
	if (tPath.depth()==0) 
	{tPath.assign(

	}

}
*/
//------------------------------------------------------------------------
//! �������������� ������� � int64 (�.�. ������ ����� ��������� K � M)
unsigned long int LogRotator::convertSize(std::string &strSize)
{
	if (strSize.empty()) return 0;
	Int64 iSize;
	Int64 multipl=1;
	// ����������� ������ � �����
	toUpperInPlace(strSize);
	string::size_type iK=strSize.find("K");
	string::size_type iM=strSize.find("M");
	if (iK!=string::npos) // ������ K
	{
		multipl=1024;
		strSize.erase(iK,1);
	}
	if (iM!=string::npos) // ������ M
	{
		multipl=1024*1024;
		strSize.erase(iM,1);
	}
	if (NumberParser::tryParse64(strSize,iSize))
	{
		return iSize*multipl;
	}
	else
	{
		log->error("Error parsing size in entry");
		return 0;
	}
}
//! ���������� ������
std::string LogRotator::getVersion()
{
return ver;
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

