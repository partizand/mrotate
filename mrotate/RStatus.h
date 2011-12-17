#pragma once



#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\DateTime.h>

//! Сохраняет/возвращает статус последней ротации
class RStatus
{
public:
	RStatus(Poco::Logger &logger);
	~RStatus(void);
	//! Возвращает дату последней ротации
	Poco::Timestamp getDate(const std::string &confName,const std::string &entryName);
	//! Сохраняет дату последней ротации. Дата становится текущей
	void setDate(const std::string &confName,const std::string &entryName);
	//! Устанавливает каталог сохранения файлов конфигураций
	void setStatusFileName(const std::string &fName);
	//! Загрузить статусы
	void load();
private:
	//! Имя файла со статусами ротацийs
	std::string statusFileName;
	//! Открытый файл конфигурации
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> pConf;
	//Poco::Util::AbstractConfiguration *pConf;

	//! Даты ротаций
	std::map<std::string,Poco::Timestamp> rDates;

	Poco::Logger *log;
};

