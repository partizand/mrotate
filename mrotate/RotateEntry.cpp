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
		const std::string &TargetDir,const std::string &TargetMask,
		const std::string &FDateMode,
		const std::string &DateReplaceMode):
	name(Name),
	source(Source),
	recurse(Recurse),
	period(Period),
	limitSize(LimitSize),
	archiverName(ArchiverName),
	keepPeriod(KeepPeriod),
	targetDir(TargetDir),
	targetMask(TargetMask)
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
		targetMask="%FileName";
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
	dateMode=Rotate::dateModeFromString(FDateMode,Rotate::Created); // Дата создания по умолчанию
	
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
	}
	return ret;
	
}
}
