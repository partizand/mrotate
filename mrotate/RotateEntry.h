#pragma once

#include "stdafx.h"


namespace Rotate
{
	//enum RotateType{Single,Multiple};
	//! Режим обработки даты файла
	enum DateMode {Created,Modified,Now};
	//! Режим замены даты в параметрах %dd
	//enum DateReplaceMode{Now,Create,Modify};

	Rotate::DateMode dateModeFromString(const std::string &str,Rotate::DateMode defaultMode);
}

//! Одна запись настроек ротации
class RotateEntry
{
public:
	RotateEntry(void);
	RotateEntry(const std::string &Name, const std::string &Source,bool Recurse,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		bool Shift,
		const std::string &targetDir,const std::string &targetMask,
		const std::string &FDateMode,
		const std::string &DateReplaceMode,
		const std::string &PreRotate,
		const std::string &PostRotate
		);
	~RotateEntry(void);
	//! Имя записи
	std::string name;
	//! Источник
	std::string source;
	//! Маска источника
	std::string sourceMask;
	//! Каталог источника
	std::string sourceDir;
	//! Обрабатывать подкаталоги
	bool recurse;

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
	//! Режим сдвига файлов при переименовании
	bool shift;
	//! Каталог назначения
	std::string targetDir;
	//! Маска названия архивного файла назначения
	std::string targetMask;
	//! Каталог назначения и маска
	std::string targetPath;
	//! Режим обрабтоки дат файлов
	Rotate::DateMode dateMode;
	//! Режим замены даты
	Rotate::DateMode dateReplaceMode;
	//! Скрипт выполнения до ротации
	std::string  preRotate;
	//! Скрипт выполнение после ротации
	std::string postRotate;
	
};

