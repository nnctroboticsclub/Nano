#pragma once

#include <array>
#include <iterator>

namespace nano::collection {
template <typename T>
class LinkedListNode {
 public:
  [[nodiscard]] auto Prev() { return prev_; }
  void Prev(T* ptr) { prev_ = ptr; }

  [[nodiscard]] auto Next() { return next_; }
  void Next(T* ptr) { next_ = ptr; }

  [[nodiscard]] bool InUse() const { return in_use_; }
  void InUse(bool value) { in_use_ = value; }

  void ResetLinkNode() {
    prev_ = nullptr;
    next_ = nullptr;
  }

 private:
  bool in_use_ = false;
  T* prev_ = 0;
  T* next_ = 0;
};

template <typename T, std::size_t N>
class LinkedList {
 public:
  T* NewNode() {
    for (auto& node : nodes_) {
      if (!node.InUse()) {
        node.InUse(true);
        return &node;
      }
    }

    return nullptr;
  }

  class Iterator : public std::contiguous_iterator_tag {
    T* current_;

   public:
    explicit Iterator(T* node) : current_(node) {}

    T* operator*() { return current_; }

    Iterator& operator++() {
      current_ = current_->Next();
      return *this;
    }

    bool operator!=(const Iterator& other) {
      return current_ != other.current_;
    }
  };

  LinkedList() {
    for (auto& node : nodes_) {
      node.ResetLinkNode();
    }
  }

  auto head_node() { return head_; }
  auto tail_node() { return tail_; }

  auto begin() { return Iterator(head_); }
  auto end() { return Iterator(nullptr); }

  void Clear() {
    for (auto& node : nodes_) {
      node.InUse(false);
      node.ResetLinkNode();
    }
    head_ = nullptr;
    tail_ = nullptr;
  }

  void Remove(T* node) {
    node->InUse(false);

    if (node == head_) {
      head_ = node->Next();
    }

    if (node == tail_) {
      tail_ = node->Prev();
    }

    if (node->Prev() != nullptr) {
      node->Prev()->Next(node->Next());
    }

    if (node->Next() != nullptr) {
      node->Next()->Prev(node->Prev());
    }
  }

  T* NewBack() {
    auto new_node = NewNode();
    if (new_node == nullptr) {
      return nullptr;
    }

    if (head_ == nullptr) {
      head_ = new_node;
      tail_ = new_node;
      return new_node;
    }

    tail_->Next(new_node);
    new_node->Prev(tail_);
    tail_ = new_node;

    return new_node;
  }

 private:
  std::array<T, N> nodes_;
  T* head_ = nullptr;
  T* tail_ = nullptr;
};
}  // namespace nano::collection