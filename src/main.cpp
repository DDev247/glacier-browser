
#include "../include/application.h"
#include <cstdlib>
#include <iostream>
#include "../include/logger.h"

using glacier::Log;

#ifndef BLAZE_VERSION
#define BLAZE_VERSION "N/A"
#endif

int main() {
    glacier::Application app;
    LOG(Log::Level::Info, "main", "Welcome to Glacier " GLACIER_VERSION " with Blaze " BLAZE_VERSION);

    try {
        app.run();
    }
    catch (const std::exception& e) {
        //std::cerr << e.what() << std::endl;
        LOG(Log::Level::Error, "main", "Exception caught: " + std::string(e.what()));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
