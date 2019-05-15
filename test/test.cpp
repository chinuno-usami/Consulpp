#include <iostream>
#include "Consulpp.h"
#include "ConsulService.h"
#include "Check.h"
using namespace std;
void menu() {
    cout << "1.Register service" << endl;
    cout << "2.delete service" << endl;
    cout << "3.set value" << endl;
    cout << "4.delete value" << endl;
    cout << "5.get value" << endl;
    cout << "6.exit" << endl;
}
void register_service(CConsulpp& ctx) {
    CConsulService service;
    service.SetAddress("121.121.1.186");
    CCheck check;
    check.SetId("client_test_check");
    check.SetInterval("5s");
    check.SetName("Client test check");
    check.SetTcp("121.121.1.186:22");
    check.SetTimeout("1s");
    check.SetNote("test note");
    service.SetCheck(check);
    service.SetId("test_service_id");
    service.SetMeta("version", "1.2.3");
    service.SetMeta("test_meta", "test_meta_value");
    service.SetName("client_test_servie");
    service.SetPort(22);
    service.SetTag("client");
    service.SetTag("test");
    ctx.RegisterService(service);
}
void delete_service(CConsulpp& ctx) {
    ctx.Deregister("test_service_id");
}
void set_value(CConsulpp& ctx) {
    ctx.SetValue("client/client_test", "test");
}
void delete_value(CConsulpp& ctx) {
    ctx.DeleteValue("client/client_test");
}
void get_value(CConsulpp& ctx) {
    cout << ctx.GetValue("client/client_test") << endl;;
}
int main() {
    CConsulpp ctx("121.121.1.195",8500);
    while (true)
    {
        menu();
        switch (getchar())
        {
        case '1':
            register_service(ctx);
            break;
        case '2':
            delete_service(ctx);
            break;
        case '3':
            set_value(ctx);
            break;
        case '4':
            delete_value(ctx);
            break;
        case '5':
            get_value(ctx);
            break;
        case '6':
            exit(0);
            break;
        default:
            break;
        }
    }
}