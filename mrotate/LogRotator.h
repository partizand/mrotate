#pragma once

#include "stdafx.h"
#include "RotateEntry.h"
#include <vector>

#include <Poco\Util\AbstractConfiguration.h>

using namespace std;

//! ����������� ������� �����
class LogRotator
{
public:
	LogRotator(void);
	~LogRotator(void);
	//! �������� �������� ������� �� �����
	void load(const std::string &fileName);

private:
	//! ������ �������
	vector<RotateEntry> Items;
	//! �������� �������� �������
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! �������������� ���������� ������� � int � ����������� ���� �������
	int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! �������������� ������� � int64
	Int64 convertSize(std::string &strSize);
};

