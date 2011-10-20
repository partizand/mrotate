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
	ArchiverParam SevenZipArh("7z","7z.exe"," a %ArhFileName %FileName");
	Archivers["7z"]=SevenZipArh; 
	setOptions("7z","");
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
	// ����� ��������:
	
	Process::Args vectArgs;

	Executer::splitArgs(Archivers[archiveName].arguments,vectArgs); // ��������� ������ ���������� �� ������
	
	string ArhFileName(targetPath); // ������ ��� ������
	string sFileName; // ��� ����� (��������) � ��� ������ (��������)
	string arguments(Archivers[archiveName].arguments); // ������ ����������
	string exeName(Archivers[archiveName].exeName); // Exe ����
	string cmd; // ������ ������� �������

	Path pPathSource(FileName);

	sFileName=pPathSource.getFileName();
	//������ � targetPath - %FileName � %yydd - �� ��� ����
	ArhFileName=ReplVar.replaceFile(ArhFileName,sFileName);
	ArhFileName=ReplVar.replaceDate(ArhFileName);
	ArhFileName+="."+Archivers[archiveName].extention;

	//������ � ���������� ���������� %ArhFileName �� ������ ���� � ��� ������, %FileName - ������ ���� � ��� ������������� �����
	arguments=ReplVar.replaceFile(arguments,FileName,ArhFileName);
	arguments=ReplVar.replaceDate(arguments);
	
	// �������� ��� ��� ���������
	cmd=exeName+" "+arguments;
	// ���� ������ ���� � ����������
	string EnvPath(Environment::get("PATH")); // ���������� ��������� Path
	EnvPath=Path::expand(EnvPath); // ���������� ������ %Dir%

	Path pExe;
	bool found=Path::find(EnvPath,exeName,pExe); 
	if (!found) return false; // ��������� �� ������


	

}
//------------------------------------------------------------------------
//! ��������� ������� ����� � ����� ����������, ���������� �����
// exeName - ������ ��� Exe ��� ����
// args ����������� �� ������ �� �������� !?
bool Archiver::startProg(std::string exeName,std::string args)
{
// ���� ������ ���� � ����������
	string EnvPath(Environment::get("PATH")); // ���������� ��������� Path
	EnvPath=Path::expand(EnvPath); // ���������� ������ %Dir%

	Path pExe;
	bool found=Path::find(EnvPath,exeName,pExe); 
	if (!found) return false; // Exe �� ������
	string fullExe=pExe.toString();
	Process::Args pArgs;
	
	Process::launch(fullExe,pArgs);

}
//------------------------------------------------------------------------
//! ���������� ��������� (��������� �� ���� �������)
void Archiver::setOptions(std::string ArchiveName,std::string TargetPath)
{
	archiveName=ArchiveName;
	//Path pPathTarget(TargetPath);
	targetPath=TargetPath;
	//targetMask=pPathTarget.getFileName(); // ����� ����� �����
	//pPathTarget.setFileName(""); 
	//targetPath=pPathTarget.toString(); // ������� ����������
	arguments=Archivers[archiveName].arguments;
	exeName=Archivers[archiveName].exeName;

}
//------------------------------------------------------------------------
