#include "StdAfx.h"
#include "LogRotator.h"


#include <Poco\Util\IniFileConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\Path.h>
#include <Poco\String.h>
#include <Poco\File.h>
#include <Poco\NumberParser.h>
#include <Poco\Glob.h>
#include <Poco\Timestamp.h>
#include <Poco\DateTime.h>

using namespace std;
using namespace Poco;
using namespace Util;

LogRotator::LogRotator(Poco::Logger &logger)

{
	log=&logger;
}


LogRotator::~LogRotator(void)
{
}
//------------------------------------------------------------------------
//! Ротировать файлы (основная функция)
void LogRotator::rotate()
{
	int i;
	bool suc;
	string arhMask;
	vector<string> fileList;
	// Перебираем все записи
	for (i=0;i<items.size();++i)
	{
		currIndex=i;
		suc=archiver.setOptions(items[i].archiverName,items[i].targetPath);
		if (!suc) continue; // Ошибка в архиваторе
		// Получить список файлов для обработки
		getFileList(fileList,items.at(currIndex).source);
		// Ротируем файлы
		rotateFile(fileList);
		// Удаляем старые архивы
		if (items[i].keepPeriod>0)
		{
			arhMask=items.at(currIndex).source
			getFileList(fileList,items[i].keepPeriod
		}

	}
}
//------------------------------------------------------------------------
//! Получить список файлов по маске, отобранных по периоду или размеру, если period и lSize не заданы, берутся из настройки текущей ротации
void LogRotator::getFileList(std::vector<std::string> &fileList, const std::string &pathMask,int Period,unsigned long int lSize)
{
set<string> files;
fileList.clear();
Glob::glob(pathMask,files); // Волшебная функция получения списка файлов по маске
/*
if (Period==0 && lSize==0) 
{
	fileList.assign(files.begin(),files.end());
	return;
}
*/
// Перебираем все файлы
	set<string>::iterator it=files.begin();
	for (;it!=files.end();++it)
	{
		if (isRotateFile(*it,Period,lSize)) // Добавляем файл если подходит
		{
			fileList.push_back(*it);
		}
	}
}
//------------------------------------------------------------------------
//! Получить список файлов для обработки
/*
void LogRotator::getFileList(std::vector<std::string> &fileList)
{
	set<string> files;
	fileList.clear();
	Glob::glob(items.at(currIndex).source,files); // Волшебная функция получения списка файлов по маске
	// Перебираем все файлы
	set<string>::iterator it=files.begin();
	for (;it!=files.end();++it)
	{
		// *it - имя файла
		if (isRotateFile(*it)) // Добавляем файл если подходит
		{
			fileList.push_back(*it);
		}
	}

}
*/
//------------------------------------------------------------------------
//! Ротировать список файлов
void LogRotator::rotateFile(const std::vector<std::string> &listFiles)
{
	int i;
	for (i=0;i<listFiles.size();++i)
	{
		rotateFile(listFiles[i]);
	}
	
}
//------------------------------------------------------------------------
//! Ротировать заданный файл
void LogRotator::rotateFile(const std::string &fileName)
{
	// Архивируем файл
	bool suc=archiver.archiveFile(fileName);
	if (suc) // Успешно заархивировался
	{
		Poco::File pFile(fileName);
		pFile.remove(); // Удаляем его
	}
}
//------------------------------------------------------------------------
//! Ротировать архивный файл (т.е. удалить его)
void LogRotator::rotateArhFile(const std::string &fileName)
{
Poco::File pFile(fileName);
pFile.remove(); // Удаляем его
}

//------------------------------------------------------------------------
//! Ротировать список архивных файлов
void LogRotator::rotateArhFile(const std::vector<std::string> &listFiles)
{
	int i;
	for (i=0;i<listFiles.size();++i)
	{
		rotateArhFile(listFiles[i]);
	}
}
//------------------------------------------------------------------------
//! Проверить нужно ли ротировать данный файл, если period и lSize не заданы, берутся из настройки текущей ротации
bool LogRotator::isRotateFile(const std::string &fileName,int Period/*=0*/,unsigned long int lSize/*=0*/)
{
Poco::File pFile(fileName);

	if (!pFile.exists()) return false; // Файла нет
	if (!pFile.isFile()) return false; // Это не файл
	if (!pFile.canRead()) return false; // Файл не может быть прочитан

	int iPeriod(Period);
	unsigned long int iSize(lSize);
	if (Period==0 && lSize==0) // Параметры не заданы, берем из текущей настройки ротации
	{
		iPeriod=items.at(currIndex).period;
		iSize=items.at(currIndex).limitSize;
	}

	if (iPeriod>0) //задан период обрабтки
	{
		//Timestamp periodTime;  //Текущее время
		DateTime pTime; //Время для сравнения
		//DateTime fTime; // Время файла
		Timespan diffTime(iPeriod-1,23,0,0,0);  //Сколько нужно отнять
		pTime-=diffTime; 
		//periodTime=periodTime-diffTime; // 

		// дата создание файла должена быть старше period - 1 час
		DateTime fTime(pFile.created());
		if (fTime<=pTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (iSize>0)
	{
		File::FileSize fsize;
		fsize=pFile.getSize();
		if (fsize > iSize)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
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
				tmpItem.period=pConf->getInt(KeyName,0);
				
				// Размер

				items.push_back(tmpItem);
				//cout<< *it<<endl;
			 
			}
 }
}
//! Преобразование размера в int64
long int LogRotator::convertSize(std::string &strSize)
{
	Int64 iSize;
	// Преобразуем строку в число
	
	if (NumberParser::tryParse64(strSize,iSize))
	{
		return iSize;
	}
	else
	{
		return 0;
	}
}
//! Преобразование строкового периода в int и определение типа ротации
/*
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
*/

