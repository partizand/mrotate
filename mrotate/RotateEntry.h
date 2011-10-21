#pragma once

#include "stdafx.h"

/*
namespace Rotate
{
	enum RotateType{Single,Multiple};
}
*/
//! ���� ������ �������� �������
class RotateEntry
{
public:
	RotateEntry(void);
	~RotateEntry(void);
	//! ��������
	std::string source;
	//! ������ ���������
	int period;
	//! ��� ������� (����� ���������� ������, ��� ����)
	//Rotate::RotateType type;
	//! ������� ���� ������� ������
	int keepPeriod;
	//! ��� ����������, ��� none
	std::string archiverName;
	//! ������� ���������� � �����
	std::string targetPath;
	//! ������� ����������
	std::string targetDir;
	//! ����������� �������
	long int limitSize;

};

