#include <iostream>
#include <boost/program_options.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/option.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <sstream>
#include "pocenums.h"

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
static const char* wf_version = "1.0.0";

void PrintHelp(boost::program_options::options_description &cmdline_options)
{
    std::cout << "querytester v." << wf_version << std::endl
        << cmdline_options << std::endl;
}



EBinaryFileType::EBinaryFileType get_file_type(std::string filename, std::vector<std::string>& tokens)
{

    boost::split(tokens, filename, boost::is_any_of("."), boost::token_compress_on );

    if (tokens.empty())
        return EBinaryFileType::UNKNOWN;


    if (tokens[0] == "pdamagea")
        return EBinaryFileType::PDAMAGEA;
    else if (tokens[0] == "intdamagebycover")
        return EBinaryFileType::INTDAMAGE_BY_COVER;
    else if (tokens[0] == "intdamagebyevent")
        return EBinaryFileType::INTDAMAGE_BY_EVENT;

    return EBinaryFileType::UNKNOWN;
}


// a custom option type
struct database_type {
    std::string value;

    EDBType::EDBType get_type() {
        if (value == "postgres")
            return EDBType::POSTGRES;
        else if (value == "gemfire")
            return EDBType::GEMFIRE;
        else
            return EDBType::UNKNOWN;
    }

    // self-describing
    static constexpr const char *option_name() { return "dbtype"; }

    static constexpr const char *description() { return "db type. must be 'postgres' or 'gemfire'"; }

    // check the value and throw a nested exception chain if it's wrong
    void check_value() const
    try {
        if (value != "postgres" and value != "gemfire") {
            std::ostringstream ss;
            ss << "value must be postgres or gemfire, you supplied " << std::quoted(value);
            throw std::invalid_argument(ss.str());
        }
    }
    catch (...) {
        std::throw_with_nested(po::validation_error(po::validation_error::invalid_option_value, option_name()));

    }

    // overload operators
    friend std::istream &operator>>(std::istream &is, database_type &arg) {
        is >> arg.value;
        arg.check_value();
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, database_type const &arg) {
        return os << arg.value;
    }

};

int main(int argc, char* argv[])
{
    std::string appName = boost::filesystem::basename(argv[0]);
    po::options_description genericOptions("General Options");
    genericOptions.add_options()
        ("help", po::value<std::string>(), "Help, I need somebody! Help, not just anybody...")
        ;

    std::string host, user, password, server, database;
    int port;

    database_type dbType;
    po::options_description outOptions("Input/Output Options");
    outOptions.add_options()
        ("host,h", po::value<std::string>(&server)->default_value("localhost"), "Host name to connect to")
        ("port,p", po::value<int>(&port)->required(), "Port name to connect to")
        (database_type::option_name(), po::value<database_type>(&dbType)->required(), database_type::description())
        ("user", po::value<std::string>(&user), "username to use")
        ("password", po::value<std::string>(&password), "password to use")
        ("database", po::value<std::string>(&database)->default_value("rqetest"), "database name")
        ;

    po::options_description cmdline_options;
    cmdline_options.add(genericOptions).add(outOptions);

    po::variables_map vm;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(cmdline_options).run(), vm);

        if (vm.count("help"))
        {
            PrintHelp(cmdline_options);

            return 0;
        }

        po::notify(vm);


        // TODO

    }
    catch (po::invalid_command_line_syntax& e)
    {
        std::cout << "Problem parsing command line: " << e.what() << std::endl << std::endl;
        PrintHelp(cmdline_options);
        return 1;
    }
    catch (po::error& e)
    {
        std::cout << "Problem parsing command line: " << e.what() << std::endl << std::endl;
        PrintHelp(cmdline_options);
        return 1;
    }
    catch (std::logic_error& e)
    {
        std::cout << "Problem parsing command line: " << e.what() << std::endl << std::endl;
        PrintHelp(cmdline_options);
        return 1;
    }
    catch (std::exception& e)
    {
        std::cout << "Problem parsing command line: " << e.what() << std::endl << std::endl;
        PrintHelp(cmdline_options);
        return 1;
    }


    return 0;
}
