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

#include "StdAfx.h"
#include "Executer.h"

#include <Poco/StringTokenizer.h>
#include <Poco\Path.h>
#include <Poco\File.h>
#include <Poco\Environment.h>
#include <Poco\Process.h>
#include <Poco\String.h>

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
//! ��������� ������� ����� � ����� ����������, ���������� �����
int Executer::execute(const std::string &exePath,const std::vector<std::string> &vectArgs,bool wait)
{
	// ���������, ��� ���� ����������
	string fullPath;
	getFullPath(exePath,fullPath);
	int ret=0;
	ProcessHandle handle=Process::launch(fullPath,vectArgs);// ��������
	
	if (wait) // ����
	{
		ret=handle.wait();
	}
	return ret;
}
//------------------------------------------------------------------------
//! �������� ������ ��� ����� (���� � %path% ���������), ���� ���� ��� ������, �� ������ ������ ��� ��
bool Executer::getFullPath(const std::string &aPath, std::string &fullPath)
{
// ���������, ��� ���� ����������
	bool found;
	Path pPath(aPath);
	pPath.makeFile();
	if (pPath.depth()==0) // �������� ������ ��� �����
	{
		// ���� ������ ����
		string EnvPath(Environment::get("PATH")); // ���������� ��������� Path
		EnvPath=Path::expand(EnvPath); // ���������� ������ %Dir%
		EnvPath+=";"+Path::current(); // ��������� ������� ������� ��� ������
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
//! �������������/����������� ����
bool Executer::moveFile(const std::string &srcFile,const std::string &dstFile,bool debug, Poco::Logger &log)
{
if (debug)
{
	
	poco_information_f2(log,"[Debug] Moving/renaming file %s to %s",srcFile,dstFile);
	return true;
}

	// ������� ���������
	Path srcPath(srcFile);
	srcPath.makeFile();
	srcPath.setFileName("");
	// ������� ���������
	Path dstPath(dstFile);
	dstPath.makeFile();
	dstPath.setFileName("");
	int rename=icompare(srcPath.toString(),dstPath.toString()); // ��������� ���������
try
{
	File tFile(srcFile);
	if (rename==0) // ��������������
	{
		tFile.renameTo(dstFile);
		poco_information_f2(log,"File %s renamed to %s",srcFile,dstFile);
		return true;
	}
	else // �����������
	{
		tFile.moveTo(dstFile);
		poco_information_f2(log,"File %s moved to %s",srcFile,dstFile);
		return true;
	}

}
catch(...) 
{
	poco_error_f2(log,"Error moving/renaming file %s to %s",srcFile,dstFile);
	return false;
}

}


//------------------------------------------------------------------------
//! ������� ������ ���������� �� ������ �� ����������� "������"
void Executer::splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs)
{
	StringTokenizer tok(strArgs," ",StringTokenizer::TOK_IGNORE_EMPTY); // ��������� �� �����
	vectArgs.assign(tok.begin(),tok.end()); // ��������
}
//------------------------------------------------------------------------
//! ������� ������� �� ����� �����
void Executer::createDir(const std::string &fileName)
{
	Path pPath(fileName);
	pPath.makeFile();
	pPath.setFileName("");
	File pFile(pPath);
	pFile.createDirectories();
}