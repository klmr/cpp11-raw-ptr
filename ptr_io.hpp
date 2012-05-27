#ifndef BASE_PTR_IO_HPP
#define BASE_PTR_IO_HPP

#include <iostream>
#include "ptr.hpp"

namespace base {
template <typename T>
inline std::ostream& operator <<(std::ostream& out, ptr<T> p) {
    return out << p.get();
}
} // namespace base

#endif // ndef BASE_PTR_IO_HPP
