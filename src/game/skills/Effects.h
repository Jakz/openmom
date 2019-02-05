//
//  Effects.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"

#include <vector>
#include <stack>

enum class Property : u8;
class Unit;

using EffectGroupParam = value_t;
struct EffectGroup
{
public:
  enum class Mode : u16 { KEEP_ALL, UNIQUE, KEEP_GREATER, KEEP_LESSER, PRIORITY };
 
private:
  Mode _mode;
  
public:
  EffectGroup(Mode mode) : _mode(mode) { }
  Mode mode() const { return _mode; }
};

enum class Order { GREATER, LESSER, EQUAL, UNCOMPARABLE, DIFFERENT };

class Effect
{
protected:
  const EffectGroup* _group;
  EffectGroupParam _groupParam;

public:
  Effect() : _group(nullptr), _groupParam(0) { }

  virtual bool isModifier() const { return false; }
  virtual bool isCompound() const { return false; }
  virtual size_t size() const { return 1; }

  void setGroup(const EffectGroup* group, EffectGroupParam param = 0) { this->_group = group; this->_groupParam = param; }
  const EffectGroup* group() const { return _group; }
  EffectGroupParam groupParam() const { return _groupParam; }

  template<typename T> const T* as() const { 
    assert(dynamic_cast<const T*>(this));
    return static_cast<const T*>(this); 
  }
};

template<typename BaseType, typename OwnerType>
class BaseEffect : public Effect
{
public:
  static constexpr BaseType COMPOUND = static_cast<BaseType>(std::numeric_limits<std::underlying_type<BaseType>::type>::max()); //TODO: used as a marker, maybe find a better design?
  const BaseType type;

  BaseEffect(BaseType type) : type(type) { }
  virtual Order compare(const OwnerType* owner, const BaseEffect<BaseType, OwnerType>* other) const { return Order::UNCOMPARABLE; }

  using base_type = BaseType;
  using owner_type = OwnerType;
};

template<typename EffectBase, typename EffectBase::base_type Type, typename EnumType>
class EnumEffect : public EffectBase
{
protected:
  const EnumType _subType;
public:
  EnumEffect(EnumType type) : EffectBase(Type), _subType(type) { }
  
  bool operator==(EnumType type) const { return _subType == type; }
  EnumType subType() const { return _subType; }
  
  Order compare(const typename EffectBase::owner_type* owner, const EffectBase* other) const override
  {
    if (other->type == this->type)
      return other->as<EnumEffect<EffectBase, Type, EnumType>>()->subType() == subType() ? Order::EQUAL : Order::DIFFERENT;
    else
      return Order::UNCOMPARABLE;
  }
};

template<typename EffectBase, typename EffectBase::base_type Type, typename EnumType, typename DataType>
class EnumEffectWithData : public EnumEffect<EffectBase, Type, EnumType>
{
protected:
  const DataType _data;

public:
  EnumEffectWithData(EnumType type, DataType data) : EnumEffect(type), _data(data) { }
  const DataType& data() { return _data; }
};


template<typename EffectBase> struct effect_list_deep_iterator;

template<typename EffectBase>
struct effect_list
{
private:
  using inner_type_t = std::vector<const EffectBase*>;
  inner_type_t data;
  
public:
  using init_list = std::initializer_list<const EffectBase*>;
  using iterator = typename inner_type_t::const_iterator;
  using value_type = typename inner_type_t::value_type;
  
  effect_list() { }
  effect_list(const effect_list&& other) : data(std::move(other.data)) { }
  effect_list(const effect_list& other) : data(other.data) { }
  effect_list(const EffectBase* effect) : data({ effect }) { }
  effect_list(const inner_type_t& effects) : data(effects) { }
  effect_list(const init_list& list) : data(list) { }

  effect_list& operator=(const effect_list& other) { data = other.data; return *this; }
  effect_list& operator=(effect_list&& other) { data = std::move(other.data); return *this; }

  template<typename T> effect_list(T begin, T end)
  {
    for (; begin != end; ++begin) operator+=((*begin)->effects());
  }
  
  void push_back(const EffectBase* effect) { data.push_back(effect); }
  void resize(size_t size) { data.resize(size); }
  
  iterator begin() const { return data.begin(); }
  iterator end() const { return data.end(); }
  
  effect_list_deep_iterator<EffectBase> dbegin() const;
  effect_list_deep_iterator<EffectBase> dend() const;
  
  size_t size() const { return data.size(); }
  size_t flatSize() const { return std::accumulate(data.begin(), data.end(), 0UL, [](size_t v, const Effect* effect) { return v + effect->size(); }); }

  template<typename T, typename EnumType> void filter(EnumType property) {
    filter([property](const EffectBase* effect) {
      return effect->type == T::effect_type && effect->as<T>()->isAffecting(property);
    });
  }

  void filter(std::function<bool(const EffectBase*)> predicate)
  {
    auto nend = std::remove_if(data.begin(), data.end(), [&predicate](const EffectBase* effect) { return !predicate(effect); });
    data.erase(nend, data.end());
  }
  
  const EffectBase* operator[](size_t index) const { return data[index]; }
  
