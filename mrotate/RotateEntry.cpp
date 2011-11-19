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

using namespace std;
using namespace Poco;

RotateEntry::RotateEntry(void)
{
}

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
}
