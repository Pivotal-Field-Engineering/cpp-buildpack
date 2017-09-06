#ifndef POSTGRESQLUPLOADER_H
#define POSTGRESQLUPLOADER_H
#include "common_types.h"
#include <string>
#include <fstream>
#include <SQLAPI.h>
#include <iostream>
#include <boost/format.hpp>

template <typename FileStruct>
class PostgreSQLUploader
{
public:
    PostgreSQLUploader(const InputParameters& p)
        : param(p)
    {}

    ~PostgreSQLUploader() { conn.Disconnect(); }

    bool Setup();
    void UploadFile();

private:
    const InputParameters& param;
    Record<FileStruct> currentRecord;
    SAConnection conn;
};

template<typename FileStruct>
bool PostgreSQLUploader<FileStruct>::Setup()
{
    // Connect to the database
    conn.setClient(SA_PostgreSQL_Client);
    conn.Connect( (boost::format("%s,%d@") % param.server % param.port ).str().c_str(), param.user.c_str(), param.password.c_str() );
    if (!conn.isAlive() || !conn.isConnected())
        throw std::runtime_error("Could not connect to database.");

    if (param.newDatabase)
    {
        SACommand cmd(&conn);
        cmd.setCommandText((boost::format("DROP DATABASE IF EXISTS %s;") % param.database).str().c_str());
        cmd.Execute();
        cmd.Close();

        cmd.setCommandText((boost::format("CREATE DATABASE %s;") % param.database).str().c_str());
        cmd.Execute();
        cmd.Close();
    }

    conn.Disconnect();

    conn.Connect((boost::format("%s,%d@%s") % param.server % param.port % param.database).str().c_str(),
                 param.user.c_str(), param.password.c_str());

    if (!conn.isAlive() || !conn.isConnected())
        throw std::runtime_error("Could not connect to database.");

    // Make sure all data structures are created

    std::string pda = "CREATE TABLE IF NOT EXISTS pdamagea ("
        "exposurekey int null,"
        "accountkey int null,"
        "policykey int null,"
        "sitekey int null,"
        "structurekey int null,"
        "coverageid smallint null,"
        "anlcfgkey int null,"
        "perilid smallint null,"
        "modelregionid smallint null,"
        "countrykey int null,"
        "fips bigint null,"
        "postcode bigint null,"
        "regionkey int null,"
        "damage bytea null"
    ")";

    std::string intdamagebycover = "CREATE TABLE IF NOT EXISTS intdamagebycover ("
       "exposurekey int null,"
       "accountkey int null,"
       "policykey int null,"
       "sitekey int null,"
       "structurekey int null,"
       "coverageid smallint null,"
       "anlcfgkey int null,"
       "modelregionid smallint null,"
       "perilid smallint null,"
       "gridid int null,"
       "rankIndex smallint null,"
       "damage bytea null"
    ")";

    std::string intdamagebyevent = "CREATE TABLE IF NOT EXISTS intdamagebyevent ("
       "exposurekey int null,"
       "accountkey int null,"
       "anlcfgkey int null,"
       "modelregionid smallint null,"
       "perilid smallint null,"
       "eventid int null,"
       "countrykey smallint null,"
       "chunkid smallint null,"
       "damage bytea null"
    ")";

    SACommand cmd(&conn);
    cmd.setCommandText(pda.c_str());
    cmd.Execute();
    cmd.setCommandText(intdamagebycover.c_str());
    cmd.Execute();
    cmd.setCommandText(intdamagebyevent.c_str());
    cmd.Execute();
    cmd.Close();

}

template<typename FileStruct>
void PostgreSQLUploader<FileStruct>::UploadFile()
{
    throw std::runtime_error("Usupported upload");
}

template<>
void PostgreSQLUploader<PdamageA>::UploadFile();

template<>
void PostgreSQLUploader<IntDamageByCover>::UploadFile();

template<>
void PostgreSQLUploader<IntDamageByEvent>::UploadFile();


#endif // POSTGRESQLUPLOADER_H
