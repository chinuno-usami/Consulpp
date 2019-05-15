#include "ConsulService.h"
#include <utility>
#include <algorithm>
#include "ServiceData.h"
#include "Check.h"
#include "util.hpp"


CConsulService::CConsulService()
{
    m_pData.reset(new CServiceData);
}

CConsulService::CConsulService(const CConsulService& other)
{
    m_pData.reset(new CServiceData(*other.m_pData));
}

CConsulService::CConsulService(CConsulService&& other)
{
    m_pData.swap(other.m_pData);
}

CConsulService::~CConsulService()
{
}


// 获取服务名称
std::string CConsulService::GetName() const
{
    return m_pData->m_strName;
}


// 获取服务地址
std::string CConsulService::GetAddress() const
{
    return m_pData->m_strAddress;
}


// 获取服务ID
std::string CConsulService::GetId() const
{
    return m_pData->m_strId;
}


// 获取服务端口
unsigned int CConsulService::GetPort() const
{
    return m_pData->m_uiPort;
}


// 获取服务标签
std::unordered_set<std::string> CConsulService::GetTags() const
{
    return m_pData->m_setTags;
}


// 获取元信息
std::unordered_map<std::string, std::string> CConsulService::GetMeta() const
{
    return m_pData->m_mapMetas;
}


// 获取所有健康检查项目
std::unordered_set<CCheck> CConsulService::GetChecks() const
{
    return m_pData->m_setChecks;
}


// 设置服务名
void CConsulService::SetName(const std::string& strName )
{
    m_pData->m_strName = strName;
}


// 设置服务地址
void CConsulService::SetAddress(const std::string& strAddress )
{
    m_pData->m_strAddress = strAddress;
}


// 设置Id
void CConsulService::SetId(const std::string& strId )
{
    m_pData->m_strId = strId;
}


// 设置元信息
void CConsulService::SetMeta(const std::string& strKey, const std::string& strValue )
{
    m_pData->m_mapMetas.insert(std::make_pair(strKey,strValue));
}


// 删除元信息
void CConsulService::RemoveMeta(const std::string& strKey )
{
    m_pData->m_mapMetas.erase(strKey);
}


// 设置服务端口
void CConsulService::SetPort( unsigned int uiPort )
{
    m_pData->m_uiPort = uiPort;
}


// 移除标签
void CConsulService::RemoveTag(const std::string& strTag )
{
    m_pData->m_setTags.erase(strTag);
}


// 添加标签
void CConsulService::SetTag(const std::string& strTag )
{
    m_pData->m_setTags.insert(strTag);
}


// 设置健康检查
void CConsulService::SetCheck( const CCheck& check )
{
    m_pData->m_setChecks.insert(check);
}


// 移除健康检查项
void CConsulService::RemoveCheck(const std::string& strId)
{
    discard_if(m_pData->m_setChecks,
                   [&strId](CCheck obj) {
                        return (obj.GetId() == strId);
                    });
    // set都是const对象无法使用remove_if
    //std::remove_if(std::begin(m_pData->m_setChecks),
    //               std::end(m_pData->m_setChecks),
    //               [&strId](CCheck obj) {
    //                    return (obj.GetId() == strId);
    //                });
}
