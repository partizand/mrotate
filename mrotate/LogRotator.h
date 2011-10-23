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
	LogRotator(Poco::Logger &logger);
	~LogRotator(void);
	//! �������� �������� ������� �� �����
	void load(const std::string &fileName);
	//! �������� �������� �������
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! ���������� ����� (�������� �������)
	void rotate();

private:
	
	//! ������ �������
	std::vector <RotateEntry> items;
	//! ������ ������� �������������� ������ ������� items
	int currIndex;
	//! ���������
	Archiver archiver;
	
	//! �������� ������ ������ ��� ���������
	//void getFileList(std::vector<std::string> &fileList);
	//! �������� ������ ������ �� �����, ���������� �� ������� ��� �������, ���� period � lSize �� ������, ������� �� ��������� ������� �������
	void getFileList(std::vector<std::string> &fileList, const std::string &pathMask,int Period=0,unsigned long int lSize=0);
	//! ��������� ����� �� ���������� ������ ����, ���� period � lSize �� ������, ������� �� ��������� ������� �������
	bool isRotateFile(const std::string &fileName,int Period=0,unsigned long int lSize=0);
	//! ���������� �������� ����
	void rotateFile(const std::string &fileName);
	//! ���������� ������ ������
	void rotateFile(const std::vector<std::string> &listFiles);
	//! ���������� �������� �����
	//void rotateArhFile(const std::string &fileName);
	//! ���������� ������ �������� ������
	//void rotateArhFile(const std::vector<std::string> &listFiles);
	//! ������� ����
	void removeFile(const std::string &fileName);
	//! ������� ������ ������
	void removeFile(const std::vector<std::string> &listFiles);
	//! �������������� ���������� ������� � int � ����������� ���� �������
	//int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! �������������� ������� � int64
	unsigned long int convertSize(std::string &strSize);
	//! ������������� target � ������� ����
	//std::string getFullTarget(const std::string &targetPath,const std::string &Source);

	Poco::Logger *log;
};

