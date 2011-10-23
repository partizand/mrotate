// mrotate.cpp : Defines the entry point for the console application.
//
#include "StdAfx.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/AutoPtr.h"
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
	RotateApp(): _helpRequested(false),_runRequested(false),_loadRequested(false),rotator(logger())
	{
		//bak=new Backup(logger());
		logger().information("mrotate v.0.1. Rotate logs utility for Windows");
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
			Option("config-file", "c", "load rotate entries from a file")
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

	
	// Загрузка параметров настройки
	/*
	void handleConfig(const std::string& name, const std::string& value)
	{
		loadConfiguration(value);
	}
	*/
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
		helpFormatter.setHeader("Rotate utility.");
		helpFormatter.format(std::cout);
	}
	
	

	int main(const std::vector<std::string>& args)
	{
		if (!_helpRequested)
		{
			
			if (!_loadRequested) rotator.load(&config());
			if (_runRequested)	rotator.rotate();
			//rotator.rotate();
		}
		return Application::EXIT_OK;
	}
	
	
private:
	bool _helpRequested;
	bool _runRequested;
	bool _loadRequested; // Указан параметр загрузки конфигурации из файла
	LogRotator rotator;
};


POCO_APP_MAIN(RotateApp)
