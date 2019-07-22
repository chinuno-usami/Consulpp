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
using namespace utility::conversions;

namespace consulpp {

namespace details {
// make_unique support for pre c++14
#if __cplusplus >= 201402L // C++14 and beyond
using std::make_unique;
#else
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
} // namespace details

class ConsulCheckData
{
public:
    ConsulCheckData() = default;
    ~ConsulCheckData() = default;

    std::string id_{};
    std::string name_{};
    std::string note_{};
    std::string timeout_{};
    std::string interval_{};
    std::string tcp_{};
};

class ConsulCheck
{
public:
    ConsulCheck()
    {
        data_ptr_ = details::make_unique<ConsulCheckData>();
    }

    ConsulCheck(const ConsulCheck& other)
    {
        data_ptr_ = details::make_unique<ConsulCheckData>(*other.data_ptr_);
    }

    ConsulCheck(ConsulCheck&& other)
    {
        data_ptr_.swap(other.data_ptr_);
    }

    ~ConsulCheck() = default;

    bool operator==(const ConsulCheck& rhs) const
    {
        return GetId() == rhs.GetId();
    }

    ConsulCheck& operator=(const ConsulCheck& rhs)
    {
        data_ptr_ = details::make_unique<ConsulCheckData>(*rhs.data_ptr_);
        return *this;
    }

    ConsulCheck& operator=(ConsulCheck&& rhs)
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
    std::unique_ptr<ConsulCheckData> data_ptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////
template<typename T, typename Comp, typename Alloc, typename Predicate>
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

struct ConsulCheckHasher
{
    size_t operator()(const ConsulCheck& obj) const
    {
        return std::hash<std::string>()(obj.GetId());
    }
};

struct ConsulCheckCmp
{
    size_t operator()(const ConsulCheck& lhs, const ConsulCheck& rhs) const
    {
        return lhs.GetId() == rhs.GetId();
    }
};

using ConsulCheckSet = std::unordered_set<ConsulCheck, ConsulCheckHasher, ConsulCheckCmp>;

class ConsulServiceData
{
public:
    ConsulServiceData() = default;
    ~ConsulServiceData() = default;

    std::string id_{};
    std::string name_{};
    std::string address_{};
    unsigned int port_{0};
    std::unordered_set<std::string> tags_;
    std::unordered_map<std::string, std::string> metas_;
    ConsulCheckSet checks_;
};

class ConsulService
{
public:
    ConsulService()
    {
        data_ptr_ = details::make_unique<ConsulServiceData>();
    }

    ConsulService(const ConsulService& other)
    {
        data_ptr_ = details::make_unique<ConsulServiceData>(*other.data_ptr_);
    }

    ConsulService(ConsulService&& other)
    {
        data_ptr_.swap(other.data_ptr_);
    }

    ~ConsulService() = default;

    const std::string& GetName() const
    {
        return data_ptr_->name_;
    }

    const std::string& GetAddress() const
    {
        return data_ptr_->address_;
    }

    const std::string& GetId() const
    {
        return data_ptr_->id_;
    }

    unsigned int GetPort() const
    {
        return data_ptr_->port_;
    }

    std::unordered_set<std::string>& GetTags() const
    {
        return data_ptr_->tags_;
    }

    const std::unordered_map<std::string, std::string>& GetMeta() const
    {
        return data_ptr_->metas_;
    }

    const ConsulCheckSet& GetChecks() const
    {
        return data_ptr_->checks_;
    }

    void SetName(const std::string& value)
    {
        data_ptr_->name_ = value;
    }

    void SetAddress(const std::string& value)
    {
        data_ptr_->address_ = value;
    }

    void SetId(const std::string& value)
    {
        data_ptr_->id_ = value;
    }

    void SetMeta(const std::string& key, const std::string& value)
    {
        data_ptr_->metas_.insert(std::make_pair(key, value));
    }

    void RemoveMeta(const std::string& key)
    {
        data_ptr_->metas_.erase(key);
    }

