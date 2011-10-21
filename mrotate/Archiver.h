#pragma once

#include <vector>
#include <map>

#include "ArchiverParam.h"
//! ���������� ��������� ������ ������� �����������
class Archiver
{
public:
	Archiver(void);
	~Archiver(void);
	//! ���������� ��������� (��������� �� ���� �������)
	bool setOptions(std::string ArchiveName,std::string TargetPath);
	
	//! �������������� ���� ����������� � �������� �����������
	bool archiveFile(std::string FileName);
	//! ���������� ���������� �����, ��� �������� �������������� ����������
	std::string getArhExtention(std::string ArchiveName="");
private:
	//! ����������
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;
	

	//! ��� ����������
	std::string archiveName;
	//! ������� ����������
	std::string targetPath;
	
	
};

