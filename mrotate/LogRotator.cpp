#include "StdAfx.h"
#include "LogRotator.h"


#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>
//#include <Poco\Path.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco\DirectoryIterator.h>
#include <Poco\NumberParser.h>
#include <Poco\Glob.h>
#include <Poco\Timestamp.h>
#include <Poco\DateTime.h>

#include "ReplVar.h"

using namespace std;
using namespace Poco;
using namespace Util;

const string ver="0.1"; // ������ ��������

LogRotator::LogRotator(Poco::Logger &logger):
	archiver(logger),
	_debugMode(false)
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
	string fileMask;
	//string oldArhMask;
	vector<string> fileList;
	// ���������� ��� ������
	for (i=0;i<items.size();++i)
	{
		currIndex=i;
		Path sourceDir(items[i].sourceDir);
		sourceDir.makeDirectory(); // 
		fileMask=items[i].sourceMask;//  sourceDir.getFileName(); // �����
		//sourceDir.setFileName(""); // ������� ��������
		//sourceDir.makeDirectory();

		Path destDir(items.at(currIndex).targetDir);
		destDir.makeDirectory(); // ������� ��������
		
		suc=archiver.isValid(items[i].archiverName);
		if (!suc) continue; // ������ � ����������
		poco_information_f1(*log,"Start rotate entry %s.",items[i].source);
		if (items.at(i).period>0 || items.at(i).limitSize>0) // ������� ������
		{
		
			// �������� �����
			rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,true,items[i].period,items[i].limitSize);
			
		}
		// ������� ������ ������
		if (items[i].keepPeriod>0)
		{
			
			fileMask=getRemoveMask(); //"*"+archiver.getExtension(items[i].archiverName);
			rotateFiles(fileMask,destDir,destDir,items[i].recurse,false,items[i].keepPeriod,0);
		}

	}
}
//------------------------------------------------------------------------
//! ���������� �����/������� ������
void LogRotator::rotateFiles(const std::string &fileMask,const Poco::Path &pSourceDir,const Poco::Path pDestDir,bool recurse,bool rotate,int Period,unsigned long int lSize)
{
		// ������������ ����� � ��������
	//string fileMask=pSourceMask.getFileName();
	Glob glob(fileMask,Glob::GLOB_CASELESS); // ������� �� ����� � �����
		
		DirectoryIterator it(pSourceDir);
		DirectoryIterator end;
		while (it != end)
		{
			if (it->isFile()) // ��� ����
			{
				if (glob.match(it.name())) // �������� �� �����
				{
					if (isRotateFile(*it,Period,lSize)) // �������� �� ����/�������
					{
						if (rotate)
							rotateFile(*it,pDestDir);
						else
							removeFile(*it);
					}

				}
				
			}
			else if (recurse && it->isDirectory()) // ��� �������
			{
				
				
				//string itname=it.name();
				Path newSource(it.path());
				newSource.makeDirectory();
				
				Path newDest(pDestDir);
				newDest.pushDirectory(it.name());
				rotateFiles(fileMask,newSource,newDest,recurse,rotate,Period,lSize);
			}
			
			++it;
		}

	
}
//------------------------------------------------------------------------
//! ���������� �������� ����
void LogRotator::rotateFile(const Poco::File &pFile,const Poco::Path &destDir)
{
// ����� ��������� �����, ����� ��������� � ���������

	

	//Path pPathSource(FileName);
	//pPathSource.makeFile();
	string fileName=pFile.path();
	//string sFileName(pFile.getFileName()); // �������� ��� ����� ���������
	//������ � targetPath - %FileName � %yydd - �� ��� ����
	string ArhFileName(items[currIndex].targetMask); // �������� ��� ������
	
	
	DateTime replDate=getDate(pFile,items[currIndex].dateReplaceMode); // ����/����� �� ������

	ArhFileName=ReplVar::replaceFileAndDate(ArhFileName,pFile.path(),"",replDate);

	Path destFile(destDir);
	destFile.setFileName(ArhFileName);

	bool suc=archiver.archiveFile(items[currIndex].archiverName,fileName,destFile.toString(),replDate);
	if (suc) // ������� ���������������
	{
		//File tFile(pFile);
		removeFile(pFile);
	}

	
}

