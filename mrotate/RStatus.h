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

#include <Poco\Logger.h>
//#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\Util\PropertyFileConfiguration.h>
//#include <Poco\AutoPtr.h>
#include <Poco\DateTime.h>

//! Сохраняет/возвращает статус последней ротации
class RStatus
{
public:
	RStatus(Poco::Logger &logger);
	~RStatus(void);
	//! Возвращает дату последней ротации
	Poco::DateTime getDate(const std::string &confName,const std::string &entryName);
	//! Сохраняет дату последней ротации. Дата становится текущей
	void setDate(const std::string &confName,const std::string &entryName);
	//! Устанавливает каталог сохранения файлов конфигураций
	void setStatusFileName(const std::string &fName);
	//! Загрузить статусы
	void load();
	//! Сохранить статусы
	void save();
private:
	//! Имя файла со статусами ротацийs
	std::string statusFileName;
	//! Открытый файл конфигурации
	//Poco::Util::PropertyFileConfiguration *pConf;
	Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConf;
	//Poco::Util::AbstractConfiguration *pConf;

	//! Даты ротаций
	std::map<std::string,Poco::Timestamp> rDates;

	Poco::Logger *log;
};

