package com.github.jakz.openmom.ui;

import java.util.Arrays;
import java.util.List;
import java.util.function.BiFunction;

import javax.swing.ImageIcon;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;

import com.github.jakz.openmom.data.Skill;
import com.github.jakz.openmom.data.SpriteInfo;
import com.github.jakz.openmom.lbx.SpriteCache;
import com.pixbits.lib.ui.table.ColumnSpec;
import com.pixbits.lib.ui.table.DataSource;
import com.pixbits.lib.ui.table.TableModel;
import com.pixbits.lib.ui.table.renderers.IconifiedTableCellRenderer;

public class LBXTable extends JTable
{
  TableModel<SpriteInfo> model;
  DataSource<SpriteInfo> data;

  public LBXTable(DataSource<SpriteInfo> data)
  {
    this.data = data;
    this.model = new TableModel<>(this, data);
    
    model.setDefaultRenderer(SpriteInfo.class, new SpriteInfoRenderer(2));

    ColumnSpec<SpriteInfo, ?> index = new ColumnSpec<>("", Integer.class, (i,t) -> i);
    ColumnSpec<SpriteInfo, ?> column = new ColumnSpec<>("", ImageIcon.class, i -> i.getIcon(2));

    index.setWidth(30);
    
    model.addColumn(index);
    model.addColumn(column);
    model.setRowHeight(40);
  }
}
