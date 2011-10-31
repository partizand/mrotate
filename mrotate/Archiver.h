#pragma once

#include <vector>
#include <map>

#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\DateTime.h>

#include "ArchiverParam.h"
//! ���������� ��������� ������ ������� �����������
class Archiver
{
public:
	Archiver(Poco::Logger &logger);
	~Archiver(void);
	//! ���������� ��������� (��������� �� ���� �������)
	//bool setOptions(const std::string &ArchiveName,const std::string &TargetPath);
	
	//! ��������� ��������� 
	bool isValid(const std::string &archiverName);

	//! �������������� ���� 
	bool archiveFile(const std::string &arhiverName,const Poco::File &pFile,const std::string &arhFileName,const Poco::DateTime &replDate);

	//! �������������� ���� ����������� � �������� �����������
	//bool archiveFile(const std::string &FileName);

	//! �������� �������� �� �����
	void load(const std::string &fileName);
	//! �������� �������� 
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! ���������� ����� ������� (�������� ���������)
	void setDebugMode();
	//! ���������� ���������� ����� ��� ����������
	std::string getExtension(const std::string &ArchiverName);
private:
	//! ����������
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;
	//! ���������� ����� �������
	bool _debugMode;

	//! ��� ����������
	//std::string archiveName;
	//! ������� ����������
	//std::string targetPath;
	//! ������
	Poco::Logger *log;
	
};

