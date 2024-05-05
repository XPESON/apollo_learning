#ifndef CYBER_BASE_MACROS_H_
#define CYBER_BASE_MACROS_H_

#include <cstdlib>
#include <new>

#if __GNUC__ >= 3
/**
 * __builtin_expect((x), 1) 表示 x 很可能为真。
 * 当条件 x 确实很可能为真时，编译器会做出相应的优化，
 * 更倾向于执行条件为真的分支，从而提高程序的执行效率
*/
#define cyber_likely(x) (__builtin_expect((x), 1))
#define cyber_unlikely(x) (_builtin_expect(x), 0) 
#else
#define cyber_likely(x) (x)
#define cyber_unlikely(x) (x)
#endif

#define CACHELINE_SIZE 64
/**
 * 这个宏定义了一个类型特征（type trait），用于判断一个类是否具有特定的成员函数或成员变量。
 * 宏展开后，生成了一个模板结构体 name，该结构体包含一个静态成员变量 value，表示类型特征的取值。
 * 结构体中定义了一个 Test 函数模板，该函数使用 SFINAE 技术（Substitution Failure Is Not An Error）
 * 对给定类型的成员函数进行检查。如果该类型具有名为 func 的成员函数，则重载的 Test 函数模板接受指向该成员函数的指针，并返回 true；
 * 否则，接受任意其他参数并返回 false。最后，通过 constexpr 关键字定义了静态成员变量 value，其值由 Test 函数的调用结果确定。
*/
#define DEFINE_TYPE_TRAIT(name, func)                        \
  template <typename T>                                      \
  struct name {                                              \
    template <typename Class>                                \
    static constexpr bool Test(decltype(&Class::func)*) {    \
      return true;                                                \
    }                                                        \
    template <typename>                                      \
    static constexpr bool Test(...) {                        \  
      return false;                                          \
    }                                                        \
                                                             \
    static constexpr bool value = Test<T>(nullptr);          \
  };                                                         \
                                                             \
  template <typename T>                                      \
  constexpr bool name<T>::value;                             \

  inline void cpu_relax() {
  #if defined(__aarch64__)
    asm volatile("yield" ::: "memory");
  #else  
    asm volatile("rep; nop" ::: "memory");
  #endif
  }

  inline void* CheckedMalloc(size_t size) {
    void* ptr = std::malloc(size);
    if(!ptr) {
      throw std::bad_alloc();
    }
    return ptr;
  } 

  inline void* CheckedCalloc(size_t num, size_t size) {
    /**calloc 函数接受两个参数：num 表示要分配的元素数量，
     * size 表示每个元素的大小（以字节为单位）。它会为 num * size 个字节的内存分配空间，并将所有字节的值都设置为零。
     * 使用 calloc 函数可以确保分配的内存在返回给用户之前被清零，这对于需要初始化内存以避免未定义行为的情况非常有用。
     * 例如，对于某些数据结构或缓冲区，如果不进行初始化，可能会导致程序在使用时产生意外的结果。
     * 因此，calloc 是一个安全的内存分配函数，可以确保分配的内存在使用之前已经被初始化为零。
    */
    void* ptr = std::calloc(num, size);
    if(!ptr) {
      throw std::bad_alloc();
    }
    return ptr;
  }
#endif

/** 知识点1：SFINAE技术
 * SFINAE 是 "Substitution Failure Is Not An Error" 的缩写，
 * 意为 "替换失败不是错误"。它是一种 C++ 模板元编程技术，用于在编译期间根据模板参数进行条件编译和代码选择。
 * 在 C++ 中，当使用模板实例化时，编译器会尝试根据模板参数来替换模板中的类型或表达式，生成具体的代码。
 * 如果在替换过程中出现了错误，通常会导致编译器报错。然而，SFINAE 技术利用了 C++ 模板的特性，即在模板参数替换失败时不会报错，
 * 而是会继续尝试其他的模板候选项，以避免编译错误。SFINAE 技术通常与模板重载和模板特化一起使用，通过设计多个模板重载，
 * 每个重载对应不同的条件，并利用模板参数的替换失败来选择合适的重载，从而达到条件编译和代码选择的目的。
 * 常见的应用场景包括类型特征的检查、模板元编程中的函数选择、模板类的部分特化等。SFINAE 技术在 C++11 标准中得到了进一步的支持，
 * 并在现代 C++ 编程中被广泛应用。
*/

/**知识点2： constexpr关键字
 * https://blog.csdn.net/crossoverpptx/article/details/128492901
*/