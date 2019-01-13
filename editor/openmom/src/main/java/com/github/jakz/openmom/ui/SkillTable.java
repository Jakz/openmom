package com.github.jakz.openmom.ui;

import java.util.Arrays;
import java.util.List;
import java.util.function.BiFunction;

import javax.swing.ImageIcon;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;

import com.github.jakz.openmom.data.Race;
import com.github.jakz.openmom.data.Skill;
import com.github.jakz.openmom.data.SpriteInfo;
import com.github.jakz.openmom.lbx.SpriteCache;
import com.pixbits.lib.ui.table.ColumnSpec;
import com.pixbits.lib.ui.table.DataSource;
import com.pixbits.lib.ui.table.TableModel;
import com.pixbits.lib.ui.table.renderers.IconifiedTableCellRenderer;

public class SkillTable extends JTable
{
  TableModel<Skill> model;
  DataSource<Skill> data;
  
  BiFunction<String, Integer, TableCellRenderer> iconRendererBuilder = (l,i) -> 
    new IconifiedTableCellRenderer<Integer>(
        new DefaultTableCellRenderer(), 
        v -> new ImageIcon(SpriteCache.instance.get(l, i).get(0, 2))
    );
  
  public SkillTable(DataSource<Skill> data)
  {
    this.data = data;
    this.model = new TableModel<>(this, data);
    
    model.setDefaultRenderer(SpriteInfo.class, new SpriteInfoRenderer(2));
        
    try
    {
      List<ColumnSpec<Skill, ?>> columns = Arrays.asList(
        new ColumnSpec<>("", ImageIcon.class, r -> r.visuals != null && r.visuals.icon != null ? r.visuals.icon.getIcon(2) : new ImageIcon()),
        new ColumnSpec<>(Skill.class.getField("identifier")),
        new ColumnSpec<>("effects", String.class, s -> s.effects != null ? s.effects.toString() : "")
        //new ColumnSpec<>(Skill.class.getField("effects"))
        // visuals
      );
      
      int[] widths = new int[] { 30, 300 };
      for (int i = 0; i < widths.length; ++i)
        columns.get(i).setWidth(widths[i]);
      
      columns.forEach(c -> {
        c.setEditable(true); 
        model.addColumn(c);
      });   
      
      model.setRowHeight(40);
    }
    catch (NoSuchFieldException e)
    {
      e.printStackTrace();
    }
  }
}
