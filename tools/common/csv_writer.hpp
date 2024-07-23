#ifndef INC_CSV_WRITER
#define INC_CSV_WRITER

#include <fstream>
#include <ostream>
#include <stdarg.h>
using namespace std;

void create_csv_header(std::string filename, const char *arg, ... ){

	try{
		std::ofstream myfile;
		myfile.open(filename, std::ios::trunc); //create a new file: ios::app

		va_list arguments;

		for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char *)) {
			myfile << arg << ",";
		}

		myfile<< std::endl;
		myfile.close();
		va_end(arguments);
	}
	catch (const ifstream::failure& e){
		std::cout << "Exception opening/ reading file" <<std::endl;
	}
}

void write_csv_file(std::string filename, const char *arg, ... ){
	try{
        //std::string fileName = "csv/";
        //fileName.append(filename);
		std::ofstream myfile;
		myfile.open(filename, std::ios::app); //opens an existing csv file: ios::out

		va_list arguments;

		for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, const char *)) {
			myfile << arg << ",";
		}

		myfile<< std::endl;
		myfile.close();
		va_end(arguments);
	}
		catch (const ifstream::failure& e){
			std::cout << "Exception opening/ reading file" <<std::endl;
		}
}

void read_csv_file(std::string filename, char *arg, ...)
{
	try{
		std::ofstream myfile;
		myfile.open(filename);
		va_list arguments;
		for (va_start(arguments, arg); arg != NULL; arg = va_arg(arguments, char *)) {
			myfile << arg << ",";
		}

	}
	catch (const ifstream::failure& e){
		std::cout << "Exception opening/ reading file" <<std::endl;
	}
}

void delete_csv_file(std::string filename)
{
    try{
        std::remove(filename.c_str());
    }
    catch (const ifstream::failure& e){
        std::cout << "Exception removing file" <<std::endl;
    }
}

#endif
