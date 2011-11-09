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
