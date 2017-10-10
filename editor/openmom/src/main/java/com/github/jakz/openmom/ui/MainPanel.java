package com.github.jakz.openmom.ui;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import com.github.jakz.openmom.Data;
import com.pixbits.lib.ui.table.DataSource;

public class MainPanel extends JPanel
{
  private JTabbedPane tabs;
  
  private UnitTable unitTable;
  private RaceTable raceTable;
  
  public MainPanel(Data data)
  {
    tabs = new JTabbedPane(JTabbedPane.LEFT);

    raceTable = new RaceTable(data.races);
    TablePanel raceTablePanel = new TablePanel(raceTable, new Dimension(1024, 600));
    tabs.addTab("Races", raceTablePanel);
    
    unitTable = new UnitTable(data.units);
    TablePanel unitTablePanel = new TablePanel(unitTable, new Dimension(1024,600));
    tabs.addTab("Units", unitTablePanel);  

    this.setLayout(new BorderLayout());
    this.add(tabs, BorderLayout.CENTER);
  }
}
