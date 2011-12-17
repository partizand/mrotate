#include "StdAfx.h"
#include "RStatus.h"

//#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco\Path.h>
//#include <Poco\DirectoryIterator.h>
#include <Poco\NumberParser.h>
//#include <Poco\Glob.h>
#include <Poco\Timestamp.h>
#include <Poco\DateTime.h>

using namespace std;
using namespace Poco;
using namespace Util;

RStatus::RStatus(Poco::Logger &logger)
{
	log=&logger;
	
}


RStatus::~RStatus(void)
{
}
//======================================================
//! Устанавливает каталог сохранения файлов конфигураций
void RStatus::setStatusFileName(const std::string &fName)
{
	statusFileName=fName;
}

//------------------------------------------------------------------------
//! Загрузка параметров из файла
void RStatus::load()
{
// Определение типа файла по расширению
	//Poco::Path pPath(fileName);
	Poco::File pFile(statusFileName);

	if (!pFile.exists()) return; // Файла нет
	if (!pFile.canRead()) return; // Файл не может быть прочитан

	//AutoPtr<AbstractConfiguration> pConf;

	pConf=new PropertyFileConfiguration(statusFileName);
	
	AbstractConfiguration::Keys RootKeys;
	pConf-> keys("",RootKeys); // Список корневых ключей
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	//RotateEntry re;
	int i;
	for (i=0;i<RootKeys.size();++i)
	{
			AbstractConfiguration::Keys level2Keys;
			pConf-> keys(RootKeys.at(i),RootKeys); // Список корневых ключей
				
				KeyName=RootKeys.at(i)+".Source";
				KeyValue=pConf->getString(KeyName,"");
				rDates[RootKeys.at(i)]
	}
}
	
}
