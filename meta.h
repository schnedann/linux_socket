#ifndef META_H
#define META_H

#include <cstdint>
#include <map>
#include <type_traits>

namespace meta {

//----- Simple Math

template <typename T, T i, T j> struct Multiply {
  static const T value = i * j;
};

template <typename T, T x, T y> struct CeilDiv {
  static const T value = (x % y) ? x / y + 1 : x / y;
};

//-----

//https://www.c-plusplus.net/forum/topic/217911/template-meta-programmierung-quadratwurzel-use-of-undefined-type-error/3
//https://de.wikipedia.org/wiki/Heron-Verfahren

namespace sqrt_detail {

  template<typename T, uint8_t I, T A, T X0, T AX > struct HeronStep {
      static_assert(std::is_unsigned<T>(),"Type is not unsigned");
      static const T x1 = ((X0+AX)/2);
      static const T value = HeronStep<T, I-1, A, x1, (A/x1) >::value;
  };

template<typename T, T A, T X0, T AX > struct HeronStep<T, 0, A, X0, AX > {
    static_assert(std::is_unsigned<T>(),"Type is not unsigned");
    static const T value = X0;
};

} //namespace

template<typename T, T A > struct SquareRoot_Heron {
  static_assert(std::is_unsigned<T>(),"Type is not unsigned");
  static const T value = sqrt_detail::HeronStep<T, 5, A, 2, (A/2) >::value;
};

//----- fibonacci
/*
namespace fibonacci_detail {

template<typename T, T X> struct Fibonacci_Step {
  static const int result = Fibonacci_Step<T,X-1>::result + Fibonacci_Step<T,X-2>::result;
};

template<typename T> struct Fibonacci_Step<T,1> {
    static const int result = T(1);
};

template<typename T> struct Fibonacci_Step<T,2> {
    static const int result = T(1);
};

}//namespace

template<typename T, T X> struct Fibonacci {
  static_assert(std::is_unsigned<T>(),"Type is not unsigned");
  static_assert(X>2,"Parameter X too small");
  static const int result = fibonacci_detail::Fibonacci_Step<T,X-1>::result + fibonacci_detail::Fibonacci_Step<T,X-2>::result;
};
*/
//-----

/**
 * A compile Time "inteligent Array" which at least knows its number of Elements
 */
template <typename T, size_t N> struct Array {
  T value [N];
  constexpr static const size_t size = N;
};

//-----



//-----

namespace Types {

template <typename U> struct remove_pointer { // in general case
  using type = U;
};

template <typename T> struct remove_pointer<T*> { // for U = T*
  using type = T;
};

template <typename W> using RemovePointer = typename remove_pointer<W>::type;

} //namespace

//-----

namespace Initialize {

// A class template that just a list of types:
template <class... T> struct typelist {
  //static size_t value = sizeof...(T...);
};

//-----

/*template <int I> struct squared {
  squared<I - 1> rest;
  static const int x = I * I ;
  constexpr int operator[](int const &i) const { return (i == I ?  x : rest[i]); }
  constexpr int size() const { return I; }
};

template <> struct squared<0> {
  static const int x = 0;
  constexpr int operator[](int const &i) const { return x; }
  constexpr int size() const { return 1; }
};*/

//-----

/*
template<typename T, T N> struct SqArr {
  static inline void fill(T arr[]) {
    arr[N-1] = (N-1)*(N-1);
    SqArr<T, N-1>::fill(arr);
  }
};

template<typename T> struct SqArr<0<T> > {
  static inline void fill(T arr[]) { }
};*/

//-----

namespace linspace_detail {

template<typename T, size_t RC, T S, T E, size_t C> struct linspace {
  static const T value;
  /*if(RC==C){
    value = S;
  }else{
    value = linspace<T,RC-1,S,E,C>::value;
  }*/

};

template<typename T, T S, T E, size_t C> struct linspace<T, 1, S, E, C> {
  static const T value = E;
};

}//namespace

} //namespace

//-----

namespace Eval {

template< bool CONDITION, class THEN, class ELSE > struct IF {};

template<class THEN, class ELSE> struct IF< false, THEN, ELSE > {
  using result = ELSE ;
};

template<class THEN, class ELSE> struct IF< true, THEN, ELSE > {
  using result = THEN;
};

template< bool CONDITION, class THEN, class ELSE > using IF_ELSE = typename IF<CONDITION,THEN,ELSE>::result;

//-----

} //namespace

} //namespace

#endif // META_H
