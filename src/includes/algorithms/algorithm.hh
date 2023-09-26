#pragma once

#include <string>
#include <utility>

#include "../context.hh"
#include "../stats.hh"

namespace apa {

class algorithm {
 public:
  explicit algorithm(std::string name, apa::context context)
      : _name{std::move(name)}, _context{std::move(context)} {}

  virtual ~algorithm() = default;

  [[nodiscard]] virtual apa::stats run() = 0;

  [[nodiscard]] const std::string& name() const { return _name; }

 protected:
  [[nodiscard]] apa::context& context() { return _context; }

 private:
  std::string _name;
  apa::context _context;
};

}  // namespace apa
