/*
	This file is part of mrotate. Rotate logs for Windows

    Copyright (C) 2011  Andrey Kapustin

    mrotate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    mrotate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	��� ���������������� �� �������� GPL 3
	�����: �������� ������, 2011 �.

*/

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
	//! ������ ���� ������������ �����
	std::string fullExeName;

	//! ��������� ������� (��� ������ .exe)
	std::string arguments;
	//! ���������� ����� ������
	std::string extension;

};

