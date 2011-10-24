#pragma once

#include <vector>
#include <map>

#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>

#include "ArchiverParam.h"
//! Реализация архивации файлов внешним архиватором
class Archiver
{
public:
	Archiver(Poco::Logger &logger);
	~Archiver(void);
	//! Установить настройки (действуют на одну ротацию)
	bool setOptions(std::string ArchiveName,std::string TargetPath);
	
	//! Заархивировать файл архиватором с текущими настройками
	bool archiveFile(std::string FileName);

	//! Загрузка настроек из файла
	void load(const std::string &fileName);
	//! Загрузка настроек 
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! Установить режим отладки (эмуляция архивации)
	void setDebugMode();
	//! Возвращает расширение файла, для текущего установленного архиватора
	std::string getArhExtention(std::string ArchiveName="");
private:
	//! Архиваторы
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;
	//! Установлен режим отладки
	bool _debugMode;

	//! Имя архиватора
	std::string archiveName;
	//! Каталог назначения
	std::string targetPath;
	//! Логгер
	Poco::Logger *log;
	
};

