package com.github.jakz.openmom.ui;

import java.awt.Component;

import javax.swing.DefaultCellEditor;
import javax.swing.JTable;
import javax.swing.JTextField;

import com.github.jakz.openmom.data.Ranged;

public class RangedEditor extends DefaultCellEditor
{
  public RangedEditor()
  {
    super(new JTextField());
  }

  @Override
  public Object getCellEditorValue()
  {
    String s = (String)super.getCellEditorValue();

    if (s.equals("none"))
      return new Ranged(Ranged.Type.none);
    else 
    {
      String[] ss = s.split(",");
      
      if (ss.length == 3)
      {
        int strength, ammo;
        Ranged.Type type;
        
        try 
        {
          strength = Integer.valueOf(ss[0].trim());
          ammo = Integer.valueOf(ss[2].trim());
          type = Ranged.Type.valueOf(ss[1].trim());
        }
        catch (IllegalArgumentException e)
        {
          return null;
        }
        
        return new Ranged(strength, type, ammo);
      }
      
      return null;
    }
  }
}
