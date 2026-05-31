#include <cstdio>

#include <memory>

#include "App.hpp"

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	auto app = std::make_unique<App>();

	app->run();

	return 0;
}
