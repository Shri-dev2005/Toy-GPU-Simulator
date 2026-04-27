#pragma once
#include <optional>
#include <string>

namespace toygpu {

enum class PTXType { Invalid, B8, B16, B32, S32, U32, F32, Pred };

struct TypeInfo {
  PTXType type{PTXType::Invalid};
  unsigned bits{0};
  bool isSigned{false};
  bool isFloat{false};
  bool isPredicate{false};
};

std::optional<PTXType> parseTypeToken(const std::string& token);
TypeInfo getTypeInfo(PTXType type);
std::string toString(PTXType type);
bool isCompatible(PTXType dst, PTXType src);

}  // namespace toygpu
