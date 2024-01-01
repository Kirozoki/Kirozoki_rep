#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include "array_ptr.h"

using namespace std::literals;

class ReserveProxyObj {
public:
    explicit ReserveProxyObj(size_t capacity_to_reserve) :proxy_capacity_(capacity_to_reserve) {}

    size_t GetCapacity()
    {
        return proxy_capacity_;
    }

private:
    size_t proxy_capacity_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size) : size_(size), capacity_(size) {
        if (size == 0) {
        SimpleVector();}
        else {
        item_ = new Type[size];
        std::fill(begin(), end(), Type {});
        }
    }

    SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size) {
        if (size == 0) {
        SimpleVector();}
        else {
        item_ = new Type[size];
        std::fill(begin(), end(), value);}
    }

    SimpleVector(std::initializer_list<Type> init) {
        if (init.size() == 0) {
        SimpleVector();}
        else {
        item_ = new Type[init.size()];
        std::copy(init.begin(), init.end(), begin()); 
        size_ = init.size();
        capacity_ = init.size();}
    }

    SimpleVector(ReserveProxyObj capacity_to_reserve) {
    Reserve(capacity_to_reserve.GetCapacity());
}

    ~SimpleVector() {delete[] item_;}

    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    Type& operator[](size_t index) noexcept {
        return *(item_ + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return *(item_ + index);
    }

    Type& At(size_t index) {
        if (index >= size_) 
        throw std::out_of_range("index is greater than size"s);
        return *(item_ + index);
    }

    const Type& At(size_t index) const {
        if (index >= size_) 
        throw std::out_of_range("index is greater than size"s);
        return *(item_ + index);
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) {
            if (new_size > capacity_) {
            int new_capacity=std::max(new_size, 2*capacity_);
            ArrayPtr<Type> newVector(new_capacity);
            auto it_end=std::copy(begin(), end(), newVector.Get());
            std::fill(it_end, it_end+new_size-size_,Type{});
            item_=newVector.Release(); 
            size_ = new_size;  
            capacity_=new_capacity;
        }
        else if(new_size > size_) {
            std::fill(begin() + size_, begin() + new_size, Type {});
            size_ = new_size;
        }
        else if (new_size < size_) {
            size_ = new_size;
        }
    }

    Iterator begin() noexcept {
        return item_;
    }

    Iterator end() noexcept {
        return item_ + size_; 
    }

    ConstIterator begin() const noexcept {
        return item_;
    }

    ConstIterator end() const noexcept {
        return item_ + size_;
    }

    ConstIterator cbegin() const noexcept {
        return item_;
    }

    ConstIterator cend() const noexcept {
        return item_ + size_;
    }

    SimpleVector(const SimpleVector& other) 
    { 
    SimpleVector tmp;
    tmp.capacity_ = other.GetCapacity();
    tmp.size_ = 0;
    tmp.item_ = new Type[other.size_];
    for (size_t i=0; i < other.size_; ++i)
        {
            tmp[i] = other[i];
            ++tmp.size_;
        }
    swap(tmp);
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            SimpleVector tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        // Напишите тело самостоятельно
        Insert(end(), item);
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(Iterator pos, const Type& value) {
    if (capacity_ == size_)
    {
        capacity_ == 0 ? capacity_ = 1 : capacity_ = capacity_ * 2;
        Type* temp_data = new Type[capacity_];
        Iterator new_elem = std::copy(begin(), Iterator(pos), temp_data);
        *new_elem = value;
        std::copy(Iterator(pos), end(), std::next(new_elem));
        delete[] item_;
        item_ = temp_data;
        ++size_;
        return new_elem;
    }
    else
    {
        std::copy_backward(Iterator(pos), end(), end());
        *(Iterator(pos)) = value;
        ++size_;
        return Iterator(pos);
    }
}


    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        // Напишите тело самостоятельно
        assert(size_!=0);
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        // Напишите тело самостоятельно
        std::copy_backward(std::next(Iterator(pos)), end(), Iterator(pos));
        --size_;
        return Iterator(pos);
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        // Напишите тело самостоятельно
       std::swap(item_, other.item_);
       std::swap(size_, other.size_);
       std::swap(capacity_, other.capacity_); 
    }

    void Reserve(size_t new_capacity){
       if (new_capacity > capacity_) {
        if (size_!=0) {
         Iterator tmp = new Type(new_capacity);
         std::copy(begin(), end(), tmp);
         delete[] item_;
         item_ = tmp;}
         capacity_ = new_capacity;
       }
    }


    private:

    Iterator item_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0; 
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    if (lhs.GetSize()!=rhs.GetSize()) {
        return false; 
    }
    if (lhs.cbegin() == rhs.cbegin())
    return true;

    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs < rhs);
}