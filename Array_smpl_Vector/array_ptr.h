#pragma once

template <typename Type>
class ArrayPtr {
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        // Реализуйте конструктор самостоятельно
        if (size == 0) {
            raw_ptr_ = nullptr;
            return;
        }
        raw_ptr_ = new Type[size];
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept {
        // Реализуйте конструктор самостоятельно
        raw_ptr_ = raw_ptr;
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        // Напишите деструктор самостоятельно
        delete[] raw_ptr_;
    }

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        auto tmp = raw_ptr_; 
        raw_ptr_ = nullptr;
        return tmp;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        // Реализуйте операцию самостоятельно
        return *(raw_ptr_ + index);
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        // Реализуйте операцию самостоятельно
        return const_cast<Type&>(*(raw_ptr_ + index));
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        // Заглушка. Реализуйте операцию самостоятельно
        return raw_ptr_ != nullptr;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        // Реализуйте метод самостоятельно
        auto tmp_ptr = other.Get(); 
         other.raw_ptr_ = this->raw_ptr_;
         raw_ptr_ = tmp_ptr;
    }

private:
    Type* raw_ptr_ = nullptr;
};

