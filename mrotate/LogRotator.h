#pragma once

#include "stdafx.h"
#include "RotateEntry.h"
#include <vector>

#include <Poco\Util\AbstractConfiguration.h>

using namespace std;

//! Осущствляет ротацию логов
class LogRotator
{
public:
	LogRotator(void);
	~LogRotator(void);
	//! Загрузка настроек ротации из файла
	void load(const std::string &fileName);

private:
	//! Записи ротации
	vector<RotateEntry> Items;
	//! Загрузка настроек ротации
	void load(const Poco::Util::AbstractConfiguration *pConf);
	//! Преобразование строкового периода в int и определение типа ротации
	int convertPeriod(std::string &strPeriod,Rotate::RotateType &rType);
	//! Преобразование размера в int64
	Int64 convertSize(std::string &strSize);
};

