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

using namespace std;
using namespace Poco;
using namespace Util;

Archiver::Archiver(Poco::Logger &logger)
	
	
	
{
	log=&logger;
	// 7z
	Archivers["7z"]=ArchiverParam("7z",".7z","7z.exe"," a %ArhFileName %FullFileName -m0=PPMd");
	// rar
	Archivers["rar"]=ArchiverParam("rar",".rar","rar.exe"," a %ArhFileName %FullFileName");
	
	//setOptions("7z","");
}
//------------------------------------------------------------------------
Archiver::~Archiver(void)
{
}
//------------------------------------------------------------------------
//! �������������� ���� ����������� � �������� �����������
bool Archiver::archiveFile(std::string FileName)
{
	// ����� ��������� �����, ����� ��������� � ���������

	string exeName(Archivers[archiveName].exeName); // Exe ����

	Path pPathSource(FileName);
	pPathSource.makeFile();

	string sFileName(pPathSource.getFileName()); // �������� ��� ����� ���������
	//������ � targetPath - %FileName � %yydd - �� ��� ����
	string ArhFileName(targetPath); // ������ ��� ������
	ArhFileName=ReplVar::replaceFileAndDate(ArhFileName,sFileName); // ������ ���� ���������!
	ArhFileName+=Archivers[archiveName].extention;

	//������ � ���������� ���������� %ArhFileName �� ������ ���� � ��� ������, %FileName - ������ ���� � ��� ������������� �����
	vector<std::string> vectArgs;
	Executer::splitArgs(Archivers[archiveName].arguments,vectArgs); // ��������� ������ ���������� �� ������
	ReplVar::replaceFileAndDate(vectArgs,FileName,ArhFileName); // ��� ��������� ��������� ������ ���� � �����
	
	
	// �������� ��� ��� ���������
	int ExitCode;
	ExitCode=Executer::execute(exeName,vectArgs);
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
				// Exe
				KeyName=RootKeys.at(i)+".exeName";
				ExeName=pConf->getString(KeyName,"");
				if (ExeName.empty())
				{
					continue;
				}

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
//! ���������� ��������� (��������� �� ���� �������)
bool Archiver::setOptions(std::string ArchiveName,std::string TargetPath)
{
	// ���������, ���� �� ����� ���������
	std::map<std::string,ArchiverParam>::iterator it=Archivers.find(ArchiveName);
	if (it==Archivers.end()) // ������ ���������� ���
	{
		poco_error_f1(*log,"ArchiveName %s not found",ArchiveName);
		return false;
	}
	archiveName=ArchiveName;
	
	targetPath=TargetPath;
	return true;

}
//------------------------------------------------------------------------
//! ���������� ���������� �����, ��� �������� �������������� ����������
std::string Archiver::getArhExtention(std::string ArchiveName/*=""*/)
{
	if (ArchiveName.empty())
	{
		return Archivers[archiveName].extention;
	}
	else
		return Archivers[ArchiveName].extention;
}
//------------------------------------------------------------------------
