package com.github.jakz.openmom;

import java.awt.Dimension;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.esotericsoftware.yamlbeans.YamlReader;
import com.github.jakz.openmom.data.Unit;
import com.github.jakz.openmom.lbx.LBX;
import com.github.jakz.openmom.lbx.SpriteSheet;
import com.github.jakz.openmom.ui.TablePanel;
import com.github.jakz.openmom.ui.UnitTable;
import com.github.jakz.openmom.yaml.YamlNode;
import com.github.jakz.openmom.yaml.YamlParser;
import com.github.jakz.openmom.yaml.unserializers.ReflectiveUnserializer;
import com.pixbits.lib.lang.Pair;
import com.pixbits.lib.ui.UIUtils;
import com.pixbits.lib.ui.WrapperFrame;
import com.pixbits.lib.ui.color.Color;
import com.pixbits.lib.ui.color.Palette;
import com.pixbits.lib.ui.table.DataSource;

public class App 
{
  static
  {
    try
    {
      System.loadLibrary("lbx");
    }
    catch (UnsatisfiedLinkError e)
    {
      e.printStackTrace();
      System.exit(1);
    }
  }
  
  public static void main( String[] args )
  {
    try
    {      
      SpriteSheet sprite = LBX.loadSprite("armylist.lbx", 0);
      
      {
        JLabel label = new JLabel(new ImageIcon(sprite.get(0)));
        JPanel panel = new JPanel();
        panel.add(label);
        WrapperFrame<?> frame = UIUtils.buildFrame(panel, "Units");
        frame.exitOnClose();
        frame.setVisible(true);
      }

      
      if (true)
        return;
      
      Path path = Paths.get("../../data/yaml/units.yaml");
      YamlNode root = new YamlParser().parse(path).get("units");
      
      ReflectiveUnserializer<Unit> uns = new ReflectiveUnserializer<>(Unit.class);
      
      List<Unit> units = new ArrayList<>();
      
      for (YamlNode node : root)
      {
        Unit unit = uns.unserialize(node);
        units.add(unit);
        System.out.println(unit.identifier+", "+unit.type+", "+unit.upkeep+", "+unit.visuals.i18n);        
      }
      
      UIUtils.setNimbusLNF();
      
      UnitTable unitTable = new UnitTable(DataSource.of(units));
      TablePanel panel = new TablePanel(unitTable, new Dimension(1024,600));
      
      WrapperFrame<?> frame = UIUtils.buildFrame(panel, "Units");
      frame.exitOnClose();
      frame.setVisible(true);
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }
}
