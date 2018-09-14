#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "Check.h"
class CServiceData
{
public:
    CServiceData() = default;
    ~CServiceData() = default;

    unsigned int m_uiPort;
    std::string m_strName;
    std::string m_strAddress;
    std::string m_strId;
    std::unordered_set<std::string> m_setTags;
    std::unordered_map<std::string, std::string> m_mapMetas;
    std::unordered_set<CCheck> m_setChecks;
};