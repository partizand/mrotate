#pragma once

#include <Poco/Process.h>

//! ������ ������� ��������
class Executer
{
public:
	Executer(void);
	~Executer(void);
	//! ��������� ������� ����� � ����� ����������, ���������� ��� ��������
	static int execute(std::string exePath,Poco::Process::Args &vectArgs,bool wait=true);
	//! ������� ������ ���������� �� ������ �� ����������� "������"
	static void splitArgs(const std::string &strArgs,Poco::Process::Args &vectArgs);
};

