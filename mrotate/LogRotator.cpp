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
#include "Executer.h"

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
	vector<string> vectArgs;
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
		// ������ ����� ��������
		if (!items[i].preRotate.empty())
		{
			poco_information_f1(*log,"Launch process %s.",items[i].preRotate);
			Executer::execute(items[i].preRotate,vectArgs);
		}
		if (items.at(i).period>0 || items.at(i).limitSize>0) // ������� ������
		{
		
			// �������� �����
			rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,true,items[i].period,items[i].limitSize);
			
		}
		// ������� ������ ������ (���� �� �����)
		if (!items[i].shift && items[i].keepPeriod>0)
		{
			
			fileMask=getRemoveMask(); //"*"+archiver.getExtension(items[i].archiverName);
			rotateFiles(fileMask,destDir,destDir,items[i].recurse,false,items[i].keepPeriod,0);
		}
		// ������ ����� �������
		if (!items[i].postRotate.empty())
		{
			poco_information_f1(*log,"Launch process %s.",items[i].postRotate);
			Executer::execute(items[i].postRotate,vectArgs);
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
	//string ArhFileName(items[currIndex].targetMask); // �������� ��� ������
	
	
	DateTime replDate=getDate(pFile,items[currIndex].dateReplaceMode); // ����/����� �� ������
	// �������� ��� ������
	string ArhFileName=ReplVar::replaceFileAndDate(items[currIndex].targetMask,fileName,"",replDate,1);

	Path destFile(destDir);
	destFile.setFileName(ArhFileName);
	string strDestFile=destFile.toString();

	if (items[currIndex].shift) // �������� ������ �����
		{
			shiftFile(fileName,destDir.toString()); 
		}

	bool suc=archiver.archiveFile(items[currIndex].archiverName,fileName,strDestFile,replDate);
	if (suc) // ������� ���������������
	{
		removeFile(pFile);
	}

	
}
//------------------------------------------------------------------------
//! �������� ������ ����� ��������� �����
// pFile - �������� ����, destDir - ������� ��� ����� ������ �����
// ������� ������ ������, ����� �������� n->n+1,..1->2
void LogRotator::shiftFile(const std::string &srcFile,const std::string &destDir)
{
	// �������� ����� ������
	
	//string fullFileName=pFile.path();
	string fileMask=ReplVar::replaceFileAndDate(items[currIndex].targetMask,srcFile,"",0,-1);
	fileMask+=archiver.getExtension(items[currIndex].archiverName);
	// ������ ������ ������
	
	Glob glob(fileMask,Glob::GLOB_CASELESS); // ������� �� ����� � �����
		map<int,string> fileList; // ������ ������ � ���������
		int ind;
		DirectoryIterator it(destDir);
		DirectoryIterator end;
		while (it != end)
		{
			if (it->isFile()) // ��� ����
			{
				if (glob.match(it.name())) // �������� �� �����
				{
					// ��������� � ������
					ind=getIndexOfFile(it.name());
					if (ind>=0)
					{
						fileList[ind]=it->path();
					}

				}
				
			}
			
			
			++it;
		}

	// �������� �� ������ � �������� ����������� (�� �������� ��������)
	Path pDest(destDir);
	pDest.makeDirectory();

	map<int,string>::reverse_iterator rit;
	for (rit=fileList.rbegin();rit!=fileList.rend();++rit)
	{
		// ������� ������
		if (items[currIndex].keepPeriod>0)
		{
		if (rit->first > items[currIndex].keepPeriod) // ������ ����� ������
		{
			removeFile(rit->second);
			continue;
		}
		}
		// �������� ���������
		
		string newFileName=ReplVar::replaceFile(items[currIndex].targetMask,srcFile,"",rit->first+1);
		pDest.setFileName(newFileName); // ����� ��� ����� � ����� ��������
		Executer::moveFile(srcFile,pDest.toString(),_debugMode,*log);

	}

	//string fileMask=Re
}
//------------------------------------------------------------------------
//! ��������� ������ ����� (�� ����������� test.log.2.7z -> ������ 2, ���� targetMask=%FileName.%Index)
// ���� ������ �� ���������� ������ -1
int LogRotator::getIndexOfFile(const std::string &fileName)
{
	string fName(fileName);
	int ind=-1;
	// ������ ���������� ����������
	string ext=archiver.getExtension(items[currIndex].archiverName);
	if (!ext.empty())
	{
		string to="";
		replaceInPlace(fName,ext,to); // ����� �������, ����� ���������� �� ����� ��������
	}
	// fName ������ - ��� ����� ��� ���������� ����������
	string strFind=ReplVar::replaceFile(items[currIndex].targetMask,fName,"",-2);
	// strFind- ��� �����, ��� ������ �������, �������� %Index
	string::size_type i=strFind.find("%Index"); // ������� ������
	if (i==string::npos || i+zeroPad>fName.length()) return ind; // ����� �� �����
	string strNum=string(fName,i,zeroPad); // ������ ������

	NumberParser::tryParse(strNum,ind); // �������� ������������

	//Path pFileName(fName);
	//pFileName.makeFile();
	//ext=pFileName.getExtension(); // ���������� ��� �����, ���� 2
	
	//NumberParser::tryParse(ext,ind);
	
	return ind;


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
string Source,ArchiverName,TargetDir,TargetMask,FDateMode,DateReplaceMode,PreRotate,PostRotate;
int Period,KeepPeriod;
bool Recurse,Shift;
unsigned long int LimitSize;

AbstractConfiguration::Keys RootKeys;
pConf-> keys("",RootKeys); // ������ �������� ������
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	//RotateEntry re;
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
				KeyName=RootKeys.at(i)+".Source";
				Source=pConf->getString(KeyName,"");
				if (Source.empty()) 
				{
					poco_error_f1(*log,"Skip entry %s. Source missing",RootKeys.at(i));
					continue;
				}
				//������
				KeyName=RootKeys.at(i)+".Period";
				Period=pConf->getInt(KeyName,0);
				
				// ������
				KeyName=RootKeys.at(i)+".Size";
				KeyValue=pConf->getString(KeyName,"");
				LimitSize=convertSize(KeyValue);
				
				// Keep ������
				KeyName=RootKeys.at(i)+".KeepPeriod";
				KeepPeriod=pConf->getInt(KeyName,0);
				
				if (LimitSize==0 && Period==0 && KeepPeriod==0) 
				{
					poco_error_f1(*log,"Skip entry %s. Period, KeepPeriod and size is null",RootKeys.at(i));
					continue;
				}
				// ���������
				KeyName=RootKeys.at(i)+".Compress";
				ArchiverName=pConf->getString(KeyName,"");
				toLowerInPlace(ArchiverName);
				if (ArchiverName.empty()) 
				{
					poco_error_f1(*log,"Skip entry %s. Archiver is empty",RootKeys.at(i));
					continue;
				}
				// Target Dir
				KeyName=RootKeys.at(i)+".TargetDir";
				TargetDir=pConf->getString(KeyName,"");
				// Target Mask
				KeyName=RootKeys.at(i)+".TargetMask";
				TargetMask=pConf->getString(KeyName,"");
				// Date mode
				KeyName=RootKeys.at(i)+".DateMode";
				FDateMode=pConf->getString(KeyName,"");
				// Date Replace Mode
				KeyName=RootKeys.at(i)+".DateReplace";
				DateReplaceMode=pConf->getString(KeyName,"");
				// �������������
				KeyName=RootKeys.at(i)+".Recurse";
				Recurse=pConf->getBool(KeyName,false);
				//RotateEntry tmpItem(Source,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask);
				// ������ ����� ��������
				KeyName=RootKeys.at(i)+".PreRotate";
				PreRotate=pConf->getString(KeyName,"");
				// ������ ����� �������
				KeyName=RootKeys.at(i)+".PostRotate";
				PostRotate=pConf->getString(KeyName,"");
				// ����� ������
				KeyName=RootKeys.at(i)+".Shift";
				Shift=pConf->getBool(KeyName,false);

				//items.push_back(tmpItem);
				items.push_back(RotateEntry(RootKeys.at(i),Source,Recurse,Period,LimitSize,ArchiverName,Shift,KeepPeriod,TargetDir,TargetMask,FDateMode,DateReplaceMode,PreRotate,PostRotate));
				//cout<< *it<<endl;
			 
			}
 }
}

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
