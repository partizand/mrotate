#include "StdAfx.h"
#include "Archiver.h"

#include "ReplVar.h"

using namespace std;

Archiver::Archiver(void)
	
	
	
{
	// Добавляем 7z
	ArchiverParam SevenZipArh("7z","7z.exe"," a %ArhFileName %FileName");
	Archivers["7z"]=SevenZipArh; 
	setOptions("7z","","");
}
//------------------------------------------------------------------------
Archiver::~Archiver(void)
{
}
//------------------------------------------------------------------------
//! Заархивировать файл архиватором с текущими настройками
bool Archiver::archiveFile(std::string ArchiverName,std::string FileName)
{
	
	// Нужно построить опции, найти архиватор и запустить
	
}
//------------------------------------------------------------------------
//! Установить настройки (действуют на одну ротацию)
void Archiver::setOptions(std::string ArchiveName,std::string TargetPath,std::string TargetMask)
{
	archiveName=ArchiveName;
	targetPath=TargetPath;
	targetMask=TargetMask;
	arguments=Archivers[archiveName].arguments;
	exeName=Archivers[archiveName].exeName;

}
//------------------------------------------------------------------------
