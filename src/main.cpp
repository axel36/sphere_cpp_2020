#include <chrono>
#include <iostream>
#include <optional>
#include <map>

class Cls{
public:
  Cls(){std::cout << "hello" << std::endl;};
  Cls(const Cls &other){std::cout << "&ctr"<< std::endl;;}
  Cls(Cls &&other){std::cout << "&&ctr"<< std::endl;}
  Cls &operator=(const Cls& other){std::cout << "&="<< std::endl;
  return *this;}

  Cls &operator=(const Cls&& other){std::cout << "&&="<< std::endl;
  return *this;}

  Cls &operator=(Cls&& other){std::cout << "&&="<< std::endl;
    return *this;}

  ~Cls(){std::cout <<"bye"<<std::endl;}
};

struct S{
  Cls c;
};

struct S2{
  Cls c;
};

Cls bar(){

  return Cls{};
}

std::optional<S2> baz(){
  S2 s;
  s.c = Cls{};
  return s;
}



enum class DiceRoll : int {};
enum class DiceMax : int {};

void foo(DiceRoll x, DiceMax y){

  std::cout << static_cast<int>(x) << static_cast<int>(y);
}


int main() {

  std::map<int, std::string> mm{{1,"q"}, {100, "w"}, {1000, "e"}};


//  std::cout << mm.lower_bound(110)->second;

  std::cout << 0. + 0.01;


//  foo(DiceRoll{3}, DiceMax{10});
//
//  const int& x = 5;
//
//
//  std::cout << &x << std::endl;
//
////  std::optional<Cls> o;
////  {
////    o = Cls{};
////  }
//  std::cout << "----------" << std::endl;
//
////  Cls d{};
////  std::optional<Cls> o2 = std::move(d);
//
//
////  std::optional<Cls> o3 = foo();
//
//  S s;
//
//  std::cout << "----------" << std::endl;
//
//  std::optional<S2> cli = S2{};
//  std::cout << "----------" << std::endl;
//  s.c = std::move(cli->c);
//
//  std::optional<Cls> o4;
  return 0;
}
