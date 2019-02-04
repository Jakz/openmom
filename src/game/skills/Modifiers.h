//
//  Modifiers.h
//  OpenMoM
//
//  Created by Jack on 3/02/19.
//  Copyright (c) 2010 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"
#include "Effects.h"

#include <vector>
#include <stack>

/*
template<typename T>
class Value
{
private:
  T _base;
  float _multiplier;

public:
  Value(T base, float multiplier) : _base(base), _multiplier(multiplier) { assert(_multiplier >= 0.0f); }
  Value(T base) : Value(base, 1.0f) { }
  Value() : Value(0) { }

  inline Value operator+(T value) const { return Value(_base + value, _multiplier); }
  inline Value operator-(T value) const { return Value(_base - value, _multiplier); }
  inline Value operator*(float multiplier) const { return Value(_base, _multiplier + multiplier); }
  inline Value operator/(float multiplier) const { return Value(_base, _multiplier - multiplier); }

  inline operator T() const { return _base * _multiplier; }
};
*/

template<typename ReturnType, typename T, typename F>
struct Modifier
{
  using owner_type = T;
  
  enum class Priority { BASE, FIRST, ANY, LAST } priority;
  enum class Type { INTEGER, FLOATING } type;
  enum class Mode { ADDITIVE, ADDITIVE_PARAMETRIC, MULTIPLICATIVE, FIXED  } mode;
  union {
    float multiplier;
    value_t value;
  };

  Modifier(Mode mode, float value, Priority priority = Priority::ANY) : priority(priority), type(Type::FLOATING), mode(mode), multiplier(value)
  { 
    assert(isFloating());
  }

  Modifier(Mode mode, value_t value, Priority priority = Priority::ANY) : priority(priority), type(Type::INTEGER), mode(mode), value(value)
  { 
    assert(!isFloating());
  }

  Modifier(value_t value) : priority(Priority::ANY), mode(Mode::ADDITIVE), type(Type::INTEGER), value(value) { }
  Modifier(float multiplier) : priority(Priority::ANY), mode(Mode::ADDITIVE_PARAMETRIC), type(Type::FLOATING), multiplier(multiplier) { }

  ReturnType transformValue(ReturnType value, const T* owner) const;
  
  bool isFloating() const { return type == Type::FLOATING; }
  value_t truncatedValue() const { return isFloating() ? multiplier : value; }
 
  Order compareMagnitude(const T* owner, const Modifier<ReturnType, T, F>& other) const
  {
    ReturnType v1 = transformValue(0, owner), v2 = transformValue(0, owner);
    
    assert(mode == Mode::ADDITIVE || mode == Mode::ADDITIVE_PARAMETRIC);
    assert(other.mode == Mode::ADDITIVE || other.mode == Mode::ADDITIVE_PARAMETRIC);

    if (v1 > v2) return Order::GREATER;
    else if (v1 < v2) return Order::LESSER;
    else return Order::EQUAL;
  }

  Order compareForSorting(const Modifier<ReturnType, T, F>& other) const
  {
    if (priority < other.priority) return Order::LESSER;
    else if (priority > other.priority) return Order::GREATER;
    else
    {
      if (type < other.type) return Order::LESSER;
      else if (type > other.type) return Order::GREATER;
      else return Order::EQUAL;
    }

  }

  bool operator<(const Modifier<ReturnType, T, F>& other) { return compareForSorting(other) == Order::LESSER; }
};

template<typename EffectBase, typename ModifierBase>
class ModifierEffect : public EffectBase
{
protected:
  using owner_type = typename EffectBase::owner_type;

  ModifierBase _value;
  predicate<const owner_type*> _predicate;
  value_t transformValue(value_t value, const owner_type* owner) const { return _predicate(owner) ? _value.transformValue(value, owner) : _value; }

public:
  ModifierEffect(typename EffectBase::base_type type, ModifierBase value) : EffectBase(type), _value(value), _predicate([](auto owner) { return true; }) { }
  ModifierEffect(typename EffectBase::base_type type, ModifierBase value, predicate<const owner_type*> predicate) : EffectBase(type), _value(value), _predicate(predicate) { }

  const ModifierBase& modifier() const { return _value; }
  bool isModifier() const override { return true; }

  Order compare(const Unit* unit, const EffectBase* other) const override
  {
    //TODO: this doesn't check if kind of modifier is the same so it should be used only when this is sure (eg in an yaml defined SkillGroup) 
    if (other->isModifier())
    {
      return modifier().compareMagnitude(unit, other->as<ModifierEffect<EffectBase, ModifierBase>>()->modifier());
    }
    else
      return Order::UNCOMPARABLE;
  }

  using modifier_type = ModifierBase;
};
