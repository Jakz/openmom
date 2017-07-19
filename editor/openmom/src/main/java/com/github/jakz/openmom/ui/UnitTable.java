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

import com.github.jakz.openmom.data.Unit;
import com.github.jakz.openmom.data.UnitType;
import com.github.jakz.openmom.lbx.SpriteCache;
import com.pixbits.lib.ui.table.ColumnSpec;
import com.pixbits.lib.ui.table.DataSource;
import com.pixbits.lib.ui.table.TableModel;
import com.pixbits.lib.ui.table.renderers.IconifiedTableCellRenderer;

public class UnitTable extends JTable
{
  TableModel<Unit> model;
  DataSource<Unit> data;
  
  BiFunction<String, Integer, TableCellRenderer> iconRendererBuilder = (l,i) -> 
    new IconifiedTableCellRenderer<Integer>(
        new DefaultTableCellRenderer(), 
        v -> new ImageIcon(SpriteCache.instance.get(l, i).get(0, 2))
    );
  
  public UnitTable(DataSource<Unit> data)
  {
    this.data = data;
    this.model = new TableModel<>(this, data);
    
    try
    {
      List<ColumnSpec<Unit, ?>> columns = Arrays.asList(
        new ColumnSpec<>("", ImageIcon.class, u -> u.visuals.icon != null ? u.visuals.icon.getIcon(1) : null),
          
        new ColumnSpec<>(Unit.class.getField("identifier")),
        new ColumnSpec<>(Unit.class.getField("type")),
        new ColumnSpec<>(Unit.class.getField("upkeep")),
        new ColumnSpec<>(Unit.class.getField("cost")),
        new ColumnSpec<>(Unit.class.getField("melee")),
        // ranged
        new ColumnSpec<>(Unit.class.getField("defense")),
        new ColumnSpec<>(Unit.class.getField("resistance")),
        new ColumnSpec<>(Unit.class.getField("hits")),
        new ColumnSpec<>(Unit.class.getField("figures")),
        new ColumnSpec<>(Unit.class.getField("movement")),
        new ColumnSpec<>(Unit.class.getField("sight"))
        // skills
        // visuals
      );

      columns.get(2).setDefaultEnumEditor();

      columns.get(5).setRenderer(iconRendererBuilder.apply("compix.lbx", 61));
      // ranged
      columns.get(6).setRenderer(iconRendererBuilder.apply("compix.lbx", 70));
      columns.get(7).setRenderer(iconRendererBuilder.apply("compix.lbx", 75));
      columns.get(8).setRenderer(iconRendererBuilder.apply("compix.lbx", 71));
      // figures
      columns.get(10).setRenderer(iconRendererBuilder.apply("compix.lbx", 72));

      
      columns.forEach(c -> {
        c.setEditable(true); 
        model.addColumn(c);
      });   
      
    }
    catch (NoSuchFieldException e)
    {
      e.printStackTrace();
    }
  }
}
