#include "Consulpp.h"
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>
#include <cpprest/http_client.h>

#include "Check.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

static const std::string REGISTER_API("/v1/agent/service/register");
static const std::string DEREGISTER_API("/v1/agent/service/deregister/");
static const std::string KV_API("/v1/kv/");

class CConsulpp::CImpl{
public:
    CImpl() = delete;
    CImpl( std::string strHost, unsigned int uiPort );
    std::string m_strHost;
    unsigned int m_uiPort;
    std::string m_strUriBase;
};

CConsulpp::CImpl::CImpl(std::string strHost, unsigned int uiPort)
{
    m_strHost = strHost;
    m_uiPort = uiPort;
    m_strUriBase = "http://" + strHost + ":" + std::to_string(uiPort);
}

static std::function<bool(http_response)> NoReturnProcessor()
{
    return [=](http_response resp) {
        std::cout << "status:" << resp.status_code()<<std::endl;
        if (resp.status_code() == status_codes::OK)
        {
            return true;
        }
        std::string strError("Register resopnsed body:");
        strError += resp.extract_string(true).get();
        std::cout << strError << std::endl;
        return false;
        //throw std::exception("register request failed");
        // return pplx::task_from_result(json::value());
    };
}

CConsulpp::CConsulpp( std::string strHost, unsigned int uiPort ):m_pData(new CImpl(strHost, uiPort))
{
}


CConsulpp::~CConsulpp()
{
}


// 向consul注册服务
bool CConsulpp::RegisterService( const CConsulService &service )
{
    json::value jsonReqData;
    jsonReqData["ID"] = json::value::string(service.GetId());
    jsonReqData["Name"] = json::value::string(service.GetName());
    jsonReqData["Port"] = json::value::number(service.GetPort());

    // 添加Metas
    json::value jsonMetas;
    const auto& mapMetas = service.GetMeta();
    std::for_each(std::begin(mapMetas), std::end(mapMetas),
                  [&jsonMetas](std::pair<std::string, std::string> item) {
        jsonMetas[item.first] = json::value::string(item.second);
    });
    jsonReqData["Meta"] = jsonMetas;

    // 添加tags
    json::value jsonTags;
    const auto& setTags = service.GetTags();
    size_t nTagsCount = 0;
    std::for_each(std::begin(setTags), std::end(setTags),
                  [&jsonTags, &nTagsCount](std::string strTag) {
        jsonTags[nTagsCount++] = json::value::string(strTag);
    });

    // 添加checks
    json::value jsonChecks;
    // 构造单个check的json对象
    auto ParseCheck = [](const CCheck& check) {
        json::value jsonCheck;
        jsonCheck["ID"] = json::value::string(check.GetId());
        jsonCheck["Name"] = json::value::string(check.GetName());
        jsonCheck["TCP"] = json::value::string(check.GetTcp());
        jsonCheck["Interval"] = json::value::string(check.GetInterval());
        jsonCheck["Timeout"] = json::value::string(check.GetTimeout());
        jsonCheck["Notes"] = json::value::string(check.GetTimeout());
        return jsonCheck;
    };
    const auto& setChecks = service.GetChecks();
    size_t nChecksCount = 0;
    std::for_each(std::begin(setChecks), std::end(setChecks),
                  [&jsonChecks, &nChecksCount, &ParseCheck](CCheck check) {
        jsonChecks[nChecksCount++] = ParseCheck(check);
    });

    jsonReqData["Tags"] = jsonTags;
    jsonReqData["Checks"] = jsonChecks;

    // 请求consul api
    http_client client(U(m_pData->m_strUriBase));
    uri_builder builder(U(REGISTER_API));

    auto resp = client.request(methods::PUT, builder.to_string(), jsonReqData).
        then(NoReturnProcessor());
    try
    {
        return resp.get();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}


// 向consul注销服务
bool CConsulpp::Deregister( std::string strId )
{
    // 请求consul api
    http_client client(U(m_pData->m_strUriBase));
    uri_builder builder(U(DEREGISTER_API));
    builder.append_path(strId);

    auto resp = client.request(methods::PUT, builder.to_string()).
        then(NoReturnProcessor());
    try
    {
        return resp.get();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

std::string CConsulpp::GetValue(std::string strKey)
{
    // 请求consul api
    http_client client(U(m_pData->m_strUriBase));
    uri_builder builder(U(KV_API));
    builder.append_path(strKey);
    // 直接返回原始值，不编码
    builder.append_query(U("raw"));

    auto resp = client.request(methods::GET, builder.to_string()).
        then([](http_response resp) {
        std::cout << "status:" << resp.status_code() << std::endl;
        if (resp.status_code() == status_codes::OK)
        {
            return resp.extract_string(true);
        }
        std::string strError("Register resopnsed body:");
        strError += resp.extract_string(true).get();
        std::cout << strError << std::endl;
        return pplx::task_from_result(std::string());
    });
    try
    {
        return resp.get();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return std::string();
    }
}

bool CConsulpp::SetValue(std::string strKey, std::string strValue)
{
    // 请求consul api
    http_client client(U(m_pData->m_strUriBase));
    uri_builder builder(U(KV_API));
    builder.append_path(strKey);

    auto resp = client.request(methods::PUT, builder.to_string(), strValue).
        then(NoReturnProcessor());
    try
    {
        return resp.get();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool CConsulpp::DeleteValue(std::string strKey)
{
    // 请求consul api
    http_client client(U(m_pData->m_strUriBase));
    uri_builder builder(U(KV_API));
    builder.append_path(strKey);

    auto resp = client.request(methods::DEL, builder.to_string()).
        then(NoReturnProcessor());
    try
    {
        return resp.get();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}
