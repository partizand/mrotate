#pragma once

#include "stdafx.h"

#include <vector>

#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\Logger.h>

#include "RotateEntry.h"
#include "Archiver.h"

//! Осущствляет ротацию логов
class LogRotator
{
public:
	LogRotator(Poco::Logger *logger);
	~LogRotator(void);
	//! Загрузка настроек ротации из файла
	void load(const std::string &fileName);
	//! Ротировать файлы (основная функция)
	void rotate();

private:
	
	//! Записи ротации
	std::vector <RotateEntry> items;
	//! индекс текущей обрабатываемой записи ротации items
	int currIndex;
	//! Архиватор
	Archiver archiver;
	//! Загрузка настроек ротации
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! Получить список файлов для обработки
	void getFileList(std::vector<std::string> &fileList);
	//! Проверить нужно ли ротировать данный файл
	bool isRotateFile(const std::string &fileName);
	//! Ротировать заданный файл
	void rotateFile(const std::string &fileName);
	//! Ротировать список файлов
	void rotateFile(const std::vector<std::string> &listFiles);

	//! Преобразование строкового периода в int и определение типа ротации
	//int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! Преобразование размера в int64
	long int convertSize(std::string &strSize);
	Poco::Logger *log;
};

