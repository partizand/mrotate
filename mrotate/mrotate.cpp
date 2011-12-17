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

	Код распространяется по лицензии GPL 3
	Автор: Капустин Андрей, 2011 г.

*/

// mrotate.cpp : Defines the entry point for the console application.
//
#include "StdAfx.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
//#include <Poco\Logger.h>
//#include <iostream>

#include "LogRotator.h"

using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;
using Poco::Util::OptionCallback;
using Poco::AutoPtr;


class RotateApp: public Application
	/// This sample demonstrates some of the features of the Util::Application class,
	/// such as configuration file handling and command line arguments processing.
	///
	/// Try SampleApp --help (on Unix platforms) or SampleApp /help (elsewhere) for
	/// more information.
{
public:
	RotateApp(): _helpRequested(false),_runRequested(false),_loadRequested(false),
		_checkRequested(false),_debugReq(false),_statusFNameReg(false),
		rotator(logger())
	{
		
		//bak=new Backup(logger());
		poco_information_f1(logger(),"mrotate v.%s",rotator.getVersion());
		//logger().information("mrotate v.0.1. Rotate logs utility for Windows");
		//logger().debug("debug mes");
	}

protected:	
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		Application::initialize(self);
		
		// add your own initialization code here
		//bak=new Backup(logger());
		
	}
	
	void uninitialize()
	{
		//delete bak;
		// add your own uninitialization code here
		Application::uninitialize();
	}
	
	void reinitialize(Application& self)
	{
		Application::reinitialize(self);
		// add your own reinitialization code here
	}
	
	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleHelp)));

		
		
		options.addOption(
			Option("rotate", "r", "Run rotate")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleRun)));
		options.addOption(
			Option("verify", "v", "Verification rotate entries")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleCheck)));
		options.addOption(
			Option("debug", "d", "Set debug mode")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleDebug)));
		options.addOption(
			Option("force", "f", "Set force mode (only for shift rotate)")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleForce)));
		options.addOption(
			Option("conf", "c", "load rotate entries from a file")
				.required(false)
				.repeatable(true)
				.argument("file")
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleLoadEntries)));
		options.addOption(
			Option("arh", "a", "load archivers settings from a file")
				.required(false)
				.repeatable(true)
				.argument("file")
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleLoadEntries)));
		options.addOption(
			Option("status", "s", "Set status filename")
				.required(false)
				.repeatable(false)
				.argument("file")
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleStatusFile)));
	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		displayHelp();
		stopOptionsProcessing();
	}
	
	void handleRun(const std::string& name, const std::string& value)
	{
		_runRequested = true;
		
	}
	void handleDebug(const std::string& name, const std::string& value)
	{
		rotator.setDebugMode();
		_debugReq = true;
		
	}
	void handleForce(const std::string& name, const std::string& value)
	{
		rotator.setForceMode();
		
		
	}

	void handleCheck(const std::string& name, const std::string& value)
	{
		_checkRequested = true;
		
	}

	
	// Загрузка параметров настройки
	/*
	void handleConfig(const std::string& name, const std::string& value)
	{
		loadConfiguration(value);
	}
	*/
	void handleLoadArh(const std::string& name, const std::string& value)
	{
		rotator.archiver.load(value); // Грузим файл архиватора
	}
	void handleStatusFile(const std::string& name, const std::string& value)
	{
		_statusFNameReg=true;
		rotator.setStatusFileName(value); // Установка файла статуса
	}
	void handleLoadEntries(const std::string& name, const std::string& value)
	{
		rotator.load(value); // Грузим файл конфигурации
		_loadRequested=true;
	}	
	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("Rotate text logs utility.");
		helpFormatter.format(std::cout);
	}
	
	

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			if (!_statusFNameReg) // Имя файла статуса по умолчанию
			{
			std::string statusFile=config().getString("application.dir","")+"mrotate.status";
			rotator.setStatusFileName(statusFile);
			}
			std::string arhFile=config().getString("application.dir","")+"archivers.ini";
			rotator.archiver.load(arhFile); // Грузим пользовательские архиваторы


			if (!_loadRequested) 
			{
				logger().information("Loading standart config file.");
				rotator.load(&config());
			}
			if (_checkRequested)
			{
				logger().information("Start checking");
				rotator.check();
			}
			if (_debugReq)	
			{
				logger().information("Start debugging");
				rotator.rotate();
			}
			if (_runRequested)	
			{
				logger().information("Start rotation");
				rotator.rotate();
			}
			//rotator.rotate();
		}
		return Application::EXIT_OK;
	}
	
	
private:
	bool _helpRequested;
	bool _runRequested;
	bool _loadRequested; // Указан параметр загрузки конфигурации из файла
	bool _checkRequested; // Проверка конфигурации
	bool _debugReq; // Режим эмуляции
	bool _statusFNameReg; // Имя файла статуса указано в параметре
	LogRotator rotator;
	
};


POCO_APP_MAIN(RotateApp)
