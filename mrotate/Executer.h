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

#include <Poco\Logger.h>

//! ������ ������� ��������
class Executer
{
public:
	Executer(void);
	~Executer(void);
	//! ��������� ������� ����� � ����� ����������, ���������� ��� ��������
	static int execute(const std::string &exePath,const std::vector<std::string> &vectArgs,Poco::Logger &log,bool wait=true);
	//! �������� ������ ��� ����� (���� � %path% ���������), ���� ���� ��� ������, �� ������ ������ ��� ��
	static bool getFullPath(const std::string &aPath, std::string &fullPath);
	
	//! ������� ������� �� ����� �����
	static void createDir(const std::string &fileName);
	
	//! ������� ������ ���������� �� ������ �� ����������� "������"
	static void splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs);

	//! �������������/����������� ����
	static bool moveFile(const std::string &srcFile,const std::string &dstFile,bool debug, Poco::Logger &log);
};

