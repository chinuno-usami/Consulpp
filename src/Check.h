#pragma once
#include <string>
#include <memory>
class CCheckData;
class CCheck
{
public:
    CCheck();
    CCheck(const CCheck& other);
    CCheck(CCheck&& other);
    ~CCheck();
    bool operator ==(const CCheck& rhs) const;
    CCheck& operator =(const CCheck& rhs);
    CCheck& operator =(CCheck&& rhs);
    void SetId(const std::string& strId);
    void SetName(const std::string& strName);
    void SetNote(const std::string& strNote);
    void SetInterval(const std::string& strInterval);
    void SetTcp(const std::string& strTcp);
    void SetTimeout(const std::string& strTimeout);
    std::string GetId() const;
    std::string GetName() const;
    std::string GetNote() const;
    std::string GetTcp() const;
    std::string GetTimeout() const;
    std::string GetInterval() const;
private:
    std::unique_ptr<CCheckData> m_pData;
};

namespace std
{
template<>
struct hash<CCheck>
{
    size_t operator()(const CCheck& obj) const {
        return hash<std::string>()(obj.GetId());
    }
};
}
