/*
	This file is part of mrotate. Rotate logs for Windows

    Copyright (C) 2011  Andrey Kapustin

    mrotate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    mrotate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	��� ���������������� �� �������� GPL 3
	�����: �������� ������, 2011 �.

*/

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
#include <Poco\LocalDateTime.h>

#include "ReplVar.h"
#include "Executer.h"

using namespace std;
using namespace Poco;
using namespace Util;

const string ver="0.2 beta"; // ������ ��������

LogRotator::LogRotator(Poco::Logger &logger):
	archiver(logger),
	_debugMode(false),
	_force(false),
	rstatus(logger)
{
	log=&logger;
}

//------------------------------------------------------------------------
LogRotator::~LogRotator(void)
{
}
//! ������������� ��� ����� �������� �������
void LogRotator::setStatusFileName(const std::string &fileName)
{
	rstatus.setStatusFileName(fileName);
}
//------------------------------------------------------------------------
//! ���������� ����� (�������� �������)
void LogRotator::rotate()
{
	int i;
	bool suc;
	string fileMask;
	//string oldArhMask;
	//vector<string> vectArgs;
	rstatus.load();
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
		executeScript(items[i].preRotate);
		

		if (items[i].shift) // ������� � ������ ������
		{
			if (_force) 
			{
				rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,true,0,0);
			}
			else
			{
			
				// �������� ������� ���� �� ���������� ���� �� �������
				if (items[i].period>0) // ������� �� ����������
				{
					// ����� ����� ������������� ����� �� �������, ���� �����, �� ��� ���� ������
					if (isNeedRotate())
					{
					rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,true,0,0);
				
					}
				}
				else if (items[i].limitSize>0) // ������� �� �������
				{
					// �������� �����
					rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,true,0,items[i].limitSize);
				}
			}


		}
		else // ������� � ������ Win
		{
			if (items.at(i).period>0 || items.at(i).limitSize>0) // ������� ������
			{
		
			// �������� �����
			rotateFiles(fileMask,sourceDir,destDir,items[i].recurse,true,items[i].period,items[i].limitSize);
			
			}
			// ������� ������ ������ (���� �� �����)
			if (items[i].keepPeriod>0)
			{
			
			fileMask=getRemoveMask(); //"*"+archiver.getExtension(items[i].archiverName);
			rotateFiles(fileMask,destDir,destDir,items[i].recurse,false,items[i].keepPeriod,0);
			}
		}

		
		
		
		
		// ������ ����� �������
		executeScript(items[i].postRotate);
		/*
		if (!items[i].postRotate.empty())
		{
			if (!_debugMode)
			{
			//poco_information_f1(*log,"Launch process %s.",items[i].preRotate);
			ret=Executer::execute(items[i].postRotate,vectArgs,*log);
			poco_information_f2(*log,"Executed %s. Exit code %i",items[i].preRotate,ret);
			}
			else
			{
			poco_information_f1(*log,"[Debug] Launch process %s.",items[i].postRotate);
			}
			
			
		}
		*/
	}
}
//------------------------------------------------------------------------
//! ��������� ������ ��/����� �������
void LogRotator::executeScript(const std::string &script)
{

		if (!script.empty())
		{
			if (!_debugMode)
			{
			vector<string> vectArgs;
			//poco_information_f1(*log,"Launch process %s.",items[i].preRotate);
			int ret=Executer::execute(script,vectArgs,*log);
			poco_information_f2(*log,"Executed %s. Exit code %i",script,ret);
			}
			else
			{
			poco_information_f1(*log,"[Debug] Launch process %s.",script);
			}
			
			
		}
}
//------------------------------------------------------------------------
//! ��������� ����� �� ������� ��� ������ shift ��� ������ indx
bool LogRotator::isNeedRotate()
{
	if (!items[currIndex].shift) return false;
bool ret=false;
//Timestamp nowStamp;
DateTime nowDate;//(nowStamp);
//Timestamp lastStamp=rstatus.getDate(items[indx].confName,items[indx].name);
DateTime lastDate=rstatus.getDate(items[currIndex].confName,items[currIndex].name);

int nowYear,lastYear,nowMonth,lastMonth,nowDay,lastDay,nowWeek,lastWeek;
	nowYear=nowDate.year();
	nowMonth=nowDate.month();
	lastYear=lastDate.year();
	lastMonth=lastDate.month();
	nowDay=nowDate.day();
	lastDay=lastDate.day();

switch(items[currIndex].period)
{
case Rotate::DAILY:
	if ((nowYear!=lastYear) || (nowMonth!=lastMonth) || (nowDay!=lastDay))
	{
		ret=true;
	}
	break;
case Rotate::WEEKLY:
	nowWeek=nowDate.week();
	lastWeek=lastDate.week();
	if (nowWeek==0) // ����� �������� �� ��������� ������ � ���������� ����
	{
		nowWeek=53;
		--nowYear;
	}
	if (lastWeek==0) // ����� �������� �� ��������� ������ � ���������� ����
	{
		lastWeek=53;
		--lastYear;
	}
	if ((nowYear!=lastYear) || (nowWeek!=lastWeek))
	{
		ret=true;
	}
	break;
case Rotate::MONTHLY:
	if ((nowYear!=lastYear) || (nowMonth!=lastMonth))
	{
		ret=true;
	}
	break;
default:
		Timespan diffTime(items[currIndex].period-1,23,0,0,0);  //������� ����� ������
		nowDate-=diffTime; // ���� � ������� �������
		if (lastDate<nowDate)
		{
			ret=true;
		}
		break;
}
return ret;
}

