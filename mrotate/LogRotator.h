#pragma once

#include "stdafx.h"

#include <vector>

#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\Logger.h>

#include "RotateEntry.h"
#include "Archiver.h"

//! ����������� ������� �����
class LogRotator
{
public:
	LogRotator(Poco::Logger *logger);
	~LogRotator(void);
	//! �������� �������� ������� �� �����
	void load(const std::string &fileName);
	//! ���������� ����� (�������� �������)
	void rotate();

private:
	
	//! ������ �������
	std::vector <RotateEntry> items;
	//! ������ ������� �������������� ������ ������� items
	int currIndex;
	//! ���������
	Archiver archiver;
	//! �������� �������� �������
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! �������� ������ ������ ��� ���������
	void getFileList(std::vector<std::string> &fileList);
	//! ��������� ����� �� ���������� ������ ����
	bool isRotateFile(const std::string &fileName);
	//! ���������� �������� ����
	void rotateFile(const std::string &fileName);
	//! ���������� ������ ������
	void rotateFile(const std::vector<std::string> &listFiles);

	//! �������������� ���������� ������� � int � ����������� ���� �������
	//int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! �������������� ������� � int64
	long int convertSize(std::string &strSize);
	Poco::Logger *log;
};

