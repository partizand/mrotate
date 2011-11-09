/*
	This file is part of mrotate. Rotate logs for Windows

    Copyright (C) 2011  Andrey Kapustin

    mrotate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    mrotate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	��� ���������������� �� �������� GPL 3
	�����: �������� ������, 2011 �.

*/

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
	bool archiveFile(const std::string &arhiverName,const std::string &fileName,const std::string &arhFileName,const Poco::DateTime &replDate);

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

