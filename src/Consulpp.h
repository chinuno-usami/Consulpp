#pragma once
#include <string>
#include <memory>
#include "ConsulService.h"
class CConsulpp
{
    class CImpl;
public:
    CConsulpp() = delete;
    CConsulpp(std::string strHost, unsigned int uiPort);
    ~CConsulpp();
    // 向consul注册服务
    bool RegisterService(const CConsulService &service);
    // 向consul注销服务
    bool Deregister(std::string strId);
    // 获取consul kv存储的值
    std::string GetValue(std::string strKey);
    // 创建，更新kv
    bool SetValue(std::string strKey, std::string strValue);
    // 删除kv
    bool DeleteValue(std::string strKey);
private:
    std::unique_ptr<CImpl> m_pData;
};

