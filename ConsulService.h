#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "Check.h"
class CServiceData;
class CConsulService
{
public:
    CConsulService();
    CConsulService(const CConsulService& other);
    CConsulService(CConsulService&& other);
    ~CConsulService();
    // 获取服务名称
    std::string GetName() const;
    // 获取服务地址
    std::string GetAddress() const;
    // 获取服务ID
    std::string GetId() const;
    // 获取服务端口
    unsigned int GetPort() const;
    // 获取服务标签
    std::unordered_set<std::string> GetTags() const;
    // 获取元信息
    std::unordered_map<std::string, std::string> GetMeta() const;
    // 获取所有健康检查项目
    std::unordered_set<CCheck> GetChecks() const;
    // 设置服务名
    void SetName(const std::string& strName);
    // 设置服务地址
    void SetAddress(const std::string& strAddress);
    // 设置Id
    void SetId(const std::string& strId);
    // 设置元信息
    void SetMeta(const std::string& strKey, const std::string& strValue);
    // 删除元信息
    void RemoveMeta(const std::string& strKey);
    // 设置服务端口
    void SetPort(unsigned int uiPort);
    // 移除标签
    void RemoveTag(const std::string& strTag);
    // 添加标签
    void SetTag(const std::string& strTag);
    // 设置健康检查
    void SetCheck(const CCheck& check);
    // 移除健康检查项
    void RemoveCheck(const std::string& strId);
private:
    // 内部变量
    std::unique_ptr<CServiceData> m_pData;
};

