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
int Executer::execute(const std::string &exePath,const std::vector<std::string> &vectArgs,bool wait)
{
	// ���������, ��� ���� ����������
	string fullPath=getFullPath(exePath);
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
std::string Executer::getFullPath(std::string aPath)
{
// ���������, ��� ���� ����������
	Path pPath(aPath);
	if (pPath.depth()==0) // �������� ������ ��� �����
	{
		// ���� ������ ����
		string EnvPath(Environment::get("PATH")); // ���������� ��������� Path
		EnvPath=Path::expand(EnvPath); // ���������� ������ %Dir%
		string fileName=pPath.getFileName();
		Path::find(EnvPath,fileName,pPath); 
		
	}
	return pPath.toString();
}

//------------------------------------------------------------------------
//! ������� ������ ���������� �� ������ �� ����������� "������"
void Executer::splitArgs(const std::string &strArgs,std::vector<std::string> &vectArgs)
{
	StringTokenizer tok(strArgs," ",StringTokenizer::TOK_IGNORE_EMPTY); // ��������� �� �����
	vectArgs.assign(tok.begin(),tok.end()); // ��������
}