#include "StdAfx.h"
#include "ArchiverParam.h"


ArchiverParam::ArchiverParam(void)
{
}

ArchiverParam::ArchiverParam(std::string ArchiverName,std::string Extention,std::string ExeName,std::string Arguments):
	archiverName(ArchiverName),
	extention(Extention),
	exeName(ExeName),
	arguments(Arguments)
{

}

ArchiverParam::~ArchiverParam(void)
{
}
