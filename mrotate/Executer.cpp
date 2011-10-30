#include "StdAfx.h"
#include "Executer.h"

#include <Poco/StringTokenizer.h>
#include <Poco\Path.h>
#include <Poco\File.h>
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
	string fullPath;
	getFullPath(exePath,fullPath);
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
bool Executer::getFullPath(const std::string &aPath, std::string &fullPath)
{
// Проверяем, что путь абсолютный
	bool found;
	Path pPath(aPath);
	pPath.makeFile();
	if (pPath.depth()==0) // передано только имя файла
	{
		// Ищем полный путь
		string EnvPath(Environment::get("PATH")); // Переменная окружения Path
		EnvPath=Path::expand(EnvPath); // Раскрываем всякие %Dir%
		string fileName=pPath.getFileName();
		found=Path::find(EnvPath,fileName,pPath); 
		
	}
	else
	{
		found=true;
	}
	fullPath=pPath.toString();
	return found;
}

//------------------------------------------------------------------------
//! Разбить строку аргументов на вектор по разделителю "пробел"
void Executer::splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs)
{
	StringTokenizer tok(strArgs," ",StringTokenizer::TOK_IGNORE_EMPTY); // Разбивает на части
	vectArgs.assign(tok.begin(),tok.end()); // Копирует
}
//------------------------------------------------------------------------
//! Создать каталог по имени файла
void Executer::createDir(const std::string &fileName)
{
	Path pPath(fileName);
	pPath.makeFile();
	pPath.setFileName("");
	File pFile(pPath);
	pFile.createDirectories();
}