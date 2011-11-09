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

//! Параметры архиватора
class ArchiverParam
{
public:
	ArchiverParam(void);
	ArchiverParam(std::string ArchiverName,std::string Extention,std::string ExeName,std::string Arguments);
	~ArchiverParam(void);
	//! Имя архиватора
	std::string archiverName;
	//! Имя запускаемого файла (7z.exe,rar.exe без пути)
	std::string exeName;
	//! Полный путь запускаемого файла
	std::string fullExeName;

	//! Параметры запуска (без самого .exe)
	std::string arguments;
	//! Расширение файла архива
	std::string extension;

};

