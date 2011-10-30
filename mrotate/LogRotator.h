#pragma once

#include "stdafx.h"

#include <vector>

#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\Logger.h>
#include <Poco\Path.h>
#include <Poco\File.h>

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
	//! Проверка загруженных записей ротации на ошибки
	bool check();
	//! Установить режим отладки (эмуляция ротации)
	void setDebugMode();
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
	
	//! Получить список файлов для обработки
	//void getFileList(std::vector<std::string> &fileList);
	//! Получить список файлов по маске, отобранных по периоду или размеру, если period=0 и lSize=0 возвращаются все файлы по маске
	//void getFileList(std::vector<std::string> &fileList, const std::string &pathMask, int Period=0,unsigned long int lSize=0);
	void getFileList(std::vector<std::string> &fileList,const Poco::Path &pathMask,bool recurse, int Period=0,unsigned long int lSize=0);
	
	void rotateFiles(const std::string &fileMask,const Poco::Path &pSourceDir,const Poco::Path pDestDir,bool recurse,bool rotate,int Period=0,unsigned long int lSize=0);
	//! Проверить нужно ли ротировать данный файл, если period и lSize не заданы, файл нужно ротировать
	bool isRotateFile(const std::string &fileName,int Period=0,unsigned long int lSize=0);
	//! Проверить нужно ли ротировать данный файл, если period и lSize не заданы, файл нужно ротировать
	bool isRotateFile(Poco::File &pFileName,int Period=0,unsigned long int lSize=0);
	//! Ротировать заданный файл
	void rotateFile(const Poco::File &pFile,const Poco::Path &destDir);
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
	//! Удалить файл
	void removeFile(Poco::File &pFile);
	//! Удалить список файлов
	void removeFile(const std::vector<std::string> &listFiles);
	//! Создать каталог, если он не существует
	void createDir(const std::string &dirName);

	//! Преобразование строкового периода в int и определение типа ротации
	//int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! Преобразование размера в int64
	unsigned long int convertSize(std::string &strSize);
	//! Преобразовать target к полному пути
	//std::string getFullTarget(const std::string &targetPath,const std::string &Source);

	Poco::Logger *log;
};

