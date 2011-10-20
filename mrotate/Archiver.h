#pragma once

#include <vector>
#include <map>

#include "ArchiverParam.h"
//! Реализация архивации файлов внешним архиватором
class Archiver
{
public:
	Archiver(void);
	~Archiver(void);
	//! Установить настройки (действуют на одну ротацию)
	bool setOptions(std::string ArchiveName,std::string TargetPath);
	
	//! Заархивировать файл архиватором с текущими настройками
	bool archiveFile(std::string FileName);
private:
	//! Архиваторы
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;
	

	//! Имя архиватора
	std::string archiveName;
	//! Каталог назначения
	std::string targetPath;
	
	
};