//------------------------------------------------------------------------
//! ���������� �����/������� ������
void LogRotator::rotateFiles(const std::string &fileMask,const Poco::Path &pSourceDir,const Poco::Path pDestDir,bool recurse,bool rotate,int Period,Poco::Int64 lSize)
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
		// ���������� ���� �������, ���� shift
		if (items[currIndex].shift && !_debugMode && items[currIndex].period>0)
		{
			rstatus.setDate(items[currIndex].confName,items[currIndex].name); // ��������� ���� �������
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
					ind=getIndexOfFile(srcFile,it.name());
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
		if (rit->first >= items[currIndex].keepPeriod) // ������ ����� ������
		{
			removeFile(rit->second);
			continue;
		}
		}
		// �������� ���������
		
		string newFileName=ReplVar::replaceFile(items[currIndex].targetMask,srcFile,"",rit->first+1);
		newFileName+=archiver.getExtension(items[currIndex].archiverName);
		pDest.setFileName(newFileName); // ����� ��� ����� � ����� ��������
		Executer::moveFile(rit->second,pDest.toString(),_debugMode,*log);

	}

	//string fileMask=Re
}
//------------------------------------------------------------------------
//! ��������� ������ ����� (�� ����������� test.log.2.7z -> ������ 2, ���� targetMask=%FileName.%Index)
// ���� ������ �� ���������� ������ -1
int LogRotator::getIndexOfFile(const std::string &baseFileName,const std::string &fileName)
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
	string strFind=ReplVar::replaceFile(items[currIndex].targetMask,baseFileName,"",-2);
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
bool LogRotator::isRotateFile(Poco::File &pFile,int Period,Poco::Int64 lSize)
{
if (!pFile.exists()) return false; // ����� ���
	if (!pFile.isFile()) return false; // ��� �� ����
	if (!pFile.canRead()) return false; // ���� �� ����� ���� ��������

	
	if (Period==0 && lSize==0) // ��������� �� ������, ����� ����������
	{
		return true;
	}
	
	int iPeriod(Period);
	Poco::Int64 iSize(lSize);

	if (_force && items[currIndex].shift) return true;
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
Timestamp fmTime;
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
	case Rotate::Last: // ���������� ����
		fileTime=pFile.created();
		fmTime=pFile.getLastModified();
		if (fmTime>fileTime)
		{
			fileTime=fmTime;
		}
		break;
	case Rotate::First: // ���������� ����
		fileTime=pFile.created();
		fmTime=pFile.getLastModified();
		if (fmTime<fileTime)
		{
			fileTime=fmTime;
		}
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
bool LogRotator::isRotateFile(const std::string &fileName,int Period/*=0*/,Poco::Int64 lSize/*=0*/)
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
//! ���������� ����� force
void LogRotator::setForceMode()
{
_force=true;
}
//------------------------------------------------------------------------
//! �������� ����������� ������� ������� �� ������
bool LogRotator::check()
{
int i;
	bool suc,ret(true),entryOk;
	//string oldArhMask;
	string fullExe;
	vector<string> fileList;
	// ���������� ��� ������
	for (i=0;i<items.size();++i)
	{
		//currIndex=i;
		entryOk=true;
		poco_information_f1(*log,"Checking entry %s.",items[i].name);
		// �������� ����������
		entryOk=archiver.isValid(items[i].archiverName);
		if (!entryOk) 
		{
			ret=false;
			//log->information("Entry contains errors");
			//continue; // ������ � ����������
		}
		// �������� ������������� �������� � ������
		File pFile(items[i].sourceDir);
		if (!pFile.exists())
		{
			ret=false;
			entryOk=false;
			poco_error_f1(*log,"Source directory %s not found",items[i].sourceDir);
			//continue; // ������� �� ������
		}
		// ������ ����� �������� ����������
		if (!items[i].preRotate.empty())
		{
			suc=Executer::getFullPath(items[i].preRotate,fullExe);
			if (!suc)
			{
				ret=false;
				entryOk=false;
				poco_error_f1(*log,"Prerotate script %s not found",items[i].preRotate);
			}

		}
		// ������ ����� ������� ����������
		if (!items[i].postRotate.empty())
		{
			suc=Executer::getFullPath(items[i].postRotate,fullExe);
			if (!suc)
			{
				ret=false;
				entryOk=false;
				poco_error_f1(*log,"Postrotate script %s not found",items[i].postRotate);
			}

		}
		// ������� ���� ���-�� ��� �������
		if (items[i].shift)
		{
			if (items[i].limitSize==0 && items[i].period==0)
			{
				ret=false;
				entryOk=false;
				log->error("Nothing TODO. Size and Period are zero");
			}
		}
		else
		{
			if (items[i].limitSize==0 && items[i].period==0 && items[i].keepPeriod==0)
			{
				ret=false;
				entryOk=false;
				log->error("Nothing TODO. Size, Period and Keep are zero");
			}

		}

		if (entryOk)
			log->information("Entry is ok");
		else
			log->information("Entry contains errors");
		
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
	pPath.makeFile();
	Poco::File pFile(pPath);

	if (!pFile.exists()) return; // ����� ���
	if (!pFile.canRead()) return; // ���� �� ����� ���� ��������

	string confName=pPath.getBaseName();

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
	load(pConf,confName);
}
//------------------------------------------------------------------------
//! �������� �������� �������
void LogRotator::load(const Poco::Util::AbstractConfiguration *pConf,const std::string &confName)
{
//RotateEntry *re;
//string Period;
//string Source,ArchiverName,TargetDir,TargetMask,FDateMode,DateReplaceMode,PreRotate,PostRotate;
string Source,ArchiverName,TargetDir,TargetMask;
//int KeepPeriod;
//bool Recurse,Shift;
//unsigned long int LimitSize;

AbstractConfiguration::Keys RootKeys;
pConf-> keys("",RootKeys); // ������ �������� ������
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	//RotateEntry re;
	int i;
	for (i=0;i<RootKeys.size();++i)
	{
				
		if (icompare(RootKeys.at(i),"application")==0) continue;
		if (icompare(RootKeys.at(i),"system")==0) continue;
		if (icompare(RootKeys.at(i),"logging")==0) continue;
		// ������� �����
		poco_debug_f1(*log,"Loading entry %s.",RootKeys.at(i));

				
				// ��������
				KeyName=RootKeys.at(i)+".Source";
				Source=pConf->getString(KeyName,"");
				if (Source.empty()) 
				{
					poco_error_f1(*log,"Skip entry %s. Source missing",RootKeys.at(i));
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
				// ������� ������
				RotateEntry re(confName,RootKeys.at(i),Source,ArchiverName);
				//re=new RotateEntry(confName,RootKeys.at(i),Source,ArchiverName);
				
				// ������ (�������� ����������� shift)
				KeyName=RootKeys.at(i)+".Size";
				KeyValue=pConf->getString(KeyName,"");
				re.setSize(KeyValue);

				//������ (������ shift)
				KeyName=RootKeys.at(i)+".Period";
				KeyValue=pConf->getString(KeyName,"0");
				re.setPeriod(KeyValue);

				// ����� ������. ������� ���� ����� ���������������� �����
				KeyName=RootKeys.at(i)+".Shift";
				re.shift=pConf->getBool(KeyName,re.shift);
				
				// Keep ������
				KeyName=RootKeys.at(i)+".Keep";
				re.keepPeriod=pConf->getInt(KeyName,0);
				
				/*
				if (LimitSize==0 && Period==0 && KeepPeriod==0) 
				{
					poco_error_f1(*log,"Skip entry %s. Period, KeepPeriod and size is null",RootKeys.at(i));
					continue;
				}
				*/

				// Target Dir. ������ ���� ����� ������������ ����������� shift
				KeyName=RootKeys.at(i)+".TargetDir";
				TargetDir=pConf->getString(KeyName,"");
				// Target Mask
				KeyName=RootKeys.at(i)+".TargetMask";
				TargetMask=pConf->getString(KeyName,"");
				
				re.setTarget(TargetDir,TargetMask);

				// Date mode
				KeyName=RootKeys.at(i)+".DateMode";
				KeyValue=pConf->getString(KeyName,"");
				re.setDateMode(KeyValue);

				// Date Replace Mode
				KeyName=RootKeys.at(i)+".DateReplace";
				KeyValue=pConf->getString(KeyName,"");
				re.setDateReplaceMode(KeyValue);

				// �������������
				KeyName=RootKeys.at(i)+".Recurse";
				re.recurse=pConf->getBool(KeyName,false);
				
				// ������ ����� ��������
				KeyName=RootKeys.at(i)+".PreRotate";
				re.preRotate=pConf->getString(KeyName,"");
				// ������ ����� �������
				KeyName=RootKeys.at(i)+".PostRotate";
				re.postRotate=pConf->getString(KeyName,"");
				
				items.push_back(re);
				//items.push_back(tmpItem);
				//items.push_back(RotateEntry(RootKeys.at(i),Source,Recurse,Period,LimitSize,ArchiverName,KeepPeriod,Shift,TargetDir,TargetMask,FDateMode,DateReplaceMode,PreRotate,PostRotate));
				//delete(re);
				//cout<< *it<<endl;
			 
			}
 }
}

//------------------------------------------------------------------------
//! �������������� ������� � int64 (�.�. ������ ����� ��������� K � M)
/*
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
*/
//! ���������� ������
std::string LogRotator::getVersion()
{
return ver;
}
