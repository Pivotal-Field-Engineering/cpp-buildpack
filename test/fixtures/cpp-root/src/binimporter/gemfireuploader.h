#ifndef GEMFIREUPLOADER_H
#define GEMFIREUPLOADER_H

#include "common_types.h"
#include <string>
#include <fstream>
#include <iostream>
#include <boost/format.hpp>
// Include the Geode library.
#include <geode/GeodeCppCache.hpp>


template <typename FileStruct>
class GemFireUploader
{
public:
    GemFireUploader(const InputParameters& p)
        : param(p)
    {}
    ~GemFireUploader()
    {
        if (!cachePtr->isClosed())
            cachePtr->close();
    }
    bool Setup();
    void UploadFile();

private:
    const InputParameters& param;
    Record<FileStruct> currentRecord;

    apache::geode::client::CachePtr cachePtr;
};


template<typename FileStruct>
bool GemFireUploader<FileStruct>::Setup()
{
    try
    {
        // Create a Geode Cache.
        apache::geode::client::CacheFactoryPtr cacheFactory = apache::geode::client::CacheFactory::createCacheFactory();

        cachePtr = cacheFactory
                            ->addLocator("10.66.51.64", 10334)
                            ->create();
        //cachePtr = cacheFactory->create();
/*
        apache::geode::client::RegionFactoryPtr regionFactory = cachePtr->createRegionFactory(apache::geode::client::CACHING_PROXY);

        // Create the example Region Programmatically.
        apache::geode::client::RegionPtr regionPtr = regionFactory->create("rqetest");

        regionPtr->put("Test", "foo");
        */
    }
    catch (const apache::geode::client::Exception& geodeExcp)
    {
        std::cerr << "Geode Exception: " << geodeExcp.getMessage() << std::endl;

        return false;
    }
    return true;

}


template<typename FileStruct>
void GemFireUploader<FileStruct>::UploadFile()
{
    throw std::runtime_error("Usupported upload");
}

template<>
void GemFireUploader<PdamageA>::UploadFile();









#endif // GEMFIREUPLOADER_H
