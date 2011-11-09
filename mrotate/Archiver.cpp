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


#include "ReplVar.h"
#include "Executer.h"
//#include <Poco\DateTime.h>
#include <Poco\Path.h>
#include <Poco\Environment.h>
#include <Poco\Process.h>
#include <Poco\Path.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>

#include "Archiver.h"
#include "Executer.h"

using namespace std;
using namespace Poco;
using namespace Util;

const std::string noneArchiverName="no";

Archiver::Archiver(Poco::Logger &logger):
_debugMode(false)
	
{
	log=&logger;
	// 7z
	Archivers["7z"]=ArchiverParam("7z",".7z","7z.exe","a %ArhFileName %FullFileName -m0=PPMd");
	Archivers["7zlzma"]=ArchiverParam("7z",".7z","7z.exe","a %ArhFileName %FullFileName");
	// rar
	Archivers["rar"]=ArchiverParam("rar",".rar","rar.exe","a %ArhFileName %FullFileName");
	Archivers["winrar"]=ArchiverParam("winrar",".rar","winrar.exe","a %ArhFileName %FullFileName");
	
	Archivers[noneArchiverName]=ArchiverParam(noneArchiverName,"","","");
	//setOptions("7z","");
}
//------------------------------------------------------------------------
Archiver::~Archiver(void)
{
}
//------------------------------------------------------------------------
//! �������������� ���� 
bool Archiver::archiveFile(const std::string &arhiverName,const std::string &fileName,const std::string &arhFileName,const Poco::DateTime &replDate)
{
// ����� ��������� �����, ����� ��������� � ���������

	
	

	

	//Path pPathSource(FileName);
	//pPathSource.makeFile();

	//string sFileName(pPathSource.getFileName()); // �������� ��� ����� ���������
	//������ � targetPath - %FileName � %yydd - �� ��� ����
	
	string ArhFileName(arhFileName); // ������ ��� ������
	ArhFileName+=Archivers[arhiverName].extension;

	//������ � ���������� ���������� %ArhFileName �� ������ ���� � ��� ������, %FileName - ������ ���� � ��� ������������� �����
	vector<std::string> vectArgs;
	Executer::splitArgs(Archivers[arhiverName].arguments,vectArgs); // ��������� ������ ���������� �� ������
	ReplVar::replaceFileAndDate(vectArgs,fileName,ArhFileName,replDate); 
	
	// ������� ������� ����������
	Executer::createDir(ArhFileName);
	
	// �������� ��� ��� ���������
	int ExitCode;
	if (icompare(arhiverName,noneArchiverName)==0) // ��������������
	{
		Executer::moveFile(fileName,ArhFileName,_debugMode,*log);
		/*
		if (!_debugMode)
			{
			File pFile(fileName);
			poco_information_f2(*log,"File %s moving to %s",fileName,ArhFileName);
			try
			{
			pFile.moveTo(ArhFileName);
			ExitCode=0;
			}
			catch(...)
			{
				poco_error_f2(*log,"Error move file %s to %s",fileName,ArhFileName);
				ExitCode=1;
			}
			}
		else
			{
			poco_information_f2(*log,"Move file %s to %s",fileName,ArhFileName);
			ExitCode=0;
			}
			*/
	}
	else // ���������
	{
		if (!_debugMode)
			{
				string fullExeName(Archivers[arhiverName].fullExeName); // Exe ����
				ExitCode=Executer::execute(fullExeName,vectArgs);
				if (ExitCode==0)
				poco_information_f2(*log,"File archived %s. Exit code %i",fileName,ExitCode);
			}
		else
			{
			poco_information_f1(*log,"[Debug] Archiving file %s",fileName);
			ExitCode=0;
			}
	}
	if (ExitCode==0) 
	{
		return true;
	}
	else
	{
		return false;
	}
	

	
}
//------------------------------------------------------------------------
//! �������������� ���� ����������� � �������� �����������
/*
bool Archiver::archiveFile(const std::string &FileName)
{
	// ����� ��������� �����, ����� ��������� � ���������

	string exeName(Archivers[archiveName].exeName); // Exe ����

	Path pPathSource(FileName);
	pPathSource.makeFile();

	string sFileName(pPathSource.getFileName()); // �������� ��� ����� ���������
	//������ � targetPath - %FileName � %yydd - �� ��� ����
	string ArhFileName(targetPath); // ������ ��� ������
	ArhFileName=ReplVar::replaceFileAndDate(ArhFileName,sFileName); 
	ArhFileName+=Archivers[archiveName].extension;

	//������ � ���������� ���������� %ArhFileName �� ������ ���� � ��� ������, %FileName - ������ ���� � ��� ������������� �����
	vector<std::string> vectArgs;
	Executer::splitArgs(Archivers[archiveName].arguments,vectArgs); // ��������� ������ ���������� �� ������
	ReplVar::replaceFileAndDate(vectArgs,FileName,ArhFileName); 
	
	// 
	
	// �������� ��� ��� ���������
	int ExitCode;
	if (icompare(archiveName,noneArchiverName)==0) // ��������������
	{
		if (!_debugMode)
			{
			File pFile(pPathSource);
			poco_information_f2(*log,"File %s moving to %s",FileName,ArhFileName);
			try
			{
			pFile.moveTo(ArhFileName);
			ExitCode=1; // ��� �� ���������� ������� �� ������� ���� �������� ���
			}
			catch(...)
			{
				poco_error_f2(*log,"Error move file %s to %s",FileName,ArhFileName);
				ExitCode=1;
			}
			}
		else
			{
			poco_information_f2(*log,"Move file %s to %s",FileName,ArhFileName);
			ExitCode=0;
			}
	}
	else // ���������
	{
		if (!_debugMode)
			{
			ExitCode=Executer::execute(exeName,vectArgs);
			}
		else
			{
			poco_information_f1(*log,"Archive file %s",FileName);
			ExitCode=0;
			}
	}
	if (ExitCode==0) 
	{
		return true;
	}
	else
	{
		return false;
	}
	

	

}
*/
//------------------------------------------------------------------------
//! ���������� ����� ������� (�������� ���������)
void Archiver::setDebugMode()
{
_debugMode=true;
}
//------------------------------------------------------------------------
//! �������� ���������� �� �����
void Archiver::load(const std::string &fileName)
{
// ����������� ���� ����� �� ����������
	Poco::Path pPath(fileName);
	Poco::File pFile(pPath);

	if (!pFile.exists()) return; // ����� ���
	if (!pFile.canRead()) return; // ���� �� ����� ���� ��������

	string Ext=pPath.getExtension();
	//toLowerInPlace(Ext); // ���������� ���������� �������

	AutoPtr<AbstractConfiguration> pConf;

	
	if (icompare(Ext,"ini")==0) // ������������ ini
	{
		pConf=new IniFileConfiguration(fileName);
	}
	if (icompare(Ext,"properties")==0) // ������������ prop
	{
		pConf=new PropertyFileConfiguration(fileName);
	}
	if (pConf.isNull()) return; // ��� ������� ����������
	
	load(pConf);
}
//------------------------------------------------------------------------
//! �������� �������� 
void Archiver::load(const Poco::Util::AbstractConfiguration *pConf)
{
//RotateEntry tmpItem;
string ArchiverName,ExeName,Extension,Args;

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
				
				poco_debug_f1(*log,"Loading arh entry %s.",RootKeys.at(i));

				//KeyName=*it+".source";
				
				// ���
				ArchiverName=RootKeys.at(i);
				toLowerInPlace(ArchiverName);
				// Exe
				KeyName=RootKeys.at(i)+".exeName";
				ExeName=pConf->getString(KeyName,"");


				//if (ExeName.empty())
				//{
				//	continue;
				//}

				// ����������
				KeyName=RootKeys.at(i)+".Extension";
				Extension=pConf->getString(KeyName,"");

				// ���������
				KeyName=RootKeys.at(i)+".Args";
				Args=pConf->getString(KeyName,"");

				

				//RotateEntry tmpItem(Source,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask);

				Archivers[ArchiverName]=ArchiverParam(ArchiverName,Extension,ExeName,Args);

				
			 
			}
 }
}
//------------------------------------------------------------------------
//! ��������� ��������� 
bool Archiver::isValid(const std::string &archiverName)
{
// ���������, ���� �� ����� ���������
	std::map<std::string,ArchiverParam>::iterator it=Archivers.find(archiverName);
	if (it==Archivers.end()) // ������ ���������� ���
	{
		poco_error_f1(*log,"ArchiveName %s not found",archiverName);
		return false;
	}
	
	//string fullPathArh;
	//bool found=Executer::getFullPath(Archivers[archiverName].exeName,fullPathArh);
	if (!Archivers[archiverName].exeName.empty() && Archivers[archiverName].fullExeName.empty())
	{
		poco_error_f2(*log,"Executable %s not found for archiver %s",Archivers[archiverName].exeName,archiverName);
		return false;
	}
	
	return true;
}
//------------------------------------------------------------------------
//! ���������� ��������� (��������� �� ���� �������)
/*
bool Archiver::setOptions(const std::string &ArchiveName,const std::string &TargetPath)
{
	// ���������, ���� �� ����� ���������
	std::map<std::string,ArchiverParam>::iterator it=Archivers.find(ArchiveName);
	if (it==Archivers.end()) // ������ ���������� ���
	{
		poco_error_f1(*log,"ArchiveName %s not found",ArchiveName);
		return false;
	}
	archiveName=ArchiveName;
	string fullPathArh;
	bool found=Executer::getFullPath(Archivers[ArchiveName].exeName,fullPathArh);
	if (!found)
	{
		poco_error_f2(*log,"Executable %s not found for archiver %s",Archivers[ArchiveName].exeName,ArchiveName);
		return false;
	}
	targetPath=TargetPath;
	return true;

}
*/
//------------------------------------------------------------------------
//! ���������� ���������� �����, ��� �������� �������������� ����������
std::string Archiver::getExtension(const std::string &ArchiverName)
{
	
		return Archivers[ArchiverName].extension;
	
}
//------------------------------------------------------------------------
