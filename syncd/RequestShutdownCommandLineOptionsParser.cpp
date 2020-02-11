#include "RequestShutdownCommandLineOptionsParser.h"

#include "swss/logger.h"

#include <unistd.h>
#include <getopt.h>

#include <ostream>
#include <iostream>

using namespace syncd;

std::shared_ptr<RequestShutdownCommandLineOptions> RequestShutdownCommandLineOptionsParser::parseCommandLine(
        _In_ int argc,
        _In_ char **argv)
{
    SWSS_LOG_ENTER();

    auto options = std::make_shared<RequestShutdownCommandLineOptions>();

    static struct option long_options[] =
    {
        { "cold", no_argument, 0, 'c' },
        { "warm", no_argument, 0, 'w' },
        { "fast", no_argument, 0, 'f' },
        { "pre",  no_argument, 0, 'p' }, // Requesting pre shutdown
    };

    bool optionSpecified = false;

    while (true)
    {
        int option_index = 0;

        int c = getopt_long(argc, argv, "cwfp", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
            case 'c':
                options->setRestartType(SYNCD_RESTART_TYPE_COLD);
                optionSpecified = true;
                break;

            case 'w':
                options->setRestartType(SYNCD_RESTART_TYPE_WARM);
                optionSpecified = true;
                break;

            case 'f':
                options->setRestartType(SYNCD_RESTART_TYPE_FAST);
                optionSpecified = true;
                break;

            case 'p':
                options->setRestartType(SYNCD_RESTART_TYPE_PRE_SHUTDOWN);
                optionSpecified = true;
                break;

            default:
                SWSS_LOG_ERROR("getopt failure");
                exit(EXIT_FAILURE);
        }
    }

    if (!optionSpecified)
    {
        SWSS_LOG_ERROR("no shutdown option specified");

        printUsage();

        exit(EXIT_FAILURE);
    }

    return options;
}

void RequestShutdownCommandLineOptionsParser::printUsage()
{
    SWSS_LOG_ENTER();

    std::cout << "Usage: syncd_request_shutdown [-w] [--wram] [-p] [--pre] [-c] [--cold] [-f] [--fast]" << std::endl;

    std::cerr << std::endl;

    std::cerr << "Shutdown option must be specified" << std::endl;
    std::cerr << "---------------------------------" << std::endl;
    std::cerr << " --warm  -w   for warm restart" << std::endl;
    std::cerr << " --pre   -p   for warm pre-shutdown" << std::endl;
    std::cerr << " --cold  -c   for cold restart" << std::endl;
    std::cerr << " --fast  -f   for fast restart" << std::endl;
}
