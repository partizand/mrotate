#pragma once

#include <vector>
#include <map>

#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\DateTime.h>

#include "ArchiverParam.h"
//! Реализация архивации файлов внешним архиватором
class Archiver
{
public:
	Archiver(Poco::Logger &logger);
	~Archiver(void);
	//! Установить настройки (действуют на одну ротацию)
	//bool setOptions(const std::string &ArchiveName,const std::string &TargetPath);
	
	//! Проверить настройки 
	bool isValid(const std::string &archiverName);

	//! Заархивировать файл 
	bool archiveFile(const std::string &arhiverName,const Poco::File &pFile,const std::string &arhFileName,const Poco::DateTime &replDate);

	//! Заархивировать файл архиватором с текущими настройками
	//bool archiveFile(const std::string &FileName);

	//! Загрузка настроек из файла
	void load(const std::string &fileName);
	//! Загрузка настроек 
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! Установить режим отладки (эмуляция архивации)
	void setDebugMode();
	//! Возвращает расширение файла для архиватора
	std::string getExtension(const std::string &ArchiverName);
private:
	//! Архиваторы
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;
	//! Установлен режим отладки
	bool _debugMode;

	//! Имя архиватора
	//std::string archiveName;
	//! Каталог назначения
	//std::string targetPath;
	//! Логгер
	Poco::Logger *log;
	
};

