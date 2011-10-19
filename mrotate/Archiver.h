#pragma once

#include "ArchiverParam.h"
#include <vector>
#include <map>
//! ���������� ��������� ������ ������� �����������
class Archiver
{
public:
	Archiver(void);
	~Archiver(void);
	//! ���������� ��������� (��������� �� ���� �������)
	void setOptions(std::string ArchiveName,std::string TargetPath);
	
	//! �������������� ���� ����������� � �������� �����������
	bool archiveFile(std::string FileName);
private:
	//! ����������
	//std::vector<ArchiverParam> Archivers;
	std::map<std::string,ArchiverParam> Archivers;
	//! ��������� ������� ����� � ����� ����������, ���������� �����
	bool startProg(std::string exeName,std::string args);

	//! ��� ����������
	std::string archiveName;
	//! ��� exe ����� ����������
	std::string exeName;
	// ! ��������� ����������
	std::string arguments;

	//! ������� ����������
	std::string targetPath;
	//! ����� ����� ������
	std::string targetMask;
	
};

