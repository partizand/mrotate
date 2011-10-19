#pragma once

#include "stdafx.h"
#include "RotateEntry.h"
#include <vector>

#include <Poco\Util\AbstractConfiguration.h>

//using namespace std;

//! ����������� ������� �����
class LogRotator
{
public:
	LogRotator(void);
	~LogRotator(void);
	//! �������� �������� ������� �� �����
	void load(const std::string &fileName);
	//! ���������� ����� (�������� �������)
	void rotate();

private:
	//! ������ �������
	vector<RotateEntry> items;
	//! ������ ������� �������������� ������ ������� items
	int currIndex;
	//! �������� �������� �������
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! �������� ������ ������ ��� ���������
	void getFileList(std::vector<std::string> &fileList);
	//! ��������� ����� �� ���������� ������ ����
	bool isRotateFile(const std::string &fileName);
	//! ���������� �������� ����
	void rotateFile(const std::string &fileName);

	//! �������������� ���������� ������� � int � ����������� ���� �������
	//int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! �������������� ������� � int64
	long int convertSize(std::string &strSize);
};

