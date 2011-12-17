#pragma once



#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\AutoPtr.h>

//! Сохраняет/возвращает статус последней ротации
class RStatus
{
public:
	RStatus(Poco::Logger &logger);
	~RStatus(void);
	//! Устанавливает каталог сохранения файлов конфигураций
	void setStatusFileName(const std::string &fName);
	//! Загрузить статусы
	void load();
private:
	//! Каталог сохранения файлов конфигураций
	std::string statusFileName;
	//! Открытый файл конфигурации
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> pConf;
	//Poco::Util::AbstractConfiguration *pConf;

	//! Даты ротаций
	std::map<std::string,Poco::Timestamp> rDates;

	Poco::Logger *log;
};

