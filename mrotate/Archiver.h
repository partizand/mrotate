/*
	This file is part of mrotate. Rotate logs for Windows

    Copyright (C) 2011  Andrey Kapustin

    mrotate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    mrotate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Код распространяется по лицензии GPL 3
	Автор: Капустин Андрей, 2011 г.

*/

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
	bool archiveFile(const std::string &arhiverName,const std::string &fileName,const std::string &arhFileName,const Poco::DateTime &replDate);

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

