#include "StdAfx.h"
#include "Archiver.h"

#include "ReplVar.h"
#include "Executer.h"
//#include <Poco\DateTime.h>
#include <Poco\Path.h>
#include <Poco\Environment.h>
#include <Poco\Process.h>

using namespace std;
using namespace Poco;

Archiver::Archiver(void)
	
	
	
{
	// ��������� 7z
	ArchiverParam SevenZipArh("7z",".7z","7z.exe"," a %ArhFileName %FileName");
	Archivers["7z"]=SevenZipArh; 
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

	string sFileName(pPathSource.getFileName()); // �������� ��� ����� ���������
	//������ � targetPath - %FileName � %yydd - �� ��� ����
	string ArhFileName(targetPath); // ������ ��� ������
	ArhFileName=ReplVar::replaceFileAndDate(ArhFileName,sFileName);
	ArhFileName+=Archivers[archiveName].extention;

	//������ � ���������� ���������� %ArhFileName �� ������ ���� � ��� ������, %FileName - ������ ���� � ��� ������������� �����
	vector<std::string> vectArgs;
	Executer::splitArgs(Archivers[archiveName].arguments,vectArgs); // ��������� ������ ���������� �� ������
	ReplVar::replaceFileAndDate(vectArgs,FileName,ArhFileName);
	
	
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
//! ���������� ��������� (��������� �� ���� �������)
bool Archiver::setOptions(std::string ArchiveName,std::string TargetPath)
{
	// ���������, ���� �� ����� ���������
	std::map<std::string,ArchiverParam>::iterator it=Archivers.find(ArchiveName);
	if (it==Archivers.end()) // ������ ���������� ���
	{
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
