#pragma once
#include <fstream>
#include <string>
class Logger
{
public:
	Logger(std::string path)
	{
		fout.open(path);
		fout << "Запись начата.\n";
		fout.close();
		this->path = path;
	}
	void log(std::string message)
	{
		fout.open(path, std::ios_base::app);
		fout << message << std::endl;
		fout.close();
	}
	private:
	std::string path;
	std::ofstream fout;
};