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
int Executer::execute(const std::string &exePath,const std::vector<std::string> &vectArgs,bool wait)
{
	// Проверяем, что путь абсолютный
	string fullPath=getFullPath(exePath);
	int ret=0;
	ProcessHandle handle=Process::launch(fullPath,vectArgs);// Стартуем
	
	if (wait) // Ждем
	{
		ret=handle.wait();
	}
	return ret;
}
//------------------------------------------------------------------------
//! Получить полное имя файла (ищет в %path% каталогах), если путь уже полный, то просто вернет его же
std::string Executer::getFullPath(std::string aPath)
{
// Проверяем, что путь абсолютный
	Path pPath(aPath);
	if (pPath.depth()==0) // передано только имя файла
	{
		// Ищем полный путь
		string EnvPath(Environment::get("PATH")); // Переменная окружения Path
		EnvPath=Path::expand(EnvPath); // Раскрываем всякие %Dir%
		string fileName=pPath.getFileName();
		Path::find(EnvPath,fileName,pPath); 
		
	}
	return pPath.toString();
}

//------------------------------------------------------------------------
//! Разбить строку аргументов на вектор по разделителю "пробел"
void Executer::splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs)
{
	StringTokenizer tok(strArgs," ",StringTokenizer::TOK_IGNORE_EMPTY); // Разбивает на части
	vectArgs.assign(tok.begin(),tok.end()); // Копирует
}