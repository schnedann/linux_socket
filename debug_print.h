#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <typeinfo>
#include <limits>

#include "dtypes.h"
#include "meta.h"

namespace Debug {

//-----

template<typename T> struct bytecaster{

  using type = meta::Eval::IF_ELSE<(typeid(T).hash_code() == typeid(u8).hash_code()),u16,
               meta::Eval::IF_ELSE<(typeid(T).hash_code() == typeid(s8).hash_code()),s16,
               T>>;
};

//-----

template<typename T> std::string to_string(std::string name, T x){
  std::stringstream ss;
  ss << name << ": " << x;
  return ss.str();
}

#define PRNVAR(_X) (to_string<bytecaster<decltype(_X)>::type>((#_X),(_X)))

//-----

template<typename T> std::string to_hstring(std::string name, T x){
  std::stringstream ss;
  bool is_int = std::numeric_limits<T>::is_integer;
  if(is_int){
    ss << name << ": " << "0x" << std::hex << x;
  }else{
    ss << name << ": " << x;
  }
  return ss.str();
}

#define PRNHEXVAR(_X) (to_hstring<bytecaster<decltype(_X)>::type>((#_X),(_X)))

//-----

#define DBGOUT(str) {cout<<(str)<<"\n";}

//-----

/*
class debug_print
{
public:
  debug_print();
};*/

}//namespace

#endif // DEBUG_PRINT_H
