#pragma once

//! Параметры архиватора
class ArchiverParam
{
public:
	ArchiverParam(void);
	ArchiverParam(std::string ArchiverName,std::string Extention,std::string ExeName,std::string Arguments);
	~ArchiverParam(void);
	//! Имя архиватора
	std::string archiverName;
	//! Имя запускаемого файла (7z.exe,rar.exe без пути)
	std::string exeName;
	//! Параметры запуска (без самого .exe)
	std::string arguments;
	//! Расширение файла архива
	std::string extension;

};

