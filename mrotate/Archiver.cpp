#include "StdAfx.h"
#include "Archiver.h"

#include "ReplVar.h"

using namespace std;

Archiver::Archiver(void)
	
	
	
{
	// ��������� 7z
	ArchiverParam SevenZipArh("7z","7z.exe"," a %ArhFileName %FileName");
	Archivers["7z"]=SevenZipArh; 
	setOptions("7z","","");
}
//------------------------------------------------------------------------
Archiver::~Archiver(void)
{
}
//------------------------------------------------------------------------
//! �������������� ���� ����������� � �������� �����������
bool Archiver::archiveFile(std::string ArchiverName,std::string FileName)
{
	
	// ����� ��������� �����, ����� ��������� � ���������
	
}
//------------------------------------------------------------------------
//! ���������� ��������� (��������� �� ���� �������)
void Archiver::setOptions(std::string ArchiveName,std::string TargetPath,std::string TargetMask)
{
	archiveName=ArchiveName;
	targetPath=TargetPath;
	targetMask=TargetMask;
	arguments=Archivers[archiveName].arguments;
	exeName=Archivers[archiveName].exeName;

}
//------------------------------------------------------------------------
