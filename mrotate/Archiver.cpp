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

Archiver::Archiver(Poco::Logger &logger)
	
	
	
{
	log=&logger;
	// Добавляем 7z
	ArchiverParam SevenZipArh("7z",".7z","7z.exe"," a %ArhFileName %FullFileName -m0=PPMd");
	Archivers["7z"]=SevenZipArh; 
	//setOptions("7z","");
}
//------------------------------------------------------------------------
Archiver::~Archiver(void)
{
}
//------------------------------------------------------------------------
//! Заархивировать файл архиватором с текущими настройками
bool Archiver::archiveFile(std::string FileName)
{
	// Нужно построить опции, найти архиватор и запустить

	string exeName(Archivers[archiveName].exeName); // Exe файл

	Path pPathSource(FileName);
	pPathSource.makeFile();

	string sFileName(pPathSource.getFileName()); // Короткое имя файла источника
	//Меняем в targetPath - %FileName и %yydd - на тек дату
	string ArhFileName(targetPath); // Полное имя архива
	ArhFileName=ReplVar::replaceFileAndDate(ArhFileName,sFileName); // Полный путь пропадает!
	ArhFileName+=Archivers[archiveName].extention;

	//меняем в аргументах архиватора %ArhFileName на полный путь и имя архива, %FileName - полный путь и имя архивируемого файла
	vector<std::string> vectArgs;
	Executer::splitArgs(Archivers[archiveName].arguments,vectArgs); // Разбиваем строку аргументов на вектор
	ReplVar::replaceFileAndDate(vectArgs,FileName,ArhFileName); // Все аргументы становятя полный путь к файлу
	
	
	// Осталось это все запустить
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
//! Установить настройки (действуют на одну ротацию)
bool Archiver::setOptions(std::string ArchiveName,std::string TargetPath)
{
	// Проверяем, есть ли такой архиватор
	std::map<std::string,ArchiverParam>::iterator it=Archivers.find(ArchiveName);
	if (it==Archivers.end()) // Такого архиватора нет
	{
		poco_error_f1(*log,"ArchiveName %s not found",ArchiveName);
		return false;
	}
	archiveName=ArchiveName;
	
	targetPath=TargetPath;
	return true;

}
//------------------------------------------------------------------------
//! Возвращает расширение файла, для текущего установленного архиватора
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
