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
//! Загрузка параметров из файла
void Archiver::load(const std::string &fileName)
{
// Определение типа файла по расширению
	Poco::Path pPath(fileName);
	Poco::File pFile(pPath);

	if (!pFile.exists()) return; // Файла нет
	if (!pFile.canRead()) return; // Файл не может быть прочитан

	string Ext=pPath.getExtension();
	//toLowerInPlace(Ext); // Расширение маленькими буквами

	AutoPtr<AbstractConfiguration> pConf;

	
	if (icompare(Ext,"ini")==0) // Конфигурация ini
	{
		pConf=new IniFileConfiguration(fileName);
	}
	if (icompare(Ext,"properties")==0) // Конфигурация prop
	{
		pConf=new PropertyFileConfiguration(fileName);
	}
	if (pConf.isNull()) return; // Нет нужного расширения
	
	load(pConf);
}
//------------------------------------------------------------------------
//! Загрузка настроек 
void Archiver::load(const Poco::Util::AbstractConfiguration *pConf)
{
//RotateEntry tmpItem;
string ArchiverName,ExeName,Extension,Args;

AbstractConfiguration::Keys RootKeys;
pConf-> keys("",RootKeys); // Список корневых ключей
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	int i;
	for (i=0;i<RootKeys.size();++i)
	
	 //for (AbstractConfiguration::Keys::const_iterator it = RootKeys.begin(); it != RootKeys.end(); ++it)
			{
				// Базовые ключи
				//KeyName=RootKeys.at(i);
				if (icompare(RootKeys.at(i),"application")==0) continue;
				if (icompare(RootKeys.at(i),"system")==0) continue;
				if (icompare(RootKeys.at(i),"logging")==0) continue;
				
				poco_debug_f1(*log,"Loading arh entry %s.",RootKeys.at(i));

				//KeyName=*it+".source";
				
				// Имя
				ArchiverName=RootKeys.at(i);
				// Exe
				KeyName=RootKeys.at(i)+".exeName";
				ExeName=pConf->getString(KeyName,"");
				if (ExeName.empty())
				{
					continue;
				}

				// Расширение
				KeyName=RootKeys.at(i)+".Extension";
				Extension=pConf->getString(KeyName,"");

				// Аргументы
				KeyName=RootKeys.at(i)+".Args";
				Args=pConf->getString(KeyName,"");

				

				//RotateEntry tmpItem(Source,Period,LimitSize,ArchiverName,KeepPeriod,TargetDir,TargetMask);

				Archivers[ArchiverName]=ArchiverParam(ArchiverName,Extension,ExeName,Args);
			 
			}
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
