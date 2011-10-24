#pragma once

#include <vector>

//! Запуск внешних программ
class Executer
{
public:
	Executer(void);
	~Executer(void);
	//! Запустить внешнюю прогу и ждать завершения, возвращает код возврата
	static int execute(const std::string &exePath,const std::vector<std::string> &vectArgs,bool wait=true);
	//! Получить полное имя файла (ищет в %path% каталогах), если путь уже полный, то просто вернет его же
	static bool getFullPath(const std::string &aPath, std::string &fullPath);
	
	
	
	//! Разбить строку аргументов на вектор по разделителю "пробел"
	static void splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs);
};

