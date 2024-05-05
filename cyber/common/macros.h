#ifndef CYBER_COMMON_MACROS_H_
#define CYBER_COMMON_MACROS_H_

#include <iostream>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include "cyber/base/macros.h"

DEFINE_TYPE_TRAIT(HasShutdown, Shutdown)
/**
 * 如果类型 T 具有 Shutdown 成员函数（即 HasShutdown<T>::value 为 true），那么 
 * std::enable_if<HasShutdown<T>::value>::type 将会被定义为有效的类型。否则，由于模板参数推断失败，这个函数将被 SFINAE 排除在外。
*/
template <typename T>
typename std::enable_if<HasShutdown<T>::value>::type CallShutdown(T* instance) {
  instance->Shutdown();
}

template <typename T>
typename std::enable_if<!HasShutdown<T>::value>::type CallShutdown(T* instance) {
  (void)instance;
}

#undef UNUSED
#undef DISALLOW_COPY_AND_ASSIGN

#define UNUSED(param) (void)param

#define DISALLOW_COPY_AND_ASSIGN(classname)           \
  classname(const classname &) = delete;              \
  classname& operator=(const classname&) = delete;

//这段代码尝试使用 std::call_once 函数来保证多线程环境下只有一个线程执行某个任务。具体来说，它的作用是：
//1、定义了一个静态的 std::once_flag 变量 flag，用于标记是否已经执行过某个任务。
//2、使用 std::call_once 函数来执行一个 lambda 函数，该 lambda 函数中创建了一个对象实例，但只会在第一次调用时执行。
#define DECLARE_SINGLETON(classname)                                              \
  public:                                                                         \
    statc classname* instance(bool create_if_needed = true) {                     \
      static classname* instance = nullptr;                                       \
      if(!instance && create_if_needed) {                                         \                                       
        static std::once_flag flag;                                               \
        std::call_once(flag, [&]{ instance = new (std::nothrow) classname(); });  \
      }                                                                           \
      return instance;                                                            \
    }                                                                             \
                                                                                  \
    static void CleanUp() {                                                       \
      auto instance = Instance(false);                                            \
      if(instance != nullptr) {                                                   \
        CallShutdown(instance);                                                   \
      }                                                                           \
    }                                                                             \
                                                                                  \
  private:                                                                        \
    classname();                                                                  \
    DISALLOW_COPY_AND_ASSIGN(classname);


#endif
/**知识点：std::enable_if<>
 * std::enable_if 是一个模板元编程工具，用于根据条件来控制模板的有效性。它提供了一种在编译时根据条件来选择不同实现的机制。
 * 具体地说，std::enable_if 可以根据一个布尔表达式来选择是否启用或禁用模板。当布尔表达式为 true 时，
 * std::enable_if 将定义一个内部的成员类型 type，表示模板实例有效，否则不会定义这个成员类型，从而使模板实例无效。
 * std::enable_if 主要用于两个地方：
 * 函数模板重载：可以根据条件选择是否启用不同版本的函数模板。
 * 类模板的成员函数：可以根据条件选择是否启用类模板的某个成员函数。
*/
