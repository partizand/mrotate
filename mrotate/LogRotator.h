#pragma once

#include "stdafx.h"
#include "RotateEntry.h"
#include <vector>

using namespace std;

//! ����������� ������� �����
class LogRotator
{
public:
	LogRotator(void);
	~LogRotator(void);
	void LoadFromFile(const std::string &fileName);

private:
	//! ������ �������
	vector<RotateEntry> Items;
};

