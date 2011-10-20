#include "StdAfx.h"
#include "Executer.h"

#include <Poco/StringTokenizer.h>
#include <Poco\Path.h>
#include <Poco\Environment.h>
#include <Poco\Process.h>

using namespace std;
using namespace Poco;

Executer::Executer(void)
{
}

//------------------------------------------------------------------------
Executer::~Executer(void)
{
}
//------------------------------------------------------------------------
//! Запустить внешнюю прогу и ждать завершения, возвращает успех
int Executer::execute(std::string exePath,Poco::Process::Args &vectArgs,bool wait=true)
{
	// Проверяем, что путь абсолютный
	Path pExe(exePath);
	if (!pExe.isAbsolute())
	{
		// Ищем полный путь
		string EnvPath(Environment::get("PATH")); // Переменная окружения Path
		EnvPath=Path::expand(EnvPath); // Раскрываем всякие %Dir%
		string fileName=pExe.getFileName();
		bool found=Path::find(EnvPath,fileName,pExe); 
		//if (!found) return false; // Архиватор не найден
	}
	int ret=0;
	ProcessHandle handle=Process::launch(pExe.toString(),vectArgs);// Стартуем
	
	if (wait) // Ждем
	{
		ret=handle.wait();
	}
	return ret;
}
//------------------------------------------------------------------------
//! Разбить строку аргументов на вектор по разделителю "пробел"
void Executer::splitArgs(const std::string &strArgs,Poco::Process::Args &vectArgs)
{
	StringTokenizer tok(strArgs," ",StringTokenizer::TOK_IGNORE_EMPTY); // Разбивает на части
	vectArgs.assign(tok.begin(),tok.end()); // Копирует
}