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
	// Добавляем 7z
	ArchiverParam SevenZipArh("7z","7z.exe"," a %ArhFileName %FileName");
	Archivers["7z"]=SevenZipArh; 
	setOptions("7z","");
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
	// Нужно заменить:
	
	Process::Args vectArgs;

	Executer::splitArgs(Archivers[archiveName].arguments,vectArgs); // Разбиваем строку аргументов на вектор
	
	string ArhFileName(targetPath); // Полное имя архива
	string sFileName; // Имя файла (короткое) и имя архива (короткое)
	string arguments(Archivers[archiveName].arguments); // Строка аргументов
	string exeName(Archivers[archiveName].exeName); // Exe файл
	string cmd; // Строка запуска целиком

	Path pPathSource(FileName);

	sFileName=pPathSource.getFileName();
	//Меняем в targetPath - %FileName и %yydd - на тек дату
	ArhFileName=ReplVar.replaceFile(ArhFileName,sFileName);
	ArhFileName=ReplVar.replaceDate(ArhFileName);
	ArhFileName+="."+Archivers[archiveName].extention;

	//меняем в аргументах архиватора %ArhFileName на полный путь и имя архива, %FileName - полный путь и имя архивируемого файла
	arguments=ReplVar.replaceFile(arguments,FileName,ArhFileName);
	arguments=ReplVar.replaceDate(arguments);
	
	// Осталось это все запустить
	cmd=exeName+" "+arguments;
	// Ищем полный путь к архиватору
	string EnvPath(Environment::get("PATH")); // Переменная окружения Path
	EnvPath=Path::expand(EnvPath); // Раскрываем всякие %Dir%

	Path pExe;
	bool found=Path::find(EnvPath,exeName,pExe); 
	if (!found) return false; // Архиватор не найден


	

}
//------------------------------------------------------------------------
//! Запустить внешнюю прогу и ждать завершения, возвращает успех
// exeName - только имя Exe без пути
// args разбивается на вектор по пробелам !?
bool Archiver::startProg(std::string exeName,std::string args)
{
// Ищем полный путь к архиватору
	string EnvPath(Environment::get("PATH")); // Переменная окружения Path
	EnvPath=Path::expand(EnvPath); // Раскрываем всякие %Dir%

	Path pExe;
	bool found=Path::find(EnvPath,exeName,pExe); 
	if (!found) return false; // Exe не найден
	string fullExe=pExe.toString();
	Process::Args pArgs;
	
	Process::launch(fullExe,pArgs);

}
//------------------------------------------------------------------------
//! Установить настройки (действуют на одну ротацию)
void Archiver::setOptions(std::string ArchiveName,std::string TargetPath)
{
	archiveName=ArchiveName;
	//Path pPathTarget(TargetPath);
	targetPath=TargetPath;
	//targetMask=pPathTarget.getFileName(); // Маска имени файла
	//pPathTarget.setFileName(""); 
	//targetPath=pPathTarget.toString(); // Каталог назначения
	arguments=Archivers[archiveName].arguments;
	exeName=Archivers[archiveName].exeName;

}
//------------------------------------------------------------------------
