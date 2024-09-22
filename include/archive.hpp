#include <cstddef>
#include <vector>

#include "serialize.hpp"

namespace bitpacker {
  class BinaryArchive {
    // Member types
  private:
    using archive_type = BinaryArchive;
    using size_type = std::vector<std::byte>::size_type;
    // Member variables
  private:
    std::vector<std::byte> data;

  public:
    template <typename T> archive_type& operator<<(T t) noexcept {
      auto bytes = serialize(t);
      data.insert(data.end(), bytes.begin(), bytes.end());
      return *this;
    }

    size_type size() const noexcept { return data.size(); }

    bool is_empty() const noexcept { return data.empty(); }
    void clear() noexcept { data.clear(); }
  };
} // namespace bitpacker