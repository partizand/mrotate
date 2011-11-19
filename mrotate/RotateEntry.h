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


namespace Rotate
{
	//enum RotateType{Single,Multiple};
	//! ����� ��������� ���� �����
	enum DateMode {Created,Modified,Last,Now};
	//! ����� ������ ���� � ���������� %dd
	//enum DateReplaceMode{Now,Create,Modify};

	Rotate::DateMode dateModeFromString(const std::string &str,Rotate::DateMode defaultMode);
}

//! ���� ������ �������� �������
class RotateEntry
{
public:
	RotateEntry(void);
	RotateEntry(const std::string &Name, const std::string &Source,bool Recurse,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		bool Shift,
		const std::string &targetDir,const std::string &targetMask,
		const std::string &FDateMode,
		const std::string &DateReplaceMode,
		const std::string &PreRotate,
		const std::string &PostRotate
		);
	~RotateEntry(void);
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
	unsigned long int limitSize;
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
	
};

