#pragma once

#include <vector>
#include <map>

#include <Poco\Logger.h>
#include <Poco\Util\AbstractConfiguration.h>

#include "ArchiverParam.h"
//! ���������� ��������� ������ ������� �����������
class Archiver
{
public:
	Archiver(Poco::Logger &logger);
	~Archiver(void);
	//! ���������� ��������� (��������� �� ���� �������)
	bool setOptions(std::string ArchiveName,std::string TargetPath);
	
	//! �������������� ���� ����������� � �������� �����������
	bool archiveFile(std::string FileName);

	//! �������� �������� �� �����
	void load(const std::string &fileName);
	//! �������� �������� 
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! ���������� ����� ������� (�������� ���������)
	void setDebugMode();
	//! ���������� ���������� �����, ��� �������� �������������� ����������
	std::string getArhExtention(std::string ArchiveName="");
private:
	//! ����������
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;
	//! ���������� ����� �������
	bool _debugMode;

	//! ��� ����������
	std::string archiveName;
	//! ������� ����������
	std::string targetPath;
	//! ������
	Poco::Logger *log;
	
};

