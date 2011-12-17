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

#include "StdAfx.h"
#include "RotateEntry.h"

#include <Poco\Path.h>
#include <Poco\String.h>
#include <Poco\NumberParser.h>

using namespace std;
using namespace Poco;
/*
RotateEntry::RotateEntry(void)
{
	recurse=false;
	period=0;
	limitSize=0;
	keepPeriod=0;
	shift=false;
	preRotate="";
	postRotate="";
	targetMask="%FileName";
	dateMode=Rotate::Last;
	dateReplaceMode=Rotate::Now;
}
*/
//! Создание записи ротации по минимуму, остальное по умолчанию
RotateEntry::RotateEntry(const std::string &ConfName,
				const std::string &Name, 
				const std::string &Source,
				const std::string &ArchiverName):
	confName(ConfName),
	name(Name),
	source(Source),
	archiverName(ArchiverName)
	
{
	// Вычисляемые значения. Маска файлов источника
	Path sPath(source);
	sPath.makeFile();
	sourceMask=sPath.getFileName();
	// Вычисляемые значения. Каталог источника
	sPath.setFileName("");
	sourceDir=sPath.toString();
	
	// Значения по умолчанию
	recurse=false;
	period=0;
	limitSize=0;
	keepPeriod=0;
	shift=false;
	preRotate="";
	postRotate="";
	setTarget("","");
	//targetMask="%FileName";
	dateMode=Rotate::Last;
	dateReplaceMode=Rotate::Now;
}
//! Установить период ротации 
void RotateEntry::setPeriod(const std::string &Period)
{
	if (shift)
	{
		periodMode=Rotate::periodModeFromString(Period);
		period=0;
	}
	else
	{
		int iPeriod=0;
		NumberParser::tryParse(Period,iPeriod);
		period=iPeriod;
	}
}
//! Установить размер
void RotateEntry::setSize(std::string &lSize)
{
	limitSize=Rotate::convertSize(lSize);
}
//! Установить приемник
void RotateEntry::setTarget(const std::string &TargetDir,const std::string &TargetMask)
{
	targetDir=TargetDir;
	targetMask=TargetMask;
	// Заполнение target если оно пусто
	if (targetMask.empty())
	{
		if (shift)
		{
		targetMask="%FileName.%Index";
		}
		else
		{
		targetMask="%FileName";
		}

	}
	
	if (targetDir.empty())
	{
		targetDir=sourceDir;
	}
	Path tPath(targetDir);
	tPath.makeDirectory();
	tPath.setFileName(targetMask);
	targetPath=tPath.toString();
}
//! Установить режим даты
void RotateEntry::setDateMode(const std::string &DateMode)
{
dateMode=Rotate::dateModeFromString(DateMode,Rotate::Last); // Дата создания по умолчанию
}
	//! Установить режим замены даты
void RotateEntry::setDateReplaceMode(const std::string &DateReplaceMode)
{
dateReplaceMode=Rotate::dateModeFromString(DateReplaceMode,Rotate::Now); // Текущая дата по умолчанию
}
/*
RotateEntry::RotateEntry(const std::string &Name,const std::string &Source,bool Recurse,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		bool Shift,
		const std::string &TargetDir,const std::string &TargetMask,
		const std::string &FDateMode,
		const std::string &DateReplaceMode,
		const std::string &PreRotate,
		const std::string &PostRotate):
	name(Name),
	source(Source),
	recurse(Recurse),
	period(Period),
	limitSize(LimitSize),
	archiverName(ArchiverName),
	keepPeriod(KeepPeriod),
	shift(Shift),
	targetDir(TargetDir),
	targetMask(TargetMask),
	preRotate(PreRotate),
	postRotate(PostRotate)
{
	// Маска файлов источника
	Path sPath(source);
	sPath.makeFile();
	sourceMask=sPath.getFileName();
	// Каталог источника
	sPath.setFileName("");
	sourceDir=sPath.toString();
	
	// Заполнение target если оно пусто
	if (targetMask.empty())
	{
		if (Shift)
		{
		targetMask="%FileName.%Index";
		}
		else
		{
		targetMask="%FileName";
		}

	}
	
	if (targetDir.empty())
	{
		//Path sPath(source);
		//sPath.makeFile();
		//sPath.setFileName("");
		targetDir=sourceDir;
	}
	Path tPath(targetDir);
	tPath.makeDirectory();
	tPath.setFileName(targetMask);
	targetPath=tPath.toString();
	dateMode=Rotate::dateModeFromString(FDateMode,Rotate::Last); // Дата создания по умолчанию
	
	dateReplaceMode=Rotate::dateModeFromString(DateReplaceMode,Rotate::Now); // Текущая дата по умолчанию
	
}
	*/
RotateEntry::~RotateEntry(void)
{
}

namespace Rotate
{
//! Преобразовывает строку в тип DateMode
Rotate::DateMode dateModeFromString(const std::string &str,Rotate::DateMode defaultMode)
{
	Rotate::DateMode ret=defaultMode;
	if (!str.empty())
	{
		if (icompare(str,0,1,"M")==0)
			{
				ret=Rotate::Modified;
			}
		else if (icompare(str,0,1,"C")==0)
			{
				ret=Rotate::Created;
			}
		else if (icompare(str,0,1,"N")==0)
			{
				ret=Rotate::Now;
			}
		else if (icompare(str,0,1,"L")==0)
			{
				ret=Rotate::Last;
			}
	}
	return ret;
	
}

//! Получение периода ротации из строки
Rotate::PeriodMode periodModeFromString(const std::string &str)
{
	Rotate::PeriodMode ret=Rotate::None;
	if (!str.empty())
	{
		if (icompare(str,0,1,"D")==0)
			{
				ret=Rotate::Daily;
			}
		else if (icompare(str,0,1,"W")==0)
			{
				ret=Rotate::Weekly;
			}
		else if (icompare(str,0,1,"M")==0)
			{
				ret=Rotate::Monthly;
			}
		
	}
	return ret;
}
//------------------------------------------------------------------------
//! Преобразование размера в int64 (Т.е. строка может содержать K и M)
Poco::Int64 convertSize(std::string &strSize)
{
	if (strSize.empty()) return 0;
	Int64 iSize;
	Int64 multipl=1;
	// Преобразуем строку в число
	toUpperInPlace(strSize);
	string::size_type iK=strSize.find("K");
	string::size_type iM=strSize.find("M");
	if (iK!=string::npos) // Найден K
	{
		multipl=1024;
		strSize.erase(iK,1);
	}
	if (iM!=string::npos) // Найден M
	{
		multipl=1024*1024;
		strSize.erase(iM,1);
	}
	if (NumberParser::tryParse64(strSize,iSize))
	{
		return iSize*multipl;
	}
	else
	{
		//log->error("Error parsing size in entry");
		return 0;
	}
}

}
