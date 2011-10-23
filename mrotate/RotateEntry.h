#pragma once

#include "stdafx.h"


namespace Rotate
{
	//enum RotateType{Single,Multiple};
	//! Режим обработки даты файла
	enum DateMode {Created,Modified};
}

//! Одна запись настроек ротации
class RotateEntry
{
public:
	RotateEntry(void);
	RotateEntry(const std::string &Source,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		const std::string &targetDir,const std::string &targetMask,
		const std::string &FDateMode);
	~RotateEntry(void);

	//! Источник
	std::string source;
	//! Период обработки
	int period;
	//! Ограничение размера
	unsigned long int limitSize;
	//! Имя архиватора, или none
	std::string archiverName;
	//! Тип ротации (много ежедневных файлов, или один)
	//Rotate::RotateType type;
	//! Сколько дней хранить архивы
	int keepPeriod;
	//! Каталог назначения
	std::string targetDir;
	//! Маска названия архивного файла назначения
	std::string targetMask;
	//! Каталог назначения и маска
	std::string targetPath;
	//! Режим обрабтоки дат файлов
	Rotate::DateMode dateMode; 

};

