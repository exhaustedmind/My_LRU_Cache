#include "LRU_Cache.h"

LRU_Cache::LRU_Cache(size_t capacity) : capacity_(capacity) {
  // TODO: also makes sense to reserve that capacity for std::unordered_map,
  //       i.e. call accessStorage_.reserve(capacity) here

}

size_t LRU_Cache::size() const {
    // TODO: this and other short functions could be defined in header to be inlined
    return size_;
}

void LRU_Cache::put(const std::string &key, int value) {
    auto it = accessStorage_.find(key);
    if (it != accessStorage_.end()) {
        it->second->value = value;
        itemList_.splice(itemList_.begin(), itemList_, it->second);
    } else {
        if (size_ >= capacity_) {
            accessStorage_.erase(itemList_.back().key);
            itemList_.pop_back();
            --size_;
        }
    }

    itemList_.emplace_front(key, value);
    accessStorage_[key] = itemList_.begin();
    ++size_;
}

void LRU_Cache::removeElement(const std::string &key) {
    auto it = accessStorage_.find(key);
    if (it != accessStorage_.end()) {
        itemList_.erase(it->second);
        accessStorage_.erase(key);
    }
}

bool LRU_Cache::isEmpty() const {
    return size_ == 0;
}

void LRU_Cache::clear() {
    itemList_.clear();
    accessStorage_.clear();
    size_ = 0;
}

LRU_Cache::LRU_Cache(const LRU_Cache &other) : size_(other.size_), capacity_(other.capacity_) {
    itemList_ = other.itemList_;

    // TODO: code below could be just a for loop
    auto it = itemList_.begin();

    while (it != itemList_.end()) {
        accessStorage_[it->key] = it;
        ++it;
    }
}

LRU_Cache &LRU_Cache::operator=(const LRU_Cache &other) {
    if (this != &other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        itemList_.clear();
        accessStorage_.clear();
        // TODO: would `itemList = other.itemList_;` work?
        for (const auto &item: other.itemList_) {
            itemList_.push_back(item);
        }
        for (auto it = itemList_.begin(); it != itemList_.end(); ++it) {
            accessStorage_[it->key] = it;
        }
    }
    return *this;
}

LRU_Cache::LRU_Cache(LRU_Cache &&other) noexcept: size_(other.size_), capacity_(other.capacity_),
                                                  itemList_(std::move(other.itemList_)),
                                                  accessStorage_(std::move(other.accessStorage_)) {
    other.size_ = 0;
}

LRU_Cache &LRU_Cache::operator=(LRU_Cache &&other) noexcept {
    if (this != &other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        itemList_ = std::move(other.itemList_);
        accessStorage_ = std::move(other.accessStorage_);
        other.size_ = 0;
    }
    return *this;
}

// TODO: or simply `= default` in the header
LRU_Cache::~LRU_Cache() {

}

LRU_Cache::Iterator LRU_Cache::begin() {
    return Iterator(itemList_.begin());
}

LRU_Cache::Iterator LRU_Cache::end() {
    return LRU_Cache::Iterator(itemList_.end());
}
