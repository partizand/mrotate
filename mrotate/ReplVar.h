#pragma once

#include <vector>

#include <Poco\DateTime.h>

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
	static std::string replaceFile(const std::string &str,const std::string &FileName,const std::string &ArhFileName="",int index=0);
	//! �������� ��� ����� � ���� � ������
	static std::string replaceFileAndDate(const std::string &str,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTim,int index=0);
	//! �������� ��� ����� � ���� � �������
	static void replaceFileAndDate(std::vector<std::string> &strVect,const std::string &FileName,const std::string &ArhFileName,Poco::DateTime dateTime,int index=0);
private:
	//! �������� ����/����� � ������ �� *
	static std::string replaceDateMask(const std::string &str);
};