    void SetPort(unsigned int value)
    {
        data_ptr_->port_ = value;
    }

    void RemoveTag(const std::string& value)
    {
        data_ptr_->tags_.erase(value);
    }

    void SetTag(const std::string& value)
    {
        data_ptr_->tags_.insert(value);
    }

    void SetCheck(const ConsulCheck& check)
    {
        data_ptr_->checks_.insert(check);
    }

    void RemoveCheck(const std::string& check_id)
    {
        discard_if(data_ptr_->checks_, [&check_id](const ConsulCheck& obj) { return (obj.GetId() == check_id); });
    }

private:
    std::unique_ptr<ConsulServiceData> data_ptr_{nullptr};
};

struct ConsulServiceHasher
{
    size_t operator()(const ConsulService& service) const noexcept
    {
        return std::hash<std::string>()(service.GetId());
    }
};

struct ConsulServiceCmp
{
    bool operator()(const ConsulService& lhs, const ConsulService& rhs) const noexcept
    {
        return (lhs.GetId() == rhs.GetId());
    }
};

using ConsulServiceSet = std::unordered_set<ConsulService, ConsulServiceHasher, ConsulServiceCmp>;
//////////////////////////////////////////////////////////////////////////
static const std::string REGISTER_API("/v1/agent/service/register");
static const std::string DEREGISTER_API("/v1/agent/service/deregister/");
static const std::string KV_API("/v1/kv/");
static const std::string HEALTH_CHECK_API("/v1/health/service/"); // http://127.0.0.1:8500/v1/health/service/atelier?tag=xxx&passing=1

static std::function<bool(http_response)> NoReturnProcessor()
{
    return [=](http_response resp) {
#ifdef _DEBUG
        std::cout << "status:" << resp.status_code() << std::endl;
#endif
        if (resp.status_code() == status_codes::OK)
        {
            return true;
        }

        std::string error_msg("Register responded body: ");
        error_msg += to_utf8string(resp.extract_string(true).get());
        std::cout << error_msg << std::endl;
        return false;
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
        unsigned int port_{0};
        std::string base_url{};
    };

public:
    Consulpp() = delete;
    ~Consulpp() = default;

    Consulpp(std::string host, unsigned int port)
    {
        data_ptr_ = details::make_unique<Impl>(host, port);
    }

    bool RegisterService(const ConsulService& service)
    {
        json::value json_req_data;
        json_req_data[U("ID")] = json::value::string(to_string_t(service.GetId()));
        json_req_data[U("Name")] = json::value::string(to_string_t(service.GetName()));
        json_req_data[U("Address")] = json::value::string(to_string_t(service.GetAddress()));
        json_req_data[U("Port")] = json::value::number(service.GetPort());

        // add metas
        json::value json_metas;
        const auto& all_metas = service.GetMeta();
        std::for_each(std::begin(all_metas), std::end(all_metas), [&json_metas](std::pair<std::string, std::string> item) {
            json_metas[to_string_t(item.first)] = json::value::string(to_string_t(item.second));
        });
        json_req_data[U("Meta")] = json_metas;

        // add tags
        json::value json_tags;
        const auto& all_tags = service.GetTags();
        size_t tag_count = 0;
        std::for_each(std::begin(all_tags), std::end(all_tags),
            [&json_tags, &tag_count](const std::string& tag) { json_tags[tag_count++] = json::value::string(to_string_t(tag)); });

        // add checks
        json::value json_checks;
        auto ParseCheck = [](const ConsulCheck& check) {
            json::value json_check;
            json_check[U("ID")] = json::value::string(to_string_t(check.GetId()));
            json_check[U("Name")] = json::value::string(to_string_t(check.GetName()));
            json_check[U("TCP")] = json::value::string(to_string_t(check.GetTcp()));
            json_check[U("Interval")] = json::value::string(to_string_t(check.GetInterval()));
            json_check[U("Timeout")] = json::value::string(to_string_t(check.GetTimeout()));
            json_check[U("Notes")] = json::value::string(to_string_t(check.GetNote()));
            return json_check;
        };
        const auto& all_checks = service.GetChecks();
        size_t check_count = 0;
        std::for_each(std::begin(all_checks), std::end(all_checks),
            [&json_checks, &check_count, &ParseCheck](const ConsulCheck& check) { json_checks[check_count++] = ParseCheck(check); });

        json_req_data[U("Tags")] = json_tags;
        json_req_data[U("Checks")] = json_checks;

        http_client client(to_string_t(data_ptr_->base_url));
        uri_builder builder(to_string_t(REGISTER_API));

        auto resp = client.request(methods::PUT, builder.to_string(), json_req_data).then(NoReturnProcessor());
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

    bool Deregister(std::string service_id)
    {
        http_client client(to_string_t(data_ptr_->base_url));
        uri_builder builder(to_string_t(DEREGISTER_API));
        builder.append_path(to_string_t(service_id));

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
        http_client client(to_string_t(data_ptr_->base_url));
        uri_builder builder(to_string_t(KV_API));
        builder.append_path(to_string_t(key));
        builder.append_query(U("raw")); // raw

        auto resp = client.request(methods::GET, builder.to_string()).then([](http_response resp) {
#ifdef _DEBUG
            std::cout << "status:" << resp.status_code() << std::endl;
#endif
            if (resp.status_code() == status_codes::OK)
            {
                return resp.extract_string(true);
            }
            std::string error_msg("Register responded body:");
            error_msg += to_utf8string(resp.extract_string(true).get());
            std::cout << error_msg << std::endl;
            return pplx::task_from_result(utility::string_t());
        });
        try
        {
            return to_utf8string(resp.get());
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return std::string();
        }
    }

    bool SetValue(const std::string& key, const std::string& value)
    {
        http_client client(to_string_t(data_ptr_->base_url));
        uri_builder builder(to_string_t(KV_API));
        builder.append_path(to_string_t(key));

        auto resp = client.request(methods::PUT, builder.to_string(), to_string_t(value)).then(NoReturnProcessor());
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
        http_client client(to_string_t(data_ptr_->base_url));
        uri_builder builder(to_string_t(KV_API));
        builder.append_path(to_string_t(key));

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

    bool HealthCheck(const std::string& service_name, const std::string& tag_filter, ConsulServiceSet& health_services)
    {
        http_client client(to_string_t(data_ptr_->base_url));
        uri_builder builder(to_string_t(HEALTH_CHECK_API));

        builder.append_path(to_string_t(service_name));
        builder.append_query(U("tag"), to_string_t(tag_filter));
        builder.append_query(U("passing"), U("1"));

        // ucout << data_ptr_->base_url << builder.to_string() << std::endl;

        auto resp = client.request(methods::GET, builder.to_string()).then([](http_response resp) -> pplx::task<json::value> {
            if (resp.status_code() == status_codes::OK)
            {
#ifdef _DEBUG
                ucout << "request OK" << std::endl;
#endif
                return resp.extract_json();
            }

            return pplx::task_from_result(json::value());
        });

        try
        {
            const json::value& json_result = resp.get();
            // ucout << json_result.serialize() << std::endl;

            const json::array& node_array = json_result.as_array();
            for (auto iter = node_array.begin(); iter != node_array.end(); ++iter)
            {
                const json::object& node = iter->as_object();
                const json::object& service_data = node.at(U("Service")).as_object();
                ConsulService service;
                service.SetId(to_utf8string(service_data.at(U("ID")).as_string()));
                service.SetName(to_utf8string(service_data.at(U("Service")).as_string()));
                service.SetAddress(to_utf8string(service_data.at(U("Address")).as_string()));
                service.SetPort(service_data.at(U("Port")).as_integer());
                health_services.insert(service);
            }

            return true;
        }
        catch (const http_exception& e)
        {
            ucerr << e.what() << std::endl;
            return false;
        }
    }

private:
    std::unique_ptr<Impl> data_ptr_{nullptr};
};

} // namespace consulpp
