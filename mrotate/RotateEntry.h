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

#include <Poco\Poco.h>


namespace Rotate
{
	const int Daily=1;
	const int Weekly=7;
	const int Monthly=30;
	//! ����� ��������� ���� �����
	enum DateMode {Created,Modified,Last,First,Now};
	//! ����� ������� ��� shift	
	//enum PeriodMode {None,Daily,Weekly,Monthly};
	//! ��������� ������ ��������� ���� �� ������
	Rotate::DateMode dateModeFromString(const std::string &str,Rotate::DateMode defaultMode);
	//! ��������� ������� ������� �� ������
	//int periodModeFromString(const std::string &str);
	//! �������������� ������� � int64 (�.�. ������ ����� ��������� K � M)
	Poco::Int64 convertSize(std::string &strSize);
}

//! ���� ������ �������� �������
class RotateEntry
{
public:
	//RotateEntry(void);
	//! �������� ������ ������� �� ��������, ��������� �� ���������
	RotateEntry(const std::string &ConfName,
				const std::string &Name, 
				const std::string &Source,
				const std::string &ArchiverName);
	//! �������� ������ ������� �� ����� �����������
	/*
	RotateEntry(const std::string &Name, const std::string &Source,bool Recurse,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		bool Shift,
		const std::string &targetDir,const std::string &targetMask,
		const std::string &FDateMode,
		const std::string &DateReplaceMode,
		const std::string &PreRotate,
		const std::string &PostRotate
		);
	*/
	~RotateEntry(void);
	//! ���������� ������ (� ����������� �� Shift)
	void setPeriod(const std::string &Period);
	//! ���������� ������
	void setSize(std::string &lSize);
	//! ���������� ��������
	void setTarget(const std::string &TargetDir,const std::string &TargetMask);
	//! ���������� ����� ����
	void setDateMode(const std::string &DateMode);
	//! ���������� ����� ������ ����
	void setDateReplaceMode(const std::string &DateReplaceMode);
	//! ��� ������
	std::string name;
	//! ��������
	std::string source;
	//! ����� ���������
	std::string sourceMask;
	//! ������� ���������
	std::string sourceDir;
	//! ������������ �����������
	bool recurse;

	//! ������ ���������
	int period;
	//! ����������� �������
	Poco::Int64 limitSize;
	//! ��� ����������, ��� none
	std::string archiverName;
	//! ��� ������� (����� ���������� ������, ��� ����)
	//Rotate::RotateType type;
	//! ������� ���� ������� ������
	int keepPeriod;
	//! ����� ������ ������ ��� ��������������
	bool shift;
	//! ������� ����������
	std::string targetDir;
	//! ����� �������� ��������� ����� ����������
	std::string targetMask;
	//! ������� ���������� � �����
	std::string targetPath;
	//! ����� ��������� ��� ������
	Rotate::DateMode dateMode;
	//! ����� ������ ����
	Rotate::DateMode dateReplaceMode;
	//! ������ ���������� �� �������
	std::string  preRotate;
	//! ������ ���������� ����� �������
	std::string postRotate;
	//! ����� ������� ��� shift
	//Rotate::PeriodMode periodMode;
	//! ��� ����� � ����������, ��� ����������
	std::string confName;
	
};

