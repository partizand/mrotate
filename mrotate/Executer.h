#pragma once

#include <vector>

//! ������ ������� ��������
class Executer
{
public:
	Executer(void);
	~Executer(void);
	//! ��������� ������� ����� � ����� ����������, ���������� ��� ��������
	static int execute(const std::string &exePath,const std::vector<std::string> &vectArgs,bool wait=true);
	//! �������� ������ ��� ����� (���� � %path% ���������), ���� ���� ��� ������, �� ������ ������ ��� ��
	static std::string getFullPath(std::string aPath);
	//! ������� ������ ���������� �� ������ �� ����������� "������"
	static void splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs);
};

