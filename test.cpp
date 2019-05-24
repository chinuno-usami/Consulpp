#include <iostream>
#include "consulpp.hpp"

void show_menu()
{
  std::cout << "1.Register service" << std::endl;
  std::cout << "2.delete service" << std::endl;
  std::cout << "3.set value" << std::endl;
  std::cout << "4.delete value" << std::endl;
  std::cout << "5.get value" << std::endl;
  std::cout << "6.health check" << std::endl;

  std::cout << "0.exit" << std::endl;
}

void register_service(consulpp::Consulpp& ctx)
{
  consulpp::ConsulService service;
  service.SetAddress("127.0.0.1");
  service.SetId("test_service_id");
  service.SetMeta("version", "1.2.3");
  service.SetMeta("test_meta", "test_meta_value");
  service.SetName("client_test_servie");
  service.SetPort(22);
  service.SetTag("client");
  service.SetTag("test");

  consulpp::Check check;
  check.SetId("client_test_check");
  check.SetInterval("5s");
  check.SetName("Client test check");
  check.SetTcp("127.0.0.1:22");
  check.SetTimeout("1s");
  check.SetNote("test note");
  service.SetCheck(check);

  ctx.RegisterService(service);
}

void delete_service(consulpp::Consulpp& ctx)
{
  ctx.Deregister("test_service_id");
}

void set_value(consulpp::Consulpp& ctx)
{
  ctx.SetValue("client/client_test", "test");
}

void delete_value(consulpp::Consulpp& ctx)
{
  ctx.DeleteValue("client/client_test");
}

void get_value(consulpp::Consulpp& ctx)
{
  std::cout << ctx.GetValue("client/client_test") << std::endl;
}

void health_check(consulpp::Consulpp& ctx, const std::string& service_name, const std::string& tag_filter)
{
  std::vector<consulpp::ConsulService> health_services;
  if (ctx.HealthCheck(service_name, tag_filter, health_services))
  {
    //print service info
  }
  else
  {
    std::cout << "health check failed, please test it" << std::endl;
  }
}

int main()
{
  consulpp::Consulpp ctx("127.0.0.1", 8500);
  while (true)
  {
    show_menu();

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
      health_check(ctx, "client_test_servie", "test");
    case '0':
      exit(0);
      break;
    default:
      break;
    }
  }

  return 0;
}
