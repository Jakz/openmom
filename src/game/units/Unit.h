#ifndef _UNIT_H_
#define _UNIT_H_

#include "Common.h"

#include "UnitSpec.h"

class Unit
{
private:
  const UnitSpec& spec;
  
public:
  Unit(const UnitSpec& spec) : spec(spec) { }
  
  Productable::Type type() { return spec.productionType(); }
};

#endif
