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

#include "stdafx.h"

#include <vector>

#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\Logger.h>
#include <Poco\Path.h>
#include <Poco\File.h>
#include <Poco\DateTime.h>

#include "RotateEntry.h"
#include "Archiver.h"
#include "RStatus.h"

//! Осущствляет ротацию логов
class LogRotator
{
public:
	LogRotator(Poco::Logger &logger);
	~LogRotator(void);
	//! Загрузка настроек ротации из файла
	void load(const std::string &fileName);
	//! Загрузка настроек ротации
	void load(const Poco::Util::AbstractConfiguration *pConf,const std::string &confName="mrotate");
	//! Ротировать файлы (основная функция)
	void rotate();
	//! Проверка загруженных записей ротации на ошибки
	bool check();
	//! Установить режим отладки (эмуляция ротации)
	void setDebugMode();
	//! Установить режим force
	void setForceMode();
	//! Устанавливает имя файла статусов ротаций
	void setStatusFileName(const std::string &fileName);

	//! Возвращает версию
	std::string getVersion();
	//! Архиватор
	Archiver archiver;

private:
	
	//! Записи ротации
	std::vector <RotateEntry> items;
	//! индекс текущей обрабатываемой записи ротации items
	int currIndex;
	//! Режим отладки включен
	bool _debugMode;
	//! Режим force
	bool _force;
	//! Получить список файлов для обработки
	//void getFileList(std::vector<std::string> &fileList);
	//! Получить список файлов по маске, отобранных по периоду или размеру, если period=0 и lSize=0 возвращаются все файлы по маске
	//void getFileList(std::vector<std::string> &fileList, const std::string &pathMask, int Period=0,unsigned long int lSize=0);
	//void getFileList(std::vector<std::string> &fileList,const Poco::Path &pathMask,bool recurse, int Period=0,unsigned long int lSize=0);
	
	void rotateFiles(const std::string &fileMask,const Poco::Path &pSourceDir,const Poco::Path pDestDir,bool recurse,bool rotate,int Period=0,Poco::Int64 lSize=0);
	//! Проверить нужно ли ротировать данный файл, если period и lSize не заданы, файл нужно ротировать
	bool isRotateFile(const std::string &fileName,int Period=0,Poco::Int64 lSize=0);
	//! Проверить нужно ли ротировать данный файл, если period и lSize не заданы, файл нужно ротировать
	bool isRotateFile(Poco::File &pFileName,int Period=0,Poco::Int64 lSize=0);
	//! Ротировать заданный файл
	void rotateFile(const Poco::File &pFile,const Poco::Path &destDir);
	
	//! Удалить файл
	void removeFile(const std::string &fileName);
	//! Удалить файл
	void removeFile(const Poco::File &pFile);
	

	//! Сдвинуть старые файлы заданного файла
	void shiftFile(const std::string &srcFile,const std::string &destDir);
	//! Возращает индекс файла (из конструкции test.log.2.7z -> вернет 2)
	int getIndexOfFile(const std::string &baseFileName,const std::string &fileName);

	//! Возвращает маску для удаляемых файлов для текущего задания
	std::string getRemoveMask();

	//! Удалить список файлов
	void removeFile(const std::vector<std::string> &listFiles);
	//! Возвращает дату в зависимости от режима dateMode
	Poco::DateTime getDate(const Poco::File &pFile,Rotate::DateMode dateMode);
	
	//! Выполнить скрипт до/после ротации
	void executeScript(const std::string &script);
	
	//! Определят нужна ли ротация при режиме shift для текущей записи
	bool isNeedRotate();

	//! Преобразование строки в int64
	//unsigned long int convertSize(std::string &strSize);
	//! Преобразовать target к полному пути
	//std::string getFullTarget(const std::string &targetPath,const std::string &Source);

	//! Сохранение дат ротаций
	RStatus rstatus;

	Poco::Logger *log;
};

