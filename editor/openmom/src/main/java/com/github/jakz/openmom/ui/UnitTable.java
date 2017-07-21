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

import com.github.jakz.openmom.data.Ranged;
import com.github.jakz.openmom.data.SpriteInfo;
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
    
    model.setDefaultRenderer(SpriteInfo.class, new SpriteInfoRenderer(2));
        
    try
    {
      List<ColumnSpec<Unit, ?>> columns = Arrays.asList(
        new ColumnSpec<>("", ImageIcon.class, u -> u.visuals.icon != null ? u.visuals.icon.getIcon(2) : null),
          
        new ColumnSpec<>(Unit.class.getField("identifier")),
        new ColumnSpec<>(Unit.class.getField("type")),
        new ColumnSpec<>(Unit.class.getField("upkeep")),
        new ColumnSpec<>(Unit.class.getField("cost")),
        new ColumnSpec<>(Unit.class.getField("melee")),
        new ColumnSpec<>(Unit.class.getField("ranged")),
        new ColumnSpec<>(Unit.class.getField("defense")),
        new ColumnSpec<>(Unit.class.getField("resistance")),
        new ColumnSpec<>(Unit.class.getField("hits")),
        new ColumnSpec<>(Unit.class.getField("figures")),
        new ColumnSpec<>(Unit.class.getField("movement")),
        new ColumnSpec<>(Unit.class.getField("sight")),
        // skills
        new ColumnSpec<>("icon", SpriteInfo.class, u -> u.visuals.icon, (u, s) -> { if (s != null) u.visuals.icon = s; }),
        new ColumnSpec<>("figure", SpriteInfo.class, u -> u.visuals.figure, (u, s) -> { if (s != null) u.visuals.figure = s; })

        // visuals
      );
      
      int widths[] = { 30, 200, 60, 50, 50, 50, 100, 50, 50, 50, 50, 50, 50 };
      
      for (int i = 0; i < widths.length; ++i)
        columns.get(i).setWidth(widths[i]);

      columns.get(2).setDefaultEnumEditor();

      columns.get(5).setRenderer(iconRendererBuilder.apply("compix.lbx", 61));
      
      columns.get(6).setEditor(new RangedEditor());
      columns.get(6).setSetter((u,r) -> { if (r != null) u.ranged = (Ranged)r; });
      
      columns.get(7).setRenderer(iconRendererBuilder.apply("compix.lbx", 70));
      columns.get(8).setRenderer(iconRendererBuilder.apply("compix.lbx", 75));
      columns.get(9).setRenderer(iconRendererBuilder.apply("compix.lbx", 71));
      // figures
      columns.get(11).setRenderer(iconRendererBuilder.apply("compix.lbx", 72));
      // sight
      columns.get(13).setEditor(new SpriteInfoEditor(2));
      columns.get(14).setEditor(new SpriteInfoEditor(2));
      
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
