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
		_checkRequested(false),_debugReq(false),
		rotator(logger())
	{
		//bak=new Backup(logger());
		poco_information_f1(logger(),"mrotate v.%s. Rotate logs utility for Windows.",rotator.getVersion());
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

		
			/*	
		options.addOption(
			Option("config-file", "f", "load configuration data from a file")
				.required(false)
				.repeatable(true)
				.argument("config-file")
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleConfig)));

		*/
		options.addOption(
			Option("run", "r", "Run rotate")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleRun)));
		options.addOption(
			Option("check", "", "Check rotate entries")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleCheck)));
		options.addOption(
			Option("debug", "d", "Set debug mode")
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<RotateApp>(this, &RotateApp::handleDebug)));
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
		helpFormatter.setHeader("Rotate logs utility.");
		helpFormatter.format(std::cout);
	}
	
	

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
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
	LogRotator rotator;
};


POCO_APP_MAIN(RotateApp)
