#pragma once

#include <string>
#include <memory>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <functional>
#include <cpprest/http_client.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

#if (__cplusplus < 201402L || (defined(_MSC_VER) && _MSC_VER < 1900))
namespace std
{
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args&& ...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

namespace consulpp
{

    class CheckData
    {
    public:
        CheckData() = default;
        ~CheckData() = default;

        std::string id_{};
        std::string name_{};
        std::string note_{};
        std::string timeout_{};
        std::string interval_{};
        std::string tcp_{};
    };

    class Check
    {
    public:
        Check()
        {
            data_ptr_ = std::make_unique<CheckData>();
        }

        Check(const Check& other)
        {
            data_ptr_ = std::make_unique<CheckData>(*other.data_ptr_);
        }

        Check(Check&& other)
        {
            data_ptr_.swap(other.data_ptr_);
        }

        ~Check() = default;

        bool operator==(const Check& rhs) const
        {
            return GetId() == rhs.GetId();
        }

        Check& operator=(const Check& rhs)
        {
            data_ptr_ = std::make_unique<CheckData>(*rhs.data_ptr_);
            return *this;
        }

        Check& operator=(Check&& rhs)
        {
            data_ptr_.swap(rhs.data_ptr_);
            return *this;
        }

        void SetId(const std::string& value)
        {
            data_ptr_->id_ = value;
        }

        void SetName(const std::string& value)
        {
            data_ptr_->name_ = value;
        }

        void SetNote(const std::string& value)
        {
            data_ptr_->note_ = value;
        }

        void SetInterval(const std::string& value)
        {
            data_ptr_->interval_ = value;
        }

        void SetTcp(const std::string& value)
        {
            data_ptr_->tcp_ = value;
        }

        void SetTimeout(const std::string& value)
        {
            data_ptr_->timeout_ = value;
        }

        const std::string& GetId() const
        {
            return data_ptr_->id_;
        }

        const std::string& GetName() const
        {
            return data_ptr_->name_;
        }

        const std::string& GetNote() const
        {
            return data_ptr_->note_;
        }

        const std::string& GetTcp() const
        {
            return data_ptr_->tcp_;
        }

        const std::string& GetTimeout() const
        {
            return data_ptr_->timeout_;
        }

        const std::string& GetInterval() const
        {
            return data_ptr_->interval_;
        }

