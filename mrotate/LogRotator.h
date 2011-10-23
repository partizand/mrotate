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
	LogRotator(Poco::Logger &logger);
	~LogRotator(void);
	//! Загрузка настроек ротации из файла
	void load(const std::string &fileName);
	//! Загрузка настроек ротации
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! Ротировать файлы (основная функция)
	void rotate();

private:
	
	//! Записи ротации
	std::vector <RotateEntry> items;
	//! индекс текущей обрабатываемой записи ротации items
	int currIndex;
	//! Архиватор
	Archiver archiver;
	
	//! Получить список файлов для обработки
	//void getFileList(std::vector<std::string> &fileList);
	//! Получить список файлов по маске, отобранных по периоду или размеру, если period и lSize не заданы, берутся из настройки текущей ротации
	void getFileList(std::vector<std::string> &fileList, const std::string &pathMask,int Period=0,unsigned long int lSize=0);
	//! Проверить нужно ли ротировать данный файл, если period и lSize не заданы, берутся из настройки текущей ротации
	bool isRotateFile(const std::string &fileName,int Period=0,unsigned long int lSize=0);
	//! Ротировать заданный файл
	void rotateFile(const std::string &fileName);
	//! Ротировать список файлов
	void rotateFile(const std::vector<std::string> &listFiles);
	//! Ротировать архивные файлы
	//void rotateArhFile(const std::string &fileName);
	//! Ротировать список архивных файлов
	//void rotateArhFile(const std::vector<std::string> &listFiles);
	//! Удалить файл
	void removeFile(const std::string &fileName);
	//! Удалить список файлов
	void removeFile(const std::vector<std::string> &listFiles);
	//! Преобразование строкового периода в int и определение типа ротации
	//int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! Преобразование размера в int64
	unsigned long int convertSize(std::string &strSize);
	//! Преобразовать target к полному пути
	//std::string getFullTarget(const std::string &targetPath,const std::string &Source);

	Poco::Logger *log;
};

