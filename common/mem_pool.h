#pragma once

#include <cstddef>
#include <memory>
#include <vector>

template <typename T>
class MemPool final {
public:
    // Constructor — pre-allocate num_elems slots upfront
    explicit MemPool(std::size_t num_elems) {
        store_.resize(num_elems);
        is_free_.resize(num_elems, true);
    }

    // allocate — find free slot, construct object there
    template <typename... Args>
    T* allocate(Args&&... args) noexcept {
        for (std::size_t i = 0; i < is_free_.size(); i++) {
            if (is_free_[i]) {
                is_free_[i] = false;
                return std::construct_at(&store_[i],
                       std::forward<Args>(args)...);
            }
        }
        return nullptr;  // pool is full
    }

    // deallocate — return slot back to pool
    void deallocate(const T* elem) noexcept {
        std::size_t index = elem - &store_[0];
        is_free_[index] = true;
        std::destroy_at(elem);
    }

    // Delete copy/move — pool owns its memory
    MemPool() = delete;
    MemPool(const MemPool&) = delete;
    MemPool(const MemPool&&) = delete;
    MemPool& operator=(const MemPool&) = delete;
    MemPool& operator=(const MemPool&&) = delete;

private:
    std::vector<bool> is_free_;  // tracks free slots
    std::vector<T>    store_;    // actual objects
};
