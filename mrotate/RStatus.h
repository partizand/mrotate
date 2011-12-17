#pragma once



#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\AutoPtr.h>
#include <Poco\DateTime.h>

//! ���������/���������� ������ ��������� �������
class RStatus
{
public:
	RStatus(Poco::Logger &logger);
	~RStatus(void);
	//! ���������� ���� ��������� �������
	Poco::Timestamp getDate(const std::string &confName,const std::string &entryName);
	//! ��������� ���� ��������� �������. ���� ���������� �������
	void setDate(const std::string &confName,const std::string &entryName);
	//! ������������� ������� ���������� ������ ������������
	void setStatusFileName(const std::string &fName);
	//! ��������� �������
	void load();
private:
	//! ��� ����� �� ��������� �������s
	std::string statusFileName;
	//! �������� ���� ������������
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> pConf;
	//Poco::Util::AbstractConfiguration *pConf;

	//! ���� �������
	std::map<std::string,Poco::Timestamp> rDates;

	Poco::Logger *log;
};

