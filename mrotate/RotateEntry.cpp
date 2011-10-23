#include "StdAfx.h"
#include "RotateEntry.h"

#include <Poco\Path.h>
#include <Poco\String.h>

using namespace std;
using namespace Poco;

RotateEntry::RotateEntry(void)
{
}

RotateEntry::RotateEntry(const std::string &Source,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		const std::string &TargetDir,const std::string &TargetMask,
		const std::string &FDateMode):
	source(Source),
	period(Period),
	limitSize(LimitSize),
	archiverName(ArchiverName),
	keepPeriod(KeepPeriod),
	targetDir(TargetDir),
	targetMask(TargetMask)
{
	// Заполнение target если оно пусто
	if (targetMask.empty())
	{
		targetMask="%FileName";
	}
	if (targetDir.empty())
	{
		Path sPath(source);
		sPath.makeFile();
		sPath.setFileName("");
		targetDir=sPath.toString();
	}
	Path tPath(targetDir);
	tPath.makeDirectory();
	tPath.setFileName(targetMask);
	targetPath=tPath.toString();
	dateMode=Rotate::Created; // Дата создания по умолчанию
	if (icompare(FDateMode,"Modified")==0)
	{
		dateMode=Rotate::Modified;
	}
}
RotateEntry::~RotateEntry(void)
{
}
