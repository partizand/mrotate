#include "StdAfx.h"
#include "ArchiverParam.h"

#include "Executer.h"

ArchiverParam::ArchiverParam(void)
{
}

ArchiverParam::ArchiverParam(std::string ArchiverName,std::string Extention,std::string ExeName,std::string Arguments):
	archiverName(ArchiverName),
	extension(Extention),
	exeName(ExeName),
	arguments(Arguments)
{
	if (!ExeName.empty())
	{
	bool found=Executer::getFullPath(ExeName,fullExeName);
	if (!found) fullExeName.clear();
	}
}

ArchiverParam::~ArchiverParam(void)
{
}
