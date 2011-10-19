#pragma once

#include "ArchiverParam.h"
#include <vector>
#include <map>
//! Реализация архивации файлов внешним архиватором
class Archiver
{
public:
	Archiver(void);
	~Archiver(void);
	//! Установить настройки (действуют на одну ротацию)
	void setOptions(std::string ArchiveName,std::string TargetPath,std::string TargetMask);
	
	//! Заархивировать файл архиватором с текущими настройками
	bool archiveFile(std::string ArchiverName,std::string FileName);
private:
	//! Архиваторы
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;

	//! Имя архиватора
	std::string archiveName;
	//! Имя exe файла архиватора
	std::string exeName;
	// ! Аргументы архиватора
	std::string arguments;

	//! Каталог назначения
	std::string targetPath;
	//! Маска имени архива
	std::string targetMask;
	
};