  effect_list operator+(const effect_list& other) const {
    effect_list result = effect_list(*this);
    result += other;
    return data;
  }
  
  effect_list& operator+=(const effect_list& other) {
    data.reserve(data.size()+other.data.size());
    data.insert(data.end(), other.data.begin(), other.data.end());
    return *this;
  }

  template<typename ModifierEffect_> 
  value_t reduceAsModifier(typename ModifierEffect_::property_type property, const typename ModifierEffect_::owner_type* owner, value_t base = 0) const
  {
    //TODO: begin or deep begin?
    return std::accumulate(begin(), end(), base, [property, owner](value_t v, const EffectBase* effect) {
      return effect->as<ModifierEffect_>()->transformValue(property, v, owner);
    });
  }

  void sort() { std::sort(data.begin(), data.end()); }

  /* flatten nested effects */
  effect_list<EffectBase> flatten();
  
  /* this method builds the actual effects taking into consideration
   that some effects override or replace others */
  effect_list<EffectBase> actuals(const typename EffectBase::owner_type* owner) const;
};

template<typename EffectBase>
class CompoundEffect : public EffectBase
{
private:
  effect_list<EffectBase> effects;

public:
  CompoundEffect() : EffectBase(EffectBase::COMPOUND) { }
  CompoundEffect(const effect_list<EffectBase>& effects) : EffectBase(EffectBase::COMPOUND), effects(effects) { }

  bool isCompound() const override { return true; }
  size_t size() const override { return effects.size(); }

  typename effect_list<EffectBase>::iterator begin() const { return effects.begin(); }
  typename effect_list<EffectBase>::iterator end() const { return effects.end(); }

  friend struct effect_list_deep_iterator<EffectBase>;
};

template<typename EffectBase>
struct effect_list_deep_iterator
{
private:
  struct stack_v {
    const effect_list<EffectBase>* list;
    typename effect_list<EffectBase>::iterator it;
    bool end() const { return it == list->end(); }
    const EffectBase* operator*() const { return *it; }
    stack_v(const effect_list<EffectBase>* list, typename effect_list<EffectBase>::iterator it) : list(list), it(it) { }
    bool operator==(const stack_v& other) const { return list == other.list && it == other.it; }
  };
  
  std::stack<stack_v> stack;
  
  void adjust()
  {
    if (stack.top().end())
      return;
    
    const EffectBase* c = operator*();
    
    while (c && c->isCompound())
    {
      const auto* ce = c->as<CompoundEffect<EffectBase>>();
      stack.emplace(&ce->effects, ce->effects.begin());
      adjust();
      
      if (!stack.top().end())
        c = operator*();
      else
      {
        stack.pop();
        break;
      }
    }
  }
  
public:
  using difference_type = ptrdiff_t;
  using value_type = EffectBase* ;
  using reference = const EffectBase&;
  using pointer = const EffectBase*;
  using iterator_category = std::forward_iterator_tag;
  
public:
  effect_list_deep_iterator(const effect_list<EffectBase>* list, typename effect_list<EffectBase>::iterator it) { stack.emplace(list, it); adjust(); }
  
  effect_list_deep_iterator& operator++()
  {
    auto& it = stack.top().it;
    
    /* iterator is not at end */
    if (!stack.top().end())
    {
      ++it;
      adjust();
    }
    
    {
      /* iterator is end of current child: pop and advance parent */
      while (stack.top().end() && stack.size() > 1)
      {
        /* pop scope: we're at end */
        stack.pop();
        
        /* now we're on parent, we should advance it */
        ++stack.top().it;
      }
    }
    
    return *this;
  }
  
  bool operator==(const effect_list_deep_iterator& other) const { return stack == other.stack; }
  bool operator!=(const effect_list_deep_iterator& other) const { return stack != other.stack; }
  
  const EffectBase* operator*() const { return *stack.top(); }
};

template<typename EffectBase> inline effect_list_deep_iterator<EffectBase> effect_list<EffectBase>::dbegin() const { return effect_list_deep_iterator<EffectBase>(this, begin()); }
template<typename EffectBase> inline effect_list_deep_iterator<EffectBase> effect_list<EffectBase>::dend() const { return effect_list_deep_iterator<EffectBase>(this, end()); }





enum class UnitEffectType
{
  MOVEMENT,
  DISALLOW_MOVEMENT,

  ABILITY,
  PARAMETRIC_ABILITY,
  IMMUNITY,
  MAGIC_WEAPONS,

  COMBAT_BONUS,
  UNIT_BONUS,
  ARMY_BONUS,

  GRANT_SPELL,

  SPECIAL_ATTACK,

  WIZARD_BONUS,
};

using UnitEffect = BaseEffect<UnitEffectType, Unit>;
using unit_effect_list = effect_list<UnitEffect>;



enum class CityEffectType
{
  CITY_BONUS,
  SIMPLE_EFFECT
};

enum class SimpleCityEffect
{
  ALLOWS_NIGHTSHADE
};

using CityEffect = BaseEffect<CityEffectType, City>;
using city_effect_list = effect_list<CityEffect>;