#pragma once

#include "stdafx.h"

namespace Rotate
{
	enum RotateType{Single,Multiple};
}

//! Одна запись настроек ротации
class RotateEntry
{
public:
	RotateEntry(void);
	~RotateEntry(void);
	//! Источник
	std::string source;
	//! Период обработки
	int period;
	//! Тип ротации (много ежедневных файлов, или один)
	Rotate::RotateType type;
	//! Сколько дней хранить архивы
	int keepPeriod;
	//! Имя архиватора, или none
	std::string archiver;
	//! Каталог назначения
	std::string target;

};

