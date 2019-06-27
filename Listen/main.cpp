#include <iostream>
#include <string.h>
#include "server.h"

int main() {
	std::cout << "Hello there" << std::endl;

	std::string results = test("Yupidiyup");
	std::cout << results << std::endl;

	server();

	return 0;
}