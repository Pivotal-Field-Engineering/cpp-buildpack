#include "gemfireuploader.h"

template<>
void GemFireUploader<PdamageA>::UploadFile()
{
    std::ifstream ifile(param.inputFileName, std::ios_base::binary);
    if (!ifile)
        throw std::runtime_error("could not open input file" + param.inputFileName);


    apache::geode::client::RegionFactoryPtr regionFactory = cachePtr->createRegionFactory(apache::geode::client::CACHING_PROXY);

    // Create the example Region Programmatically.
    apache::geode::client::RegionPtr regionPtr = regionFactory->create("rqetest");


    Record<PdamageA> rec;
    while (ifile >> rec)
    {
        /*
        cmd.Param("exposurekey").setAsLong() = rec.h.ExposureKey;
        cmd.Param("accountkey").setAsLong() = rec.h.AccountKey;
        cmd.Param("policykey").setAsLong() = rec.h.PolicyKey;
        cmd.Param("sitekey").setAsLong() = rec.h.SiteKey;
        cmd.Param("structurekey").setAsLong() = rec.h.StructureKey;
        cmd.Param("coverageid").setAsLong() = rec.h.CoverageID;
        cmd.Param("anlcfgkey").setAsLong() = rec.h.AnlCfgKey;
        cmd.Param("perilid").setAsLong() = rec.h.PerilID;
        cmd.Param("modelregionid").setAsLong() = rec.h.ModelRegionID;
        cmd.Param("countrykey").setAsLong() = rec.h.CountryKey;
        cmd.Param("fips").setAsLong() = rec.h.Fips;
        cmd.Param("postcode").setAsLong() = rec.h.Postcode;
        cmd.Param("regionkey").setAsLong() = rec.h.RegionKey;
        cmd.Param("damage").setAsLongBinary() = SAString(rec.bin.data(), rec.bin.size());

        cmd.Execute();
        */
    }
}