//------------------------------------------------------------------------
//! ��������� ����� �� ���������� ������ ����, ���� period � lSize �� ������, ���� ����� ����������
bool LogRotator::isRotateFile(Poco::File &pFile,int Period,unsigned long int lSize)
{
if (!pFile.exists()) return false; // ����� ���
	if (!pFile.isFile()) return false; // ��� �� ����
	if (!pFile.canRead()) return false; // ���� �� ����� ���� ��������

	int iPeriod(Period);
	unsigned long int iSize(lSize);
	if (Period==0 && lSize==0) // ��������� �� ������, ����� �� ������� ��������� �������
	{
		return true;
	}
	//if (iPeriod==0 && iSize==0) return false; // ��� ����� �� ������
	if (iPeriod>0) //����� ������ ��������
	{
		//Timestamp periodTime;  //������� �����
		DateTime pTime; //����� ��� ���������
		//DateTime fTime; // ����� �����
		Timespan diffTime(iPeriod-1,23,0,0,0);  //������� ����� ������
		pTime-=diffTime; // ���� � ������� �������
		//periodTime=periodTime-diffTime; // 
		
		// ���� ����� ������� ���� ������ period - 1 ���
		//DateTime fTime(pFile.created());
		// ���� �����������/��������
		DateTime fTime=getDate(pFile,items[currIndex].dateMode); // ���� ������� ����� �������
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
//! ���������� ����� ��� ��������� ������ ��� �������� �������
std::string LogRotator::getRemoveMask()
{
string retMask;
retMask=ReplVar::replaceFileAndDate(items[currIndex].targetMask,items[currIndex].sourceMask,"",0);
retMask+=archiver.getExtension(items[currIndex].archiverName);
return retMask;
}

//------------------------------------------------------------------------
//! ������� ����
void LogRotator::removeFile(const Poco::File &pFile)
{
	
if (_debugMode)
{
	
	poco_information_f1(*log,"[Debug] Deleting file %s",pFile.path());
	return;
}
try
{
	File tFile(pFile);
	tFile.remove(); // ������� ���
poco_information_f1(*log,"File deleted %s",pFile.path());
}
catch(...) 
{
	poco_error_f1(*log,"Error delete file %s",pFile.path());
}
}
//------------------------------------------------------------------------
//! ���������� ���� � ����������� �� ������ dateMode
// ����� ���� ����������� ��� �� ������ ������
Poco::DateTime LogRotator::getDate(const Poco::File &pFile,Rotate::DateMode dateMode)
{
Timestamp fileTime;
	switch (dateMode)
	{
	case Rotate::Created:
		fileTime=pFile.created();
		break;
	case Rotate::Modified:
		fileTime=pFile.getLastModified();
		break;
	case Rotate::Now:
		fileTime.update();
		break;
	}
	DateTime replDate(fileTime); // ����/����� �� ������
	return replDate;
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
removeFile(pFile);
}

//------------------------------------------------------------------------
//! ��������� ����� �� ���������� ������ ����, ���� period � lSize �� ������, ���� ����� ����������
bool LogRotator::isRotateFile(const std::string &fileName,int Period/*=0*/,unsigned long int lSize/*=0*/)
{
Poco::File pFile(fileName);
return isRotateFile(pFile,Period,lSize);
	
}
//------------------------------------------------------------------------
//! ���������� ����� ������� (�������� �������)
void LogRotator::setDebugMode()
{
_debugMode=true;
archiver.setDebugMode();
}
//------------------------------------------------------------------------
//! �������� ����������� ������� ������� �� ������
bool LogRotator::check()
{
int i;
	bool suc,ret(true);
	string oldArhMask;
	vector<string> fileList;
	// ���������� ��� ������
	for (i=0;i<items.size();++i)
	{
		currIndex=i;
		poco_information_f1(*log,"Checking entry %s.",items[i].name);
		// �������� ����������
		suc=archiver.isValid(items[i].archiverName);
		if (!suc) 
		{
			ret=false;
			log->information("Entry contains errors");
			continue; // ������ � ����������
		}
		// �������� ������������� ���������

		log->information("Entry is ok");
	}
	if (ret)
	{
		log->information("Everything is ok");
	}
	else
	{
		log->information("Errors found");
	}
	return ret;
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
	poco_information_f1(*log,"Loading config file %s.",fileName);
	load(pConf);
}
//------------------------------------------------------------------------
//! �������� �������� �������
void LogRotator::load(const Poco::Util::AbstractConfiguration *pConf)
{
//RotateEntry tmpItem;
string Source,ArchiverName,TargetDir,TargetMask,FDateMode,DateReplaceMode;
int Period,KeepPeriod;
bool Recurse;
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
				
				// Keep ������
				KeyName=RootKeys.at(i)+".keepPeriod";
				KeepPeriod=pConf->getInt(KeyName,0);
				
				if (LimitSize==0 && Period==0 && KeepPeriod==0) 
				{
					poco_error_f1(*log,"Skip entry %s. Period, KeepPeriod and size is null",RootKeys.at(i));
					continue;
				}
				// ���������
				KeyName=RootKeys.at(i)+".compress";
				ArchiverName=pConf->getString(KeyName,"");
				toLowerInPlace(ArchiverName);
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
				// Date Replace Mode
				KeyName=RootKeys.at(i)+".dateReplace";
				DateReplaceMode=pConf->getString(KeyName,"");
				// �������������
				KeyName=RootKeys.at(i)+".recurse";
				Recurse=pConf->getBool(KeyName,false);
				//RotateEntry tmpItem(Source,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask);


				//items.push_back(tmpItem);
				items.push_back(RotateEntry(RootKeys.at(i),Source,Recurse,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask,FDateMode,DateReplaceMode));
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

