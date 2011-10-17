#pragma once

#include "stdafx.h"
#include "RotateEntry.h"
#include <vector>

using namespace std;

//! Осущствляет ротацию логов
class LogRotator
{
public:
	LogRotator(void);
	~LogRotator(void);
	void LoadFromFile(const std::string &fileName);

private:
	//! Записи ротации
	vector<RotateEntry> Items;
};

