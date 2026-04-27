#include "toygpu/TypeSystem.h"
#include <unordered_map>

namespace toygpu {

std::optional<PTXType> parseTypeToken(const std::string& token) {
  static const std::unordered_map<std::string, PTXType> map = {
      {".b8", PTXType::B8}, {".b16", PTXType::B16}, {".b32", PTXType::B32},
      {".s32", PTXType::S32}, {".u32", PTXType::U32}, {".f32", PTXType::F32},
      {".pred", PTXType::Pred}};
  auto it = map.find(token);
  if (it == map.end()) return std::nullopt;
  return it->second;
}

TypeInfo getTypeInfo(PTXType type) {
  switch (type) {
    case PTXType::B8: return {type, 8, false, false, false};
    case PTXType::B16: return {type, 16, false, false, false};
    case PTXType::B32: return {type, 32, false, false, false};
    case PTXType::S32: return {type, 32, true, false, false};
    case PTXType::U32: return {type, 32, false, false, false};
    case PTXType::F32: return {type, 32, true, true, false};
    case PTXType::Pred: return {type, 1, false, false, true};
    default: return {PTXType::Invalid, 0, false, false, false};
  }
}

std::string toString(PTXType type) {
  switch (type) {
    case PTXType::B8: return "b8";
    case PTXType::B16: return "b16";
    case PTXType::B32: return "b32";
    case PTXType::S32: return "s32";
    case PTXType::U32: return "u32";
    case PTXType::F32: return "f32";
    case PTXType::Pred: return "pred";
    default: return "invalid";
  }
}

bool isCompatible(PTXType dst, PTXType src) {
  if (dst == src) return true;
  auto d = getTypeInfo(dst);
  auto s = getTypeInfo(src);
  if (d.isPredicate || s.isPredicate) return false;
  return d.bits == s.bits;
}

}  // namespace toygpu
