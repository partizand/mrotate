#include "StdAfx.h"
#include "LogRotator.h"


#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\Path.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco/NumberParser.h>

using namespace Poco;
using namespace Util;

LogRotator::LogRotator(void)
{
}


LogRotator::~LogRotator(void)
{
}
//! Загрузка параметров ротации из файла
void LogRotator::load(const std::string &fileName)
{
// Определение типа файла по расширению
	Poco::Path pPath(fileName);
	Poco::File pFile(pPath);

	if (!pFile.exists()) return; // Файла нет
	if (!pFile.canRead()) return; // Файл не может быть прочитан

	string Ext=pPath.getExtension();
	toLowerInPlace(Ext); // Расширение маленькими буквами

	AutoPtr<AbstractConfiguration> pConf;

	
	if (Ext.compare("ini")==0) // Конфигурация ini
	{
		pConf=new IniFileConfiguration(fileName);
	}
	if (Ext.compare("properties")==0) // Конфигурация prop
	{
		pConf=new PropertyFileConfiguration(fileName);
	}
	if (pConf.isNull()) return; // Нет нужного расширения
	
	load(pConf);
}

//! Загрузка настроек ротации
void LogRotator::load(const Poco::Util::AbstractConfiguration *pConf)
{
RotateEntry tmpItem;
AbstractConfiguration::Keys RootKeys;
pConf-> keys("",RootKeys); // Список корневых ключей
if (!RootKeys.empty())
 {
	string KeyName,KeyValue;
	int i;
	for (i=0;i<RootKeys.size();++i)
	
	 //for (AbstractConfiguration::Keys::const_iterator it = RootKeys.begin(); it != RootKeys.end(); ++it)
			{
				//KeyName=*it+".source";
				// Источник
				KeyName=RootKeys.at(i)+".source";
				tmpItem.source=pConf->getString(KeyName,"");
				if (tmpItem.source=="") 
				{
					//poco_error_f1(*AppLogger,"Source missing in config %s",RootKeys.at(i));
					continue;
				}
				//Период и тип
				KeyName=RootKeys.at(i)+".period";
				KeyValue=pConf->getString(KeyName,"0");
				tmpItem.period=convertPeriod(KeyValue,tmpItem.type);
				// Размер

				KeyName=RootKeys.at(i)+".nametask";
				tmpTask.NameTask=pConf->getString(KeyName,"");
				if (tmpTask.NameTask=="") 
				{
					tmpTask.NameTask=RootKeys.at(i);
				}
				//cout<< *it<<endl;
			addTask(tmpTask);
			}
 }
}
//! Преобразование размера в int64
Int64 LogRotator::convertSize(std::string &strSize)
{
	strSize
}
//! Преобразование строкового периода в int и определение типа ротации
int LogRotator::convertPeriod(std::string &strPeriod,Rotate::RotateType &rType)
{
	int iPeriod;
	//string sPeriod;
	trimInPlace(strPeriod); // Обрезаем пробелы
	// Ищем строки 
	rType=Rotate::RotateType.Single; 
	if (icompare(strPeriod,"Daily")==0)
	{
		
		return 1;
	}
	if (icompare(strPeriod,"weekly")==0)
	{
		
		return 7;
	}
	if (icompare(strPeriod,"monthly")==0)
	{
		
		return 30;
	}
	// Преобразуем строку в число
	rType=Rotate::RotateType.Multiple;
	if (NumberParser::tryParse(strPeriod,iPeriod))
	{
		return iPeriod;
	}
	else
	{
		return 0;
	}

}

