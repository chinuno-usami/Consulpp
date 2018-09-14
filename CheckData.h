#pragma once
#include <string>
class CCheckData
{
public:
    CCheckData() = default;
    ~CCheckData() = default;
    std::string m_strId;
    std::string m_strName;
    std::string m_strNote;
    std::string m_strTimeout;
    std::string m_strInterval;
    std::string m_strTcp;
};
