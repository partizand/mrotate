#include "StdAfx.h"
#include "ArchiverParam.h"


ArchiverParam::ArchiverParam(void)
{
}
ArchiverParam::ArchiverParam(std::string ArchiverName,std::string ExeName,std::string Arguments):
	archiverName(ArchiverName),
	exeName(ExeName),
	arguments(Arguments)
{

}

ArchiverParam::~ArchiverParam(void)
{
}
