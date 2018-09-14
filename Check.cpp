#include "Check.h"
#include "CheckData.h"

CCheck::CCheck()
{
    m_pData.reset(new CCheckData);
}

CCheck::CCheck(const CCheck & other)
{
    m_pData.reset(new CCheckData(*other.m_pData));
}


CCheck::CCheck(CCheck&& other)
{
    m_pData.swap(other.m_pData);
}

CCheck::~CCheck()
{
}


bool CCheck::operator==(const CCheck& rhs) const
{
    return GetId() == rhs.GetId();
}

CCheck& CCheck::operator=(const CCheck& rhs)
{
    m_pData.reset(new CCheckData(*rhs.m_pData));
    return *this;
}

CCheck& CCheck::operator=(CCheck&& rhs)
{
    m_pData.swap(rhs.m_pData);
    return *this;
}

void CCheck::SetId( const std::string& strId )
{
    m_pData->m_strId = strId;
}


void CCheck::SetName( const std::string& strName )
{
    m_pData->m_strName = strName;
}


void CCheck::SetNote( const std::string& strNote )
{
    m_pData->m_strNote = strNote;
}


void CCheck::SetInterval( const std::string& strInterval )
{
    m_pData->m_strInterval = strInterval;
}


void CCheck::SetTcp( const std::string& strTcp )
{
    m_pData->m_strTcp = strTcp;
}


void CCheck::SetTimeout( const std::string& strTimeout )
{
    m_pData->m_strTimeout = strTimeout;
}


std::string CCheck::GetId() const
{
    return m_pData->m_strId;
}


std::string CCheck::GetName() const
{
    return m_pData->m_strName;
}


std::string CCheck::GetNote() const
{
    return m_pData->m_strNote;
}


std::string CCheck::GetTcp() const
{
    return m_pData->m_strTcp;
}


std::string CCheck::GetTimeout() const
{
    return m_pData->m_strTimeout;
}


std::string CCheck::GetInterval() const
{
    return m_pData->m_strInterval;
}
