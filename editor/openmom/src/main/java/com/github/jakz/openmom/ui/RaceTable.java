package com.github.jakz.openmom.ui;

import java.util.Arrays;
import java.util.List;
import java.util.function.BiFunction;

import javax.swing.DefaultCellEditor;
import javax.swing.ImageIcon;
import javax.swing.JComboBox;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;

import com.github.jakz.openmom.data.Race;
import com.github.jakz.openmom.data.Ranged;
import com.github.jakz.openmom.data.SpriteInfo;
import com.github.jakz.openmom.data.Unit;
import com.github.jakz.openmom.data.UnitType;
import com.github.jakz.openmom.lbx.SpriteCache;
import com.pixbits.lib.ui.table.ColumnSpec;
import com.pixbits.lib.ui.table.DataSource;
import com.pixbits.lib.ui.table.TableModel;
import com.pixbits.lib.ui.table.renderers.IconifiedTableCellRenderer;

public class RaceTable extends JTable
{
  TableModel<Race> model;
  DataSource<Race> data;
  
  BiFunction<String, Integer, TableCellRenderer> iconRendererBuilder = (l,i) -> 
    new IconifiedTableCellRenderer<Integer>(
        new DefaultTableCellRenderer(), 
        v -> new ImageIcon(SpriteCache.instance.get(l, i).get(0, 2))
    );
  
  public RaceTable(DataSource<Race> data)
  {
    this.data = data;
    this.model = new TableModel<>(this, data);
    
    model.setDefaultRenderer(SpriteInfo.class, new SpriteInfoRenderer(2));
        
    try
    {
      List<ColumnSpec<Race, ?>> columns = Arrays.asList(
        new ColumnSpec<>("", ImageIcon.class, r -> r.visuals.graphics.worker.getIcon(2)),
        new ColumnSpec<>(Race.class.getField("identifier")),
        new ColumnSpec<>(Race.class.getField("cityGrowthModifier")),
        new ColumnSpec<>(Race.class.getField("outpostGrowthChance"))

        // visuals
      );
      
      int[] widths = new int[] { 30, 100, 100, 120 };
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
