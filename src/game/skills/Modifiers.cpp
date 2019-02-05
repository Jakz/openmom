#include "Modifiers.h"

template<typename ReturnType, typename T, typename F>
ReturnType Modifier<ReturnType, T, F>::transformValue(ReturnType previous, const T* owner) const
{
  switch (mode)
  {
    case Mode::ADDITIVE:
      return previous + (type == Type::FLOATING ? static_cast<ReturnType>(multiplier) : value);
    case Mode::ADDITIVE_PARAMETRIC:
      return previous + static_cast<ReturnType>(F()(owner)*multiplier);
    case Mode::MULTIPLICATIVE:
      return static_cast<ReturnType>(previous * multiplier);
    case Mode::FIXED:
      return value;
    default:
      assert(false);
      return 0;
  }
}

template struct Modifier<value_t, Unit, UnitModifierLevelGetter>;
template struct Modifier<value_t, City, ModifierDummyGetter<City>>;