    private:
        std::unique_ptr<CheckData> data_ptr_{ nullptr };
    };
    
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename Comp, typename Alloc, typename Predicate>
    void discard_if(std::unordered_set<T, Comp, Alloc>& c, Predicate pred)
    {
        for (auto it = c.begin(), end = c.end(); it != end;)
        {
            if (pred(*it))
            {
                it = c.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    
    class ServiceData
    {
    public:
        ServiceData() = default;
        ~ServiceData() = default;

        std::string id_{};
        std::string name_{};
        std::string address_{};
        unsigned int port_{ 0 };
        std::unordered_set<std::string> tags_;
        std::unordered_map<std::string, std::string> metas_;
        std::unordered_set<Check> checks_;
    };

    class ConsulService
    {
    public:
        ConsulService()
        {
            data_ptr_ = std::make_unique<ServiceData>();
        }

        ConsulService(const ConsulService& other)
        {
            data_ptr_ = std::make_unique<ServiceData>(*other.data_ptr_);
        }

        ConsulService(ConsulService&& other)
        {
            data_ptr_.swap(other.data_ptr_);
        }

        ~ConsulService() = default;

        // 获取服务名称
        const std::string& GetName() const
        {
            return data_ptr_->name_;
        }

        // 获取服务地址
        const std::string& GetAddress() const
        {
            return data_ptr_->address_;
        }

        // 获取服务ID
        const std::string& GetId() const
        {
            return data_ptr_->id_;
        }

        // 获取服务端口
        unsigned int GetPort() const
        {
            return data_ptr_->port_;
        }

        // 获取服务标签
        std::unordered_set<std::string>& GetTags() const
        {
            return data_ptr_->tags_;
        }

        // 获取元信息
        const std::unordered_map<std::string, std::string>& GetMeta() const
        {
            return data_ptr_->metas_;
        }

        // 获取所有健康检查项目
        const std::unordered_set<Check>& GetChecks() const
        {
            return data_ptr_->checks_;
        }

        // 设置服务名
        void SetName(const std::string& value)
        {
            data_ptr_->name_ = value;
        }

        // 设置服务地址
        void SetAddress(const std::string& value)
        {
            data_ptr_->address_ = value;
        }

        // 设置Id
        void SetId(const std::string& value)
        {
            data_ptr_->id_ = value;
        }

        // 设置元信息
        void SetMeta(const std::string& key, const std::string& value)
        {
            data_ptr_->metas_.insert(std::make_pair(key, value));
        }

        // 删除元信息
        void RemoveMeta(const std::string& key)
        {
            data_ptr_->metas_.erase(key);
        }

        // 设置服务端口
        void SetPort(unsigned int value)
        {
            data_ptr_->port_ = value;
        }

        // 移除标签
        void RemoveTag(const std::string& value)
        {
            data_ptr_->tags_.erase(value);
        }

        // 添加标签
        void SetTag(const std::string& value)
        {
            data_ptr_->tags_.insert(value);
        }

        // 设置健康检查
        void SetCheck(const Check& check)
        {
            data_ptr_->checks_.insert(check);
        }

        // 移除健康检查项
        void RemoveCheck(const std::string& check_id)
        {
            discard_if(data_ptr_->checks_, [&check_id](Check obj)
            {
                return (obj.GetId() == check_id);
            });
            // set都是const对象无法使用remove_if
            //std::remove_if(std::begin(m_pData->m_setChecks),
            //               std::end(m_pData->m_setChecks),
            //               [&strId](Check obj) {
            //                    return (obj.GetId() == strId);
            //                });
        }
    private:
        std::unique_ptr<ServiceData> data_ptr_{ nullptr };
    };

    //////////////////////////////////////////////////////////////////////////
    static const std::string REGISTER_API("/v1/agent/service/register");
    static const std::string DEREGISTER_API("/v1/agent/service/deregister/");
    static const std::string KV_API("/v1/kv/");
    static const std::string HEALTH_CHECK_API("/v1/health/service/"); //http://127.0.0.1:8500/v1/health/service/atelier?tag=xxx&passing=1

    static std::function<bool(http_response)> NoReturnProcessor()
    {
        return [ = ](http_response resp)
        {
            std::cout << "status:" << resp.status_code() << std::endl;
            if (resp.status_code() == status_codes::OK)
            {
                return true;
            }

            std::string strError("Register responded body: ");
            strError += resp.extract_string(true).get();
            std::cout << strError << std::endl;
            return false;
            //throw std::exception("register request failed");
            // return pplx::task_from_result(json::value());
        };
    }

    class Consulpp
    {
        class Impl
        {
        public:
            Impl() = delete;
            Impl(std::string host, unsigned int port)
            {
                host_ = host;
                port_ = port;
                base_url = "http://" + host + ":" + std::to_string(port);
            }

            std::string host_{};
            unsigned int port_{ 0 };
            std::string base_url{};
        };

    public:
        Consulpp() = delete;
        ~Consulpp() = default;

        Consulpp(std::string host, unsigned int port)
        {
            data_ptr_ = std::make_unique<Impl>(host, port);
        }

        // 向consul注册服务
        bool RegisterService(const ConsulService& service)
        {
            json::value jsonReqData;
            jsonReqData["ID"] = json::value::string(service.GetId());
            jsonReqData["Name"] = json::value::string(service.GetName());
            jsonReqData["Port"] = json::value::number(service.GetPort());

            // 添加Metas
            json::value jsonMetas;
            const auto& mapMetas = service.GetMeta();
            std::for_each(std::begin(mapMetas), std::end(mapMetas),
                          [&jsonMetas](std::pair<std::string, std::string> item)
            {
                jsonMetas[item.first] = json::value::string(item.second);
            });
            jsonReqData["Meta"] = jsonMetas;

            // 添加tags
            json::value jsonTags;
            const auto& setTags = service.GetTags();
            size_t nTagsCount = 0;
            std::for_each(std::begin(setTags), std::end(setTags),
                          [&jsonTags, &nTagsCount](std::string strTag)
            {
                jsonTags[nTagsCount++] = json::value::string(strTag);
            });

            // 添加checks
            json::value jsonChecks;
            // 构造单个check的json对象
            auto ParseCheck = [](const Check & check)
            {
                json::value jsonCheck;
                jsonCheck["ID"] = json::value::string(check.GetId());
                jsonCheck["Name"] = json::value::string(check.GetName());
                jsonCheck["TCP"] = json::value::string(check.GetTcp());
                jsonCheck["Interval"] = json::value::string(check.GetInterval());
                jsonCheck["Timeout"] = json::value::string(check.GetTimeout());
                jsonCheck["Notes"] = json::value::string(check.GetNote());
                return jsonCheck;
            };
            const auto& setChecks = service.GetChecks();
            size_t nChecksCount = 0;
            std::for_each(std::begin(setChecks), std::end(setChecks),
                          [&jsonChecks, &nChecksCount, &ParseCheck](Check check)
            {
                jsonChecks[nChecksCount++] = ParseCheck(check);
            });

            jsonReqData["Tags"] = jsonTags;
            jsonReqData["Checks"] = jsonChecks;

            // 请求consul api
            http_client client(U(data_ptr_->base_url));
            uri_builder builder(U(REGISTER_API));

            auto resp = client.request(methods::PUT, builder.to_string(), jsonReqData).then(NoReturnProcessor());
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
        bool Deregister(std::string service_id)
        {
            // 请求consul api
            http_client client(U(data_ptr_->base_url));
            uri_builder builder(U(DEREGISTER_API));
            builder.append_path(service_id);

            auto resp = client.request(methods::PUT, builder.to_string()).then(NoReturnProcessor());
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

        std::string GetValue(const std::string& key)
        {
            // 请求consul api
            http_client client(U(data_ptr_->base_url));
            uri_builder builder(U(KV_API));
            builder.append_path(key);
            // 直接返回原始值，不编码
            builder.append_query(U("raw"));

            auto resp = client.request(methods::GET, builder.to_string()).then([](http_response resp)
            {
                std::cout << "status:" << resp.status_code() << std::endl;
                if (resp.status_code() == status_codes::OK)
                {
                    return resp.extract_string(true);
                }
                std::string strError("Register responded body:");
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

        bool SetValue(const std::string& key, const std::string& value)
        {
            // 请求consul api
            http_client client(U(data_ptr_->base_url));
            uri_builder builder(U(KV_API));
            builder.append_path(key);

            auto resp = client.request(methods::PUT, builder.to_string(), value).then(NoReturnProcessor());
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

        bool DeleteValue(std::string key)
        {
            http_client client(U(data_ptr_->base_url));
            uri_builder builder(U(KV_API));
            builder.append_path(key);

            auto resp = client.request(methods::DEL, builder.to_string()).then(NoReturnProcessor());
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

        bool HealthCheck(const std::string& service_name, const std::string& tag_filter, std::unordered_set<ConsulService>& health_services)
        {
            // 请求consul api
            http_client client(U(data_ptr_->base_url));
            uri_builder builder(U(HEALTH_CHECK_API));

            builder.append_path(service_name);
            builder.append_query("tag", tag_filter);
            builder.append_query("passing", "1");

            auto resp = client.request(methods::GET, builder.to_string()).then(NoReturnProcessor());
            try
            {
                auto json_value = resp.get();
                //TODO:分析返回的json到ConsulService结构中


                return true;
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                return false;
            }

            return true;
        }

    private:
        std::unique_ptr<Impl> data_ptr_{ nullptr };
    };

} // namespace consulpp

namespace std
{

    //self hash function
    template <>
    struct hash<consulpp::Check>
    {
        size_t operator()(const consulpp::Check& obj) const
        {
            return hash<std::string>()(obj.GetId());
        }
    };

    template <>
    struct hash<consulpp::ConsulService>
    {
        size_t operator()(const consulpp::ConsulService& obj) const
        {
            return hash<std::string>()(obj.GetId());
        }
    };

} // namespace std
