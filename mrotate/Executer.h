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

#include <vector>

#include <Poco\Logger.h>

//! Запуск внешних программ
class Executer
{
public:
	Executer(void);
	~Executer(void);
	//! Запустить внешнюю прогу и ждать завершения, возвращает код возврата
	static int execute(const std::string &exePath,const std::vector<std::string> &vectArgs,Poco::Logger &log,bool wait=true);
	//! Получить полное имя файла (ищет в %path% каталогах), если путь уже полный, то просто вернет его же
	static bool getFullPath(const std::string &aPath, std::string &fullPath);
	
	//! Создать каталог по имени файла
	static void createDir(const std::string &fileName);
	
	//! Разбить строку аргументов на вектор по разделителю "пробел"
	static void splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs);

	//! Переименовать/переместить файл
	static bool moveFile(const std::string &srcFile,const std::string &dstFile,bool debug, Poco::Logger &log);
};

