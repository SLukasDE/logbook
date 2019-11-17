#include <logbook/examples/Example01.h>
#include <logbook/examples/Example02.h>
#include <logbook/examples/Example03.h>
#include <logbook/examples/Example04.h>
#include <iostream>
#include <string>


void printUsage() {
	std::cout << "Possible arguments:\n\n";
	std::cout << "  example01\n";
	std::cout << "  example02\n";
	std::cout << "  example03\n";
	std::cout << "  example04\n";
}

int main(int argc, const char *argv[]) {
	std::string argument;

	if(argc < 2) {
		argument = "example04";
	}
	else if(argc == 2) {
		argument = argv[1];
	}
	else {
		std::cout << "Wrong number of arguments.\n\n";
		printUsage();
		return -1;
	}

	if(argument == "example01") {
		logbook::examples::example01();
	}
	else if(argument == "example02") {
		logbook::examples::example02();
	}
	else if(argument == "example03") {
		logbook::examples::example03();
	}
	else if(argument == "example04") {
		logbook::examples::loggerInitialize();
		logbook::examples::example04();
	}
	else {
		std::cout << "unknown argument \"" << argument << "\".\n\n";
		printUsage();
		return -1;
	}
	return 0;
}
