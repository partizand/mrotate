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

#include "stdafx.h"

#include <vector>

#include <Poco\Util\AbstractConfiguration.h>
#include <Poco\Logger.h>
#include <Poco\Path.h>
#include <Poco\File.h>
#include <Poco\DateTime.h>

#include "RotateEntry.h"
#include "Archiver.h"
#include "RStatus.h"

//! ����������� ������� �����
class LogRotator
{
public:
	LogRotator(Poco::Logger &logger);
	~LogRotator(void);
	//! �������� �������� ������� �� �����
	void load(const std::string &fileName);
	//! �������� �������� �������
	void load(const Poco::Util::AbstractConfiguration *pConf,const std::string &confName="mrotate");
	//! ���������� ����� (�������� �������)
	void rotate();
	//! �������� ����������� ������� ������� �� ������
	bool check();
	//! ���������� ����� ������� (�������� �������)
	void setDebugMode();
	//! ���������� ����� force
	void setForceMode();
	//! ������������� ��� ����� �������� �������
	void setStatusFileName(const std::string &fileName);

	//! ���������� ������
	std::string getVersion();
	//! ���������
	Archiver archiver;

private:
	
	//! ������ �������
	std::vector <RotateEntry> items;
	//! ������ ������� �������������� ������ ������� items
	int currIndex;
	//! ����� ������� �������
	bool _debugMode;
	//! ����� force
	bool _force;
	//! �������� ������ ������ ��� ���������
	//void getFileList(std::vector<std::string> &fileList);
	//! �������� ������ ������ �� �����, ���������� �� ������� ��� �������, ���� period=0 � lSize=0 ������������ ��� ����� �� �����
	//void getFileList(std::vector<std::string> &fileList, const std::string &pathMask, int Period=0,unsigned long int lSize=0);
	//void getFileList(std::vector<std::string> &fileList,const Poco::Path &pathMask,bool recurse, int Period=0,unsigned long int lSize=0);
	
	void rotateFiles(const std::string &fileMask,const Poco::Path &pSourceDir,const Poco::Path pDestDir,bool recurse,bool rotate,int Period=0,Poco::Int64 lSize=0);
	//! ��������� ����� �� ���������� ������ ����, ���� period � lSize �� ������, ���� ����� ����������
	bool isRotateFile(const std::string &fileName,int Period=0,Poco::Int64 lSize=0);
	//! ��������� ����� �� ���������� ������ ����, ���� period � lSize �� ������, ���� ����� ����������
	bool isRotateFile(Poco::File &pFileName,int Period=0,Poco::Int64 lSize=0);
	//! ���������� �������� ����
	void rotateFile(const Poco::File &pFile,const Poco::Path &destDir);
	
	//! ������� ����
	void removeFile(const std::string &fileName);
	//! ������� ����
	void removeFile(const Poco::File &pFile);
	

	//! �������� ������ ����� ��������� �����
	void shiftFile(const std::string &srcFile,const std::string &destDir);
	//! ��������� ������ ����� (�� ����������� test.log.2.7z -> ������ 2)
	int getIndexOfFile(const std::string &baseFileName,const std::string &fileName);

	//! ���������� ����� ��� ��������� ������ ��� �������� �������
	std::string getRemoveMask();

	//! ������� ������ ������
	void removeFile(const std::vector<std::string> &listFiles);
	//! ���������� ���� � ����������� �� ������ dateMode
	Poco::DateTime getDate(const Poco::File &pFile,Rotate::DateMode dateMode);
	
	//! ��������� ������ ��/����� �������
	void executeScript(const std::string &script);
	
	//! ��������� ����� �� ������� ��� ������ shift ��� ������� ������
	bool isNeedRotate();

	//! �������������� ������ � int64
	//unsigned long int convertSize(std::string &strSize);
	//! ������������� target � ������� ����
	//std::string getFullTarget(const std::string &targetPath,const std::string &Source);

	//! ���������� ��� �������
	RStatus rstatus;

	Poco::Logger *log;
};

