#pragma once

#include <Poco/Process.h>

//! Запуск внешних программ
class Executer
{
public:
	Executer(void);
	~Executer(void);
	//! Запустить внешнюю прогу и ждать завершения, возвращает код возврата
	static int execute(std::string exePath,Poco::Process::Args &vectArgs,bool wait=true);
	//! Разбить строку аргументов на вектор по разделителю "пробел"
	static void splitArgs(const std::string &strArgs,Poco::Process::Args &vectArgs);
};

