#include "Modifiers.h"

/*template<typename ReturnType, typename T, typename F>
ReturnType Modifier<ReturnType, T, F>::transformValue(ReturnType previous, const T* owner) const
{
  switch (mode)
  {
    case Mode::ADDITIVE:
      return previous + (type == Type::FLOATING ? static_cast<ReturnType>(multiplier) : static_cast<ReturnType>(value));
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

template class ScalableValue<value_t>;
template class ScalableValue<float>;

#include "UnitEffects.h"

template struct Modifier<value_t, Unit, UnitModifierLevelGetter>;
template struct Modifier<value_t, City, ModifierDummyGetter<City>>;
template struct Modifier<Value, Wizard, ModifierDummyGetter<Wizard>>;


template struct Modifier<ScalableValue<value_t>, Unit, ModifierDummyGetter<Unit>>;


//TODO: used for tests
template struct Modifier<ScalableValue<value_t>, Unit, ModifierDummyGetter<Unit>>;*/
