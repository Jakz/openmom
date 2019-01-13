package com.github.jakz.openmom;

import java.util.List;

import com.github.jakz.openmom.data.Race;
import com.github.jakz.openmom.data.Skill;
import com.github.jakz.openmom.data.Unit;
import com.pixbits.lib.ui.table.ModifiableDataSource;

public class Data
{
  public ModifiableDataSource<Race> races;
  public ModifiableDataSource<Unit> units;
  public ModifiableDataSource<Skill> skills;
}
