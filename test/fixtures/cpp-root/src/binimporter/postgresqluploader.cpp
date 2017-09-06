#include "postgresqluploader.h"


template<>
void PostgreSQLUploader<PdamageA>::UploadFile()
{
    std::ifstream ifile(param.inputFileName, std::ios_base::binary);
    if (!ifile)
        throw std::runtime_error("could not open input file" + param.inputFileName);

    SACommand cmd(&conn);

    cmd.setCommandText("insert into pdamagea values(:exposurekey, :accountkey, :policykey, :sitekey, :structurekey, "
                       " :coverageid, :anlcfgkey, :perilid, :modelregionid, :countrykey, :fips, :postcode, :regionkey, :damage)");

    Record<PdamageA> rec;
    while (ifile >> rec)
    {
        cmd.Param("exposurekey").setAsLong() = rec.h.ExposureKey;
        cmd.Param("accountkey").setAsLong() = rec.h.AccountKey;
        cmd.Param("policykey").setAsLong() = rec.h.PolicyKey;
        cmd.Param("sitekey").setAsLong() = rec.h.SiteKey;
        cmd.Param("structurekey").setAsLong() = rec.h.StructureKey;
        cmd.Param("coverageid").setAsShort() = rec.h.CoverageID;
        cmd.Param("anlcfgkey").setAsLong() = rec.h.AnlCfgKey;
        cmd.Param("perilid").setAsShort() = rec.h.PerilID;
        cmd.Param("modelregionid").setAsShort() = rec.h.ModelRegionID;
        cmd.Param("countrykey").setAsLong() = rec.h.CountryKey;
        cmd.Param("fips").setAsLong() = rec.h.Fips;
        cmd.Param("postcode").setAsLong() = rec.h.Postcode;
        cmd.Param("regionkey").setAsLong() = rec.h.RegionKey;
        cmd.Param("damage").setAsLongBinary() = SAString(rec.bin.data(), rec.bin.size());

        cmd.Execute();
        //std::cout << rec.h.StructureKey << " " << rec.h.ModelRegionID << std::endl;
    }
}

template<>
void PostgreSQLUploader<IntDamageByCover>::UploadFile()
{
    std::ifstream ifile(param.inputFileName, std::ios_base::binary);
    if (!ifile)
        throw std::runtime_error("could not open input file" + param.inputFileName);

    SACommand cmd(&conn);

    cmd.setCommandText("insert into IntDamageByCover values(:exposurekey, :accountkey, :policykey, :sitekey, :structurekey, "
                       " :coverageid, :anlcfgkey, :modelregionid, :perilid, :gridid, :rankindex, :damage)");

    Record<IntDamageByCover> rec;
    while (ifile >> rec)
    {
        cmd.Param("exposurekey").setAsLong() = param.exposureKey;
        cmd.Param("accountkey").setAsLong() = rec.h.AccountKey;
        cmd.Param("policykey").setAsLong() = rec.h.PolicyKey;
        cmd.Param("sitekey").setAsLong() = rec.h.SiteKey;
        cmd.Param("structurekey").setAsLong() = rec.h.StructureKey;
        cmd.Param("coverageid").setAsShort() = rec.h.CoverageID;
        cmd.Param("anlcfgkey").setAsLong() = rec.h.AnlCfgKey;
        cmd.Param("modelregionid").setAsShort() = rec.h.ModelRegionID;
        cmd.Param("perilid").setAsShort() = rec.h.PerilID;
        cmd.Param("gridid").setAsLong() = rec.h.GridID;
        cmd.Param("rankindex").setAsShort() = rec.h.RankIndex;
        cmd.Param("damage").setAsLongBinary() = SAString(rec.bin.data(), rec.bin.size());

        cmd.Execute();
        //std::cout << rec.h.StructureKey << " " << rec.h.ModelRegionID << std::endl;
    }

}

template<>
void PostgreSQLUploader<IntDamageByEvent>::UploadFile()
{
    std::ifstream ifile(param.inputFileName, std::ios_base::binary);
    if (!ifile)
        throw std::runtime_error("could not open input file" + param.inputFileName);

    SACommand cmd(&conn);

    cmd.setCommandText("insert into intdamagebyevent values(:exposurekey, :accountkey, "
                       " :anlcfgkey, :modelregionid, :perilid, :eventid, :countrykey, :chunkid, :damage)");

    Record<IntDamageByEvent> rec;
    while (ifile >> rec)
    {
        cmd.Param("exposurekey").setAsLong() = param.exposureKey;
        cmd.Param("accountkey").setAsLong() = rec.h.AccountKey;
        cmd.Param("anlcfgkey").setAsLong() = rec.h.AnlCfgKey;
        cmd.Param("modelregionid").setAsShort() = rec.h.ModelRegionID;
        cmd.Param("perilid").setAsShort() = rec.h.PerilID;
        cmd.Param("eventid").setAsLong() = rec.h.EventID;
        cmd.Param("countrykey").setAsShort() = rec.h.CountryKey;
        cmd.Param("chunkid").setAsShort() = rec.h.ChunkID;
        cmd.Param("damage").setAsLongBinary() = SAString(rec.bin.data(), rec.bin.size());

        cmd.Execute();
        //std::cout << rec.h.StructureKey << " " << rec.h.ModelRegionID << std::endl;
    }

}
