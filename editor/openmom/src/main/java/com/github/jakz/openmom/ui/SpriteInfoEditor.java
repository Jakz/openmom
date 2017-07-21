package com.github.jakz.openmom.ui;

import java.awt.Component;

import javax.swing.DefaultCellEditor;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.table.DefaultTableCellRenderer;

import com.github.jakz.openmom.data.SpriteInfo;
import com.github.jakz.openmom.data.SpriteInfoLBX;

public class SpriteInfoEditor extends DefaultCellEditor
{
  final private int ratio;
  
  public SpriteInfoEditor(int ratio)
  {
    super(new JTextField());
    this.ratio = ratio;
  }
  
  @Override
  public Component getTableCellEditorComponent(JTable table, Object value, boolean isSelected, int row, int column)
  {
    JTextField label = (JTextField)super.getTableCellEditorComponent(table, value, isSelected, row, column);
    SpriteInfo info = (SpriteInfo)value;
        
    if (info instanceof SpriteInfoLBX)
    {
      SpriteInfoLBX lbx = (SpriteInfoLBX)info;
      label.setText(lbx.lbx+", "+lbx.index);
    }
    else
      label.setText("");
    
    return label;
  }
  
  @Override
  public Object getCellEditorValue()
  {
    String s = (String)super.getCellEditorValue();

    String[] ss = s.split(",");
    int index = 0;
    
    if (ss.length != 2) 
      return null;;
    
    try 
    {
       index = Integer.valueOf(ss[1].trim());
    }
    catch (NumberFormatException e)
    {
      return null;
    }

    return new SpriteInfoLBX(ss[0].trim(), index);
  }
}
