#pragma once



#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\AutoPtr.h>

//! ���������/���������� ������ ��������� �������
class RStatus
{
public:
	RStatus(Poco::Logger &logger);
	~RStatus(void);
	//! ������������� ������� ���������� ������ ������������
	void setStatusFileName(const std::string &fName);
	//! ��������� �������
	void load();
private:
	//! ������� ���������� ������ ������������
	std::string statusFileName;
	//! �������� ���� ������������
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> pConf;
	//Poco::Util::AbstractConfiguration *pConf;

	//! ���� �������
	std::map<std::string,Poco::Timestamp> rDates;

	Poco::Logger *log;
};

