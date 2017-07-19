package com.github.jakz.openmom;

import java.awt.Dimension;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.esotericsoftware.yamlbeans.YamlReader;
import com.github.jakz.openmom.data.Unit;
import com.github.jakz.openmom.ui.TablePanel;
import com.github.jakz.openmom.ui.UnitTable;
import com.github.jakz.openmom.yaml.YamlNode;
import com.github.jakz.openmom.yaml.YamlParser;
import com.github.jakz.openmom.yaml.unserializers.ReflectiveUnserializer;
import com.pixbits.lib.lang.Pair;
import com.pixbits.lib.ui.UIUtils;
import com.pixbits.lib.ui.WrapperFrame;
import com.pixbits.lib.ui.table.DataSource;

public class App 
{
  public static void main( String[] args )
  {
    try
    {
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
