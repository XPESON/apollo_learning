#ifndef CYBER_BASE_UNDOUNDED_QUEUE_H
#define CYBER_BASE_UNBOUNDED_QUEUE_H_

#include <unistd.h>

#include <atomic>
#include <cstdint>
#include <memory>

namespace apollo {
namespace cyber {
namespace base {

template <typename T>
class UnboundedQueue {
  public:
    UnboundedQueue() { Reset(); }
    UnboundedQueue& operator=(const UnboundedQueue& other) = delete;
    UnboundedQueue(const UnboundedQueue& other) = delete;

    ~UnboundedQueue() { Destroy(); }

    void Clear() {
      Destroy();
      Reset();
    }

    void Enqueue(const T& element) {
      auto node = new Node();
      node->data = element;
      Node* old_tail = tail_.load();
      /*
      * 较 tail_ 的当前值和 old_tail 的值是否相等，如果相等，
      * 则将 tail_ 的值更新为 node，并返回 true，表示替换成功；如果不相等，
      * 则将 tail_ 的当前值存储到 old_tail 中，并返回 false，表示替换失败。
      */
      while (true) {
        if(tail_.compare_exchange_strong(old_tail, node)) {
          old_tail->next = node;
          old_tail->release();
          size_.fetch_add(1);
          break;
        }
      } 
    }

    bool Dequeue(T* element) {
      Node* old_head = head_.load();
      Node* head_next = nullptr;
      do {
        head_next = old_head->next;
        if(head_next == nullptr) {
            return false;
        }
      } while(!head_.compare_exchange_strong(old_head, head_next));
      *element = head_next->data;
      size_.fetch_sub(1);
      old_head->release();
      return true;
    }
    

    size_t Szie() {return size_.load(); }
    bool Empty() { return size_.load() == }

  private:
    struct Node {
      std::atomic<uint32_t> ref_count;
      Node* next = nullptr;
      Node() {ref_count.store(2); }
      void release() {
        ref_count.fet_sub(1);
        if(ref_count.load() == 0) {
          delete this;
        }
      }
    };

    void Reset() {
      auto node = new Node();
      head_.store(node);
      tail_.store(node);
      size_.store(0);
    }

    void Destory() {
      auto iter = head_.load();
      Node* tmp = nullptr;
      while(iter != nullptr) {
        tmp = iter->next;
        delete iter;
        iter = tmp;
      }
    }


    std::atomic<Node*> head_;
    std::atomic<Node*> tail_;
    std::atomic<size_t> size_;
};



}
}
}




#endif