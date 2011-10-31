#pragma once

#include "stdafx.h"


namespace Rotate
{
	//enum RotateType{Single,Multiple};
	//! ����� ��������� ���� �����
	enum DateMode {Created,Modified,Now};
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
		const std::string &targetDir,const std::string &targetMask,
		const std::string &FDateMode,
		const std::string &DateReplaceMode
		);
	~RotateEntry(void);
	//! ��� ������
	std::string name;
	//! ��������
	std::string source;
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


};

