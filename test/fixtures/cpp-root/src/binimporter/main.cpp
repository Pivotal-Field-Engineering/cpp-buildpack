#include <iostream>
#include <boost/program_options.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/option.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <sstream>
#include "common_types.h"
#include "postgresqluploader.h"
#include "gemfireuploader.h"


using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
static const char* wf_version = "1.0.0";

void PrintHelp(boost::program_options::options_description &cmdline_options)
{
    std::cout << "binimporter v." << wf_version << std::endl
        << cmdline_options << std::endl;
}


std::istream &operator>>(std::istream &is, Record<PdamageA> &r) {
    is.read((char *)&r.h, sizeof(r.h));
    r.bin.resize(r.h.DamageLen);
    is.read(r.bin.data(), r.h.DamageLen);
    return is;
}

std::istream &operator>>(std::istream &is, Record<IntDamageByCover> &r) {
    is.read((char *)&r.h, sizeof(r.h));
    r.bin.resize(r.h.DamageLen);
    is.read(r.bin.data(), r.h.DamageLen);
    return is;
}

std::istream &operator>>(std::istream &is, Record<IntDamageByEvent> &r) {
    is.read((char *)&r.h, sizeof(r.h));
    r.bin.resize(r.h.DamageLen);
    is.read(r.bin.data(), r.h.DamageLen);
    return is;
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

template <typename FileStruct>
void upload_file_pg(const InputParameters& p)
{
    PostgreSQLUploader<FileStruct> uploader(p);

    uploader.Setup();
    uploader.UploadFile();
}

template <typename FileStruct>
void upload_file_gf(const InputParameters& p)
{
    GemFireUploader<FileStruct> uploader(p);

    uploader.Setup();
    uploader.UploadFile();
}

template <typename FileStruct>
void upload_file(const InputParameters& p)
{
    switch (p.dbType) {
    case EDBType::POSTGRES:
        upload_file_pg<FileStruct>(p);
        break;
    case EDBType::GEMFIRE:
        upload_file_gf<FileStruct>(p);
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    std::string appName = boost::filesystem::basename(argv[0]);
    po::options_description genericOptions("General Options");
    genericOptions.add_options()
        ("help", po::value<std::string>(), "Help, I need somebody! Help, not just anybody...")
        ;

    std::string inputPath, host, user, password;
    int port;
    InputParameters parms;

    database_type dbType;
    po::options_description outOptions("Input/Output Options");
    outOptions.add_options()
        ("input,i", po::value<std::string>(&inputPath)->required(), "Input directory to process")
        ("host,h", po::value<std::string>(&parms.server)->default_value("localhost"), "Host name to connect to")
        ("port,p", po::value<int>(&parms.port)->required(), "Port name to connect to")
        (database_type::option_name(), po::value<database_type>(&dbType)->required(), database_type::description())
        ("user", po::value<std::string>(&parms.user), "username to use")
        ("password", po::value<std::string>(&parms.password), "password to use")
        ("newdb", po::value<bool>(&parms.newDatabase)->default_value(false), "create new database")
        ("database", po::value<std::string>(&parms.database)->default_value("rqetest"), "database name")
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
        fs::path input = inputPath;

        // validate path
        if ( !fs::exists(input) || !fs::is_directory(input) )
        {
            cerr << "Input path must exist!" << endl;
            return 1;
        }

        // Determine file type
        for (fs::directory_iterator di(input); di != fs::directory_iterator(); di++)
        {
            std::vector<std::string> tokFilename;
            std::string filename = di->path().filename().string();
            auto fileType = get_file_type(filename, tokFilename);
            auto db_type = dbType.get_type();

            parms.inputFileName = di->path().string();
            parms.dbType = dbType.get_type();
            parms.exposureKey = atoi(tokFilename[1].c_str());


            try {
                switch (fileType) {
                case EBinaryFileType::PDAMAGEA:
                    upload_file<PdamageA>(parms);
                    break;
                case EBinaryFileType::INTDAMAGE_BY_COVER:
                    upload_file<IntDamageByCover>(parms);
                    break;
                case EBinaryFileType::INTDAMAGE_BY_EVENT:
                    upload_file<IntDamageByEvent>(parms);
                    break;
                default:
                    cerr << "Unsupported file type." << endl;
                    return 2;
                    break;
                }
            }
            catch (SAException& e)
            {
                cerr << (const char*) e.ErrMessage() << endl;
            }
            catch (std::runtime_error& e)
            {
                cerr << e.what() << endl;
            }
        }
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
