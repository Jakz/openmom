package com.github.jakz.openmom.ui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import com.github.jakz.openmom.data.SpriteInfo;
import com.github.jakz.openmom.data.SpriteInfoLBX;
import com.github.jakz.openmom.lbx.LBX;
import com.github.jakz.openmom.lbx.LBXID;
import com.pixbits.lib.ui.table.DataSource;

public class LBXMultiTable extends JPanel
{
  private JTabbedPane tabs = new JTabbedPane(JTabbedPane.RIGHT);
  
  public LBXMultiTable()
  {
    for (LBXID id : LBXID.values())
    {
      if (id.isGfx)
      {
        int c = LBX.getEntryCount(id.path+".lbx");
        List<SpriteInfo> entries = IntStream.range(0, c).mapToObj(i -> new SpriteInfoLBX(id.path+".lbx", i)).collect(Collectors.toList());
        
        LBXTable lbxTable = new LBXTable(DataSource.of(entries));
        TablePanel lbxTablePanel = new TablePanel(lbxTable, new Dimension(800,600));
        tabs.addTab(id.path, lbxTablePanel);  
      }
    }
    
    this.setLayout(new BorderLayout());
    this.add(tabs, BorderLayout.CENTER);
    
    
  }
}
