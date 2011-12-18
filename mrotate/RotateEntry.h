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

#include <Poco\Poco.h>


namespace Rotate
{
	const int Daily=1;
	const int Weekly=7;
	const int Monthly=30;
	//! Режим обработки даты файла
	enum DateMode {Created,Modified,Last,First,Now};
	//! Режим ротации при shift	
	//enum PeriodMode {None,Daily,Weekly,Monthly};
	//! Получение режима обработки даты из строки
	Rotate::DateMode dateModeFromString(const std::string &str,Rotate::DateMode defaultMode);
	//! Получение периода ротации из строки
	//int periodModeFromString(const std::string &str);
	//! Преобразование размера в int64 (Т.е. строка может содержать K и M)
	Poco::Int64 convertSize(std::string &strSize);
}

//! Одна запись настроек ротации
class RotateEntry
{
public:
	//RotateEntry(void);
	//! Создание записи ротации по минимуму, остальное по умолчанию
	RotateEntry(const std::string &ConfName,
				const std::string &Name, 
				const std::string &Source,
				const std::string &ArchiverName);
	//! Создание записи ротации со всеми параметрами
	/*
	RotateEntry(const std::string &Name, const std::string &Source,bool Recurse,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		bool Shift,
		const std::string &targetDir,const std::string &targetMask,
		const std::string &FDateMode,
		const std::string &DateReplaceMode,
		const std::string &PreRotate,
		const std::string &PostRotate
		);
	*/
	~RotateEntry(void);
	//! Установить период (в зависимости от Shift)
	void setPeriod(const std::string &Period);
	//! Установить размер
	void setSize(std::string &lSize);
	//! Установить приемник
	void setTarget(const std::string &TargetDir,const std::string &TargetMask);
	//! Установить режим даты
	void setDateMode(const std::string &DateMode);
	//! Установить режим замены даты
	void setDateReplaceMode(const std::string &DateReplaceMode);
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
	Poco::Int64 limitSize;
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
	//! Режим ротации при shift
	//Rotate::PeriodMode periodMode;
	//! Имя файла с настройкой, без расширения
	std::string confName;
	
};

