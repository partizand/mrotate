#pragma once

//! ��������� ����������
class ArchiverParam
{
public:
	ArchiverParam(void);
	ArchiverParam(std::string ArchiverName,std::string Extention,std::string ExeName,std::string Arguments);
	~ArchiverParam(void);
	//! ��� ����������
	std::string archiverName;
	//! ��� ������������ ����� (7z.exe,rar.exe ��� ����)
	std::string exeName;
	//! ��������� ������� (��� ������ .exe)
	std::string arguments;
	//! ���������� ����� ������
	std::string extension;

};

