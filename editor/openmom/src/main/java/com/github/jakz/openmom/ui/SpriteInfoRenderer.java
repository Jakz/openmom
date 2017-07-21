package com.github.jakz.openmom.ui;

import java.awt.Component;

import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;

import com.github.jakz.openmom.data.SpriteInfo;

public class SpriteInfoRenderer extends DefaultTableCellRenderer
{
  final private int ratio;
  
  public SpriteInfoRenderer(int ratio)
  {
    this.ratio = ratio;
  }
  
  @Override
  public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column)
  {
    SpriteInfo info = (SpriteInfo)value;
    JLabel label = (JLabel)super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
    
    if (info != null)
    {
      label.setIcon(info.getIcon(ratio));
      label.setText(info.toString());
    }
    else
    {
      label.setIcon(null);
      label.setText("");
    }
    
    return label;
  }
}
