package com.github.jakz.openmom.ui;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;

public class TablePanel extends JPanel
{
  private JTable table;
  private JScrollPane scrollPane;
  
  public TablePanel(JTable table, Dimension dimension)
  {
    this.table = table;
    this.scrollPane = new JScrollPane(table);
    scrollPane.setPreferredSize(dimension);
    this.setLayout(new BorderLayout());
    this.add(scrollPane, BorderLayout.CENTER);
  }
  
}

