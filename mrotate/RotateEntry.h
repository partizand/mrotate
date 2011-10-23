#pragma once

#include "stdafx.h"


namespace Rotate
{
	//enum RotateType{Single,Multiple};
	//! ����� ��������� ���� �����
	enum DateMode {Created,Modified};
}

//! ���� ������ �������� �������
class RotateEntry
{
public:
	RotateEntry(void);
	RotateEntry(const std::string &Source,int Period,unsigned long int LimitSize,
		const std::string &ArchiverName,int KeepPeriod,
		const std::string &targetDir,const std::string &targetMask,
		const std::string &FDateMode);
	~RotateEntry(void);

	//! ��������
	std::string source;
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

};

