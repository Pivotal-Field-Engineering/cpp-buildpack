#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H
#include "intdamagebycover.h"
#include "pdamagea.h"
#include "intdamagebyevent.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include "pocenums.h"

struct InputParameters {
    bool force = false;
    bool newDatabase = false;
    std::string inputFileName;
    EDBType::EDBType dbType;
    int port;
    std::string server;
    std::string user;
    std::string password;
    std::string database;
    int exposureKey;
};


template <typename HeaderType>
struct Record {
    HeaderType h;
    std::vector<char> bin;
};

std::istream &operator>>(std::istream &is, Record<PdamageA> &r);
std::istream &operator>>(std::istream &is, Record<IntDamageByCover> &r);
std::istream &operator>>(std::istream &is, Record<IntDamageByEvent> &r);

#endif // COMMON_TYPES_H
