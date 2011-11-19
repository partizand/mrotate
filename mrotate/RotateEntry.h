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


namespace Rotate
{
	//enum RotateType{Single,Multiple};
	//! Режим обработки даты файла
	enum DateMode {Created,Modified,Last,Now};
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

