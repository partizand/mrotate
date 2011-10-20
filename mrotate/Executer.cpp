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
//! ��������� ������� ����� � ����� ����������, ���������� �����
int Executer::execute(std::string exePath,Poco::Process::Args &vectArgs,bool wait=true)
{
	// ���������, ��� ���� ����������
	Path pExe(exePath);
	if (!pExe.isAbsolute())
	{
		// ���� ������ ����
		string EnvPath(Environment::get("PATH")); // ���������� ��������� Path
		EnvPath=Path::expand(EnvPath); // ���������� ������ %Dir%
		string fileName=pExe.getFileName();
		bool found=Path::find(EnvPath,fileName,pExe); 
		//if (!found) return false; // ��������� �� ������
	}
	int ret=0;
	ProcessHandle handle=Process::launch(pExe.toString(),vectArgs);// ��������
	
	if (wait) // ����
	{
		ret=handle.wait();
	}
	return ret;
}
//------------------------------------------------------------------------
//! ������� ������ ���������� �� ������ �� ����������� "������"
void Executer::splitArgs(const std::string &strArgs,Poco::Process::Args &vectArgs)
{
	StringTokenizer tok(strArgs," ",StringTokenizer::TOK_IGNORE_EMPTY); // ��������� �� �����
	vectArgs.assign(tok.begin(),tok.end()); // ��������
}