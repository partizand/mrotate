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
		Path sourceDir(items[i].source);
		sourceDir.makeFile(); // 
		fileMask=sourceDir.getFileName(); // �����
		sourceDir.setFileName(""); // ������� ��������

		Path destDir(items.at(currIndex).targetDir);
		destDir.makeDirectory(); // ������� ��������
		
		suc=archiver.isValid(items[i].archiverName);
		if (!suc) continue; // ������ � ����������
		poco_information_f1(*log,"Start rotate entry %s.",items[i].source);
		if (items.at(i).period>0 || items.at(i).limitSize>0) // ������� ������
		{
		// �������� ������ ������ ��� ���������
			//Path pathMask(items[i].source);
			//fileList.clear();
			//getFileList(fileList,pathMask,items[i].recurse,items[i].period,items[i].limitSize);
			// �������� �����
			rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,true,items[i].period,items[i].limitSize);
			//createDir(items.at(i).targetDir);
		// �������� �����
			//rotateFile(fileList);
		}
		// ������� ������ ������
		if (items[i].keepPeriod>0)
		{
			//Path pPath(items.at(currIndex).targetDir);
			//pPath.makeDirectory();
			fileMask="*"+archiver.getExtension(items[i].archiverName);
			//pPath.setFileName("*"+archiver.getArhExtention()); // ������ �� ���������� ������
			//oldArhMask=pPath.toString();
			//fileList.clear();
			//getFileList(fileList,pPath,items[i].recurse,items[i].keepPeriod); // ������ ������ ��� ��������
			//removeFile(fileList);
			rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,false,items[i].keepPeriod,0);
		}

	}
}
//------------------------------------------------------------------------
//! ���������� �����/������� ������
void LogRotator::rotateFiles(const std::string &fileMask,const Poco::Path &pSourceDir,const Poco::Path pDestDir,bool recurse,bool rotate,int Period,unsigned long int lSize)
{
		// ������������ ����� � ��������
	//string fileMask=pSourceMask.getFileName();
	Glob glob(fileMask);
		
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
			if (recurse && it->isDirectory()) // ��� �������
			{
				// �� ���� ����� ����� �������� �� . ..
				Path newSource(it.path());
				newSource.setFileName(fileMask);
				Path newDest(pDestDir);
				newDest.pushDirectory(it.name());
				rotateFiles(fileMask,newSource,newDest,recurse,rotate,Period,lSize);
			}
			
			++it;
		}

	/*
	if (recurse) // ���������� ������������ �����������
	{
		
		DirectoryIterator it(pSourceDir);
		//DirectoryIterator end;
		while (it != end)
		{
			if (it->isDirectory()) // ��� �������
			{
				// �� ���� ����� ����� �������� �� . ..
				Path newSource(it.path());
				newSource.setFileName(fileMask);
				Path newDest(pDestDir);
				newDest.pushDirectory(it.name());
				rotateFiles(fileMask,newSource,newDest,recurse,rotate,Period,lSize);
			}
			++it;
		}
	}
	*/
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
	ArhFileName=ReplVar::replaceFileAndDate(ArhFileName,pFile.path());

	Path destFile(destDir);
	destFile.setFileName(ArhFileName);

	bool suc=archiver.archiveFile(items[currIndex].archiverName,fileName,destFile.toString());
	if (suc) // ������� ���������������
	{
		File tFile(pFile);
		removeFile(tFile);
	}

	
}
//------------------------------------------------------------------------
//! ������� ����
void LogRotator::removeFile(Poco::File &pFile)
{
	
if (_debugMode)
{
	
	poco_information_f1(*log,"[Debug] Deleting file %s",pFile.path());
	return;
}
try
{
pFile.remove(); // ������� ���
poco_information_f1(*log,"File deleted %s",pFile.path());
}
catch(...) 
{
	poco_error_f1(*log,"Error delete file %s",pFile.path());
}
}


//------------------------------------------------------------------------
//! �������� ������ ������ �� �����, ���������� �� ������� ��� �������, ���� period=0 � lSize=0 ������������ ��� ����� �� �����
/*
void LogRotator::getFileList(std::vector<std::string> &fileList,const Poco::Path &pathMask,bool recurse, int Period,unsigned long int lSize)
{
	// �������� ������ ������ �������� ��������
	set<string> files;
	//fileList.clear();
	
	Glob::glob(pathMask,files); // ��������� ������� ��������� ������ ������ �� �����

// ���������� ��� �����
	set<string>::iterator it=files.begin();
	for (;it!=files.end();++it)
	{
		if (isRotateFile(*it,Period,lSize)) // ��������� ���� ���� ��������
		{
			fileList.push_back(*it);
		}
	}
	if (recurse) // ���������� ������������ �����������
	{
		Path pPath(pathMask);
		pPath.setFileName("");
		DirectoryIterator it(pPath);
		DirectoryIterator end;
		while (it != end)
		{
			if (it->isDirectory())
			{
				Path p(it.path());
				p.setFileName(pathMask.getFileName());
				getFileList(fileList,p,recurse,Period,lSize);
			}
			++it;
		}
	}
}
*/
//------------------------------------------------------------------------
//! ������� �������, ���� �� �� ����������
/*
void LogRotator::createDir(const std::string &dirName)
{
	Path pPath(dirName);
	pPath.makeDirectory();
	File pFile(pPath);
	pFile.createDirectories();
}
*/
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
/*
void LogRotator::rotateFile(const std::vector<std::string> &listFiles)
{
	int i;
	for (i=0;i<listFiles.size();++i)
	{
		rotateFile(listFiles[i]);
	}
	
}
*/
//------------------------------------------------------------------------
//! ���������� �������� ����
/*
void LogRotator::rotateFile(const std::string &fileName)
{
	File pFile(fileName);
	rotateFile(pFile);
	// ���������� ����
	bool suc=archiver.archiveFile(fileName);
	if (suc) // ������� ���������������
	{
		removeFile(fileName);
	}
}
*/
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
string Source,ArchiverName,TargetDir,TargetMask,FDateMode;
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
				// �������������
				KeyName=RootKeys.at(i)+".recurse";
				Recurse=pConf->getBool(KeyName,false);
				//RotateEntry tmpItem(Source,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask);


				//items.push_back(tmpItem);
				items.push_back(RotateEntry(RootKeys.at(i),Source,Recurse,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask,FDateMode));
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

