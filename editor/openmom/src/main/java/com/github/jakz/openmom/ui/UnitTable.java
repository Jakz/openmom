package com.github.jakz.openmom.ui;

import java.util.Arrays;
import java.util.List;

import javax.swing.DefaultCellEditor;
import javax.swing.JComboBox;
import javax.swing.JTable;

import com.github.jakz.openmom.data.Unit;
import com.github.jakz.openmom.data.UnitType;
import com.pixbits.lib.ui.table.ColumnSpec;
import com.pixbits.lib.ui.table.DataSource;
import com.pixbits.lib.ui.table.TableModel;

public class UnitTable extends JTable
{
  TableModel<Unit> model;
  DataSource<Unit> data;
  
  public UnitTable(DataSource<Unit> data)
  {
    this.data = data;
    this.model = new TableModel<>(this, data);
    
    try
    {
      List<ColumnSpec<Unit, ?>> columns = Arrays.asList(
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
         
      columns.get(1).setDefaultEnumEditor();
          
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
