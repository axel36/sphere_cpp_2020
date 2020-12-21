#include <iostream>
#include <memory>
#include <map>
#include <sys/epoll.h>
#include <list>
#include <unordered_map>
#include "net.hpp"
#include "tcp.hpp"

tcp::Connection GetCon(){
  return {desc::Descriptor{7777}, "", 1};
}

void Test(tcp::Connection& con, int i){
  con.set(i);
}

int main() {

  std::map<int, tcp::Connection> m;

  std::list<std::unique_ptr<int>> l;
  std::unique_ptr<int> p = std::make_unique<int>(4);
  std::cout << "a: " << p.get() << std::endl;

  l.push_back(std::move(p));

  tcp::Connection c{desc::Descriptor{3}, "", 1};

  Test(c, 33);

  m.emplace(1, std::move(c));

  Test(m[1], 55);
  auto dd = std::move(m.at(1));

//  std::unique_ptr<int> p2 = std::move(p);

  std::cout << "a: " << p.get() << std::endl;
  std::cout << "l: " << l.front().get() << std::endl;



  for (auto& el : l){
    std::cout << "el: " << &el << std::endl;
    auto x = &el;

    std::cout << "l: " << l.front().get() << std::endl;
  }

  std::cout << "l: " << l.front().get() << std::endl;

  std::cout << "____________"<< std::endl;



  std::map<int, std::shared_ptr<tcp::Connection>> m3;
//  std::map<int, std::string> m2;

//  std::list<tcp::Connection> l;
//
  auto con = std::make_shared<tcp::Connection>(GetCon());
//  auto con2 = std::make_unique<tcp::Connection>(desc::Descriptor{5}, "rr", 1);
//

  m3[11]= con;

//
//

//  l.push_back(c);
//  auto con = GetCon();
//  m[con->GetSocket()] = std::move(con);
//
  ::epoll_event event{};
  event.data.ptr = &m3[11];

//  std::cout << ";las,d";


//  m[3] = std::move(con);
//  m[4] = std::move(tcp::Connection{desc::Descriptor{7}, "777", 7777});


//  m.emplace(3, tcp::Connection{desc::Descriptor{7}, "777", 7777});

//  auto xx =  m3[3].get();
//  std::cout << "&m[11]: " << m3[11].get() << std::endl;
//  std::cout << "&m[3]: " << &m3[100] << std::endl;
//
////  for(int i = 10; i < 10000000; i++){
////    m3[i] = std::make_shared<tcp::Connection>(desc::Descriptor{i+100}, "", 1);
////  }
//
//  std::cout << "&m[11]: " << &m3[11] << std::endl;
//  std::cout << "&m[3]: " << &m3[100] << std::endl;
}
