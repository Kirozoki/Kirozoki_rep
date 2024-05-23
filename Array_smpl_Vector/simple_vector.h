#pragma once

#include "array_ptr.h"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <utility>

class ReserveProxyObj{

    public:
    ReserveProxyObj() = default;
    ReserveProxyObj(size_t capacity_to_reserve) : proxy_capacity(capacity_to_reserve) {}
    size_t GetCapacity() const {
        return proxy_capacity;
    }
    private:
    size_t proxy_capacity = 0;

};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
    using Iterator = Type*;
    using ConstIterator = const Type*;

public:
    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size, const Type& value = Type{}) : size_(size), capacity_(size) {
        if (size == 0) {
            SimpleVector();
        } else {
            items_ = ArrayPtr<Type>(size);
            std::fill(begin(), end(), value);
        }
    }

    SimpleVector(std::initializer_list<Type> init) {
        if (init.size() == 0) {
            SimpleVector();
        } else {
            size_t list_size = init.size();
            items_ = ArrayPtr<Type>(list_size);
            std::copy(init.begin(), init.end(), begin());
            size_ = list_size;
            capacity_ = list_size;
        }
    }

    SimpleVector(const SimpleVector& other) noexcept {
        assert(size_ == 0);
        SimpleVector tmp(other.GetSize());
        tmp.capacity_ = other.capacity_;
        std::copy(other.begin(), other.end(), tmp.begin());
        swap(tmp);
    }

    SimpleVector(SimpleVector&& other) {
        swap(other);
    }

    SimpleVector(ReserveProxyObj capacity_to_reserve) {
    Reserve(capacity_to_reserve.GetCapacity());
}

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            SimpleVector tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs) {
        if (this != &rhs) {
            SimpleVector temp(std::move(rhs));
            swap(temp);
        }
        return *this;
    }

    ~SimpleVector() {
    }

    Iterator begin() noexcept {return items_.Get();}
    Iterator end() noexcept {return items_.Get() + size_;}
    ConstIterator begin() const noexcept {return items_.Get();}
    ConstIterator end() const noexcept {return items_.Get() + size_;}
    ConstIterator cbegin() const noexcept {return items_.Get();}
    ConstIterator cend() const noexcept {return items_.Get() + size_;}

    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return items_[index];
    }

    const Type& operator[](size_t index) const noexcept{
        assert(index < size_);
        return items_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }
    
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index is out of range");
        }
        return items_[index];
    }

    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index is out of range");
        }
        return items_[index];
    }

    void Resize(size_t new_size) {
        if (new_size > capacity_) {
            size_t new_capacity = std::max(new_size, capacity_ * 2);
            ArrayPtr<Type> tmp(new_capacity);
            auto it = std::move(begin(), end(), tmp.Get()); // returns to tmp.element_past_copied
            //std::fill(it, it + (new_size - size_), Type{});
            auto it_end = it + (new_size - size_);
            for (; it != it_end; ++it) {
                *it = Type{};
            }
            items_.swap(tmp); 
            size_ = new_size;
            capacity_ = new_capacity;
            
        } else if (new_size > size_) {
            
            //std::fill(begin() + size_, begin() + new_size, Type{});
            auto it = begin() + size_;
            auto it_end = begin() + new_size;
            for (; it != it_end; ++it) {
                *it = Type{};
            }
            size_ = new_size;

        } else if (new_size < size_) {
            size_ = new_size; 
        }
    }

    void PopBack() noexcept {
        assert(size_ != 0u);
        --size_;
    }

    void PushBack(const Type& value) {
       Insert(end(), value); 
    }

    void PushBack(Type&& value) {
       Insert(end(), std::move(value)); 
    }

    Iterator Erase(ConstIterator pos) {
        std::move(std::next(Iterator (pos)), end(), Iterator(pos));
        --size_;
        return Iterator(pos);
    }

    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end());
        size_t index = pos - begin();        
        if (size_ < capacity_) {
            std::copy_backward(Iterator(pos), end(), end()); 
            *Iterator(pos) = value;            
        } else {
            size_t new_capacity = capacity_ * 2;
            ArrayPtr<Type> tmp(new_capacity);
            auto it = std::copy(begin(), Iterator(pos), tmp.Get());
            *it = value;
            std::copy(Iterator(pos), end(), std::next(it));
            items_.swap(tmp);
            capacity_ = new_capacity;   
        }
        ++size_;
        return &items_[index];
    }

    Iterator Insert(Iterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end());
        size_t index = pos - begin();         
        if (capacity_ == 0){
            ArrayPtr<Type> temp(++capacity_);
            temp[0] = std::move(value);
            items_.swap(temp);

        } else if (size_ < capacity_) {
            std::move_backward(Iterator(pos), end(), end()); 
            *Iterator(pos) = std::move(value);

        } else {
            size_t new_capacity = capacity_ * 2;
            ArrayPtr<Type> tmp(new_capacity);
            auto it = std::move(begin(), Iterator(pos), tmp.Get());
            *it = std::move(value);
            if (pos != end()) {
            std::move(Iterator(pos), end(), std::next(it));}
            items_.swap(tmp);
            capacity_ = new_capacity;
        }
        ++size_;
        return &items_[index];
    }

    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);

    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<Type> temp(new_capacity);
            if (size_ != 0) {
            std::move(begin(), end(), temp.Get());}
            items_.swap(temp);
            capacity_ = new_capacity;
        } 
    }
    
private:
    // Вместо сырого указателя лучше использовать умный указатель, такой как ArrayPtr
    ArrayPtr<Type> items_;

    size_t size_ = 0;
    size_t capacity_ = 0;
};

template<typename Type>
bool operator==(SimpleVector<Type> left, SimpleVector<Type> right) {
    if (left.GetSize() != right.GetSize()) {
        return false;
    }

    if (left.begin() == right.begin()) {
        return true;
    }

    return std::equal(left.begin(), left.end(), right.begin(), right.end());
}

template<typename Type>
bool operator!=(SimpleVector<Type> left, SimpleVector<Type> right) {
    return !(left == right);
}

template<typename Type>
bool operator<(SimpleVector<Type> left, SimpleVector<Type> right) {
    return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
}

template<typename Type>
bool operator>(SimpleVector<Type> left, SimpleVector<Type> right) {
    return right < left;
}

template<typename Type>
bool operator>=(SimpleVector<Type> left, SimpleVector<Type> right) {
    return !(left < right);
}

template<typename Type>
bool operator<=(SimpleVector<Type> left, SimpleVector<Type> right) {
    return !(right < left);
}
