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

#include <vector>

#include <Poco\DateTime.h>

const int zeroPad=3; // ����� �������, ����������� ������
const int MAXINDEX=999; // ����������� ���������� �������� �������, ������� pow(10,zeroPad), �� ����� ������� ��� �������?

//! ������ ���������� � ������

/**
������� ������ %yymmdd �� ������� ���� � ������

%w - abbreviated weekday (Mon, Tue, ...) 
%W - full weekday (Monday, Tuesday, ...) 
%b - abbreviated month (Jan, Feb, ...) 
%B - full month (January, February, ...) 
%d - zero-padded day of month (01 .. 31) 
%e - day of month (1 .. 31) 
%f - space-padded day of month ( 1 .. 31) 
%m - zero-padded month (01 .. 12) 
%n - month (1 .. 12) 
%o - space-padded month ( 1 .. 12) 
%y - year without century (70) 
%Y - year with century (1970) 
%H - hour (00 .. 23) 
%h - hour (00 .. 12) 
%a - am/pm 
%A - AM/PM 
%M - minute (00 .. 59) 
%S - second (00 .. 59) 
%s - seconds and microseconds (equivalent to %S.%F) 
%i - millisecond (000 .. 999) 
%c - centisecond (0 .. 9) 
%F - fractional seconds/microseconds (000000 - 999999) 
%z - time zone differential in ISO 8601 format (Z or +NN.NN) 
%Z - time zone differential in RFC format (GMT or +NNNN) 
%% - percent sign

%FileName ������ ��� �����
%FileBaseName ��� ����� ��� ����������
%FileExt ���������� �����
*/
class ReplVar
{
public:
	ReplVar(void);
	~ReplVar(void);
	//! �������� ����/����� � ������, �� ��������� �� �������
	static std::string replaceDate(const std::string &str,Poco::DateTime dateTime);
	
	//! �������� ��� ����� � ������
	static std::string replaceFile(const std::string &str,const std::string &FileName,const std::string &ArhFileName="",int index);
	//! �������� ��� ����� � ���� � ������
	static std::string replaceFileAndDate(const std::string &str,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTim,int index);
	//! �������� ��� ����� � ���� � �������
	static void replaceFileAndDate(std::vector<std::string> &strVect,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime,int index);
	//! ��������� ������� ������ ������� � �����
	static std::string::size_type getIndexPos(const std::string &baseFileName,const std::string &targetMask);
	//! ���������� ������ �����, ��� ��� ��������� ����� � ������� ������ �������
	static int getIndex(const std::string &fileName,std::string::size_type posIndex);
private:
	//! �������� ����/����� � ������ �� *
	static std::string replaceDateMask(const std::string &str);
};

