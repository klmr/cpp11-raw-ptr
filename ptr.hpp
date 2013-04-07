#ifndef BASE_PTR_HPP
#define BASE_PTR_HPP

#include <memory>

namespace base {

template <typename T>
class ptr {
public:

    using pointer = T*;
    using reference = T&;

    constexpr ptr() noexcept = default;

    constexpr ptr(std::nullptr_t) noexcept : value() { }

    template <typename U>
    friend class ptr;

    template <typename Other>
    ptr(ptr<Other> const& other) noexcept : value(other.value) { }

    pointer get() const noexcept { return value; }

    reference operator *() const noexcept { return *get(); }

    pointer operator ->() const noexcept { return get(); }

    template <typename U>
    friend ptr<U> raw_ptr(U*) noexcept;

private:
    pointer value;

    // We want to force users to use the builder function `raw_ptr`, rather than
    // using a converting constructor to create a `ptr` instance from a raw T*.
    // This makes it explicit that we are intentionally handling a raw pointer.
    explicit ptr(pointer value) noexcept : value(value) { }
};

template <typename T>
inline ptr<T> raw_ptr(T* value) noexcept {
    return static_cast<ptr<T>>(value);
}

template <typename T, typename U>
inline bool operator ==(ptr<T> const& lhs, ptr<U> const& rhs) noexcept {
    return lhs.get() == rhs.get();
}

template <typename T>
inline bool operator ==(ptr<T> const& lhs, std::nullptr_t) noexcept {
    return lhs.get() == nullptr;
}

template <typename T>
inline bool operator ==(std::nullptr_t, ptr<T> const& rhs) noexcept {
    return rhs.get() == nullptr;
}

template <typename T, typename U>
inline bool operator !=(ptr<T> lhs, ptr<U> rhs) noexcept {
    return not (lhs == rhs);
}

template <typename T>
inline bool operator !=(ptr<T> const& lhs, std::nullptr_t) noexcept {
    return lhs.get() != nullptr;
}

template <typename T>
inline bool operator !=(std::nullptr_t, ptr<T> const& rhs) noexcept {
    return rhs.get() != nullptr;
}

template <typename T, typename U>
inline ptr<T> static_pointer_cast(ptr<U> const& p) noexcept {
    return raw_ptr(static_cast<T*>(p.get()));
}

template <typename T, typename U>
inline ptr<T> dynamic_pointer_cast(ptr<U> const& p) noexcept {
    return raw_ptr(dynamic_cast<T*>(p.get()));
}

template <typename T, typename U>
inline ptr<T> const_pointer_cast(ptr<U> const& p) noexcept {
    return raw_ptr(const_cast<T*>(p.get()));
}

template <typename T, typename U>
inline bool operator <(ptr<T> const& lhs, ptr<U> const& rhs) noexcept {
    return lhs.get() < rhs.get();
}

template <typename T>
inline bool operator <(ptr<T> const&, std::nullptr_t) noexcept {
    return false;
}

template <typename T>
inline bool operator <(std::nullptr_t, ptr<T> const& rhs) noexcept {
    return rhs.get() != nullptr;
}

template <typename T, typename U>
inline bool operator <=(ptr<T> const& lhs, ptr<U> const& rhs) noexcept {
    return lhs.get() <= rhs.get();
}

template <typename T>
inline bool operator <=(ptr<T> const& lhs, std::nullptr_t) noexcept {
    return lhs.get() == nullptr;
}

template <typename T>
inline bool operator <=(std::nullptr_t, ptr<T> const&) noexcept {
    return true;
}

template <typename T, typename U>
inline bool operator >(ptr<T> const& lhs, ptr<U> const& rhs) noexcept {
    return lhs.get() > rhs.get();
}

template <typename T>
inline bool operator >(ptr<T> const& lhs, std::nullptr_t) noexcept {
    return lhs != nullptr;
}

template <typename T>
inline bool operator >(std::nullptr_t, ptr<T> const&) noexcept {
    return false;
}

template <typename T, typename U>
inline bool operator >=(ptr<T> const& lhs, ptr<U> const& rhs) noexcept {
    return lhs.get() >= rhs.get();
}

template <typename T>
inline bool operator >=(ptr<T> const&, std::nullptr_t) noexcept {
    return true;
}

template <typename T>
inline bool operator >=(std::nullptr_t, ptr<T> const& rhs) noexcept {
    return nullptr == rhs.get();
}

} // namespace base

namespace std {
    template <typename T>
    struct pointer_traits<base::ptr<T>> {
        using pointer = T*;
        using element_type = T;
        using difference_type = ptrdiff_t;

        template <typename U>
        using rebind = base::ptr<U>;
    };

    template <typename T>
    struct hash<base::ptr<T>> {
        using result_type = size_t;
        using argument_type = base::ptr<T>;

        result_type operator ()(argument_type const& p) const noexcept {
            return std::hash<typename base::ptr<T>::pointer>()(p.get());
        }
    };
} // namespace std

#endif // ndef BASE_PTR_HPP
