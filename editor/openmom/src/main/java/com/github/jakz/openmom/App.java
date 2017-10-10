package com.github.jakz.openmom;

import java.awt.Dimension;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.function.Function;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.esotericsoftware.yamlbeans.YamlReader;
import com.github.jakz.openmom.data.HouseType;
import com.github.jakz.openmom.data.Race;
import com.github.jakz.openmom.data.Ranged;
import com.github.jakz.openmom.data.SpriteInfo;
import com.github.jakz.openmom.data.SpriteInfoLBX;
import com.github.jakz.openmom.data.Unit;
import com.github.jakz.openmom.lbx.LBX;
import com.github.jakz.openmom.lbx.SpriteSheet;
import com.github.jakz.openmom.ui.MainPanel;
import com.github.jakz.openmom.ui.TablePanel;
import com.github.jakz.openmom.ui.UnitTable;
import com.pixbits.lib.lang.Pair;
import com.pixbits.lib.ui.UIUtils;
import com.pixbits.lib.ui.WrapperFrame;
import com.pixbits.lib.ui.color.Color;
import com.pixbits.lib.ui.color.Palette;
import com.pixbits.lib.ui.table.DataSource;
import com.pixbits.lib.ui.table.ModifiableDataSource;
import com.pixbits.lib.yaml.YamlNode;
import com.pixbits.lib.yaml.YamlParser;
import com.pixbits.lib.yaml.YamlUnserializer;
import com.pixbits.lib.yaml.unserializer.ReflectiveUnserializer;
import com.pixbits.lib.yaml.unserializer.EnumUnserializer;
import com.pixbits.lib.yaml.unserializer.ListUnserializer;

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
  
  static final Function<String, String> fieldNameFromYaml = n -> {
    StringBuilder b = new StringBuilder();
    
    for (int i = 0; i < n.length(); ++i)
    {
      if (n.charAt(i) == '_' && i < n.length()-1)
        b.append(Character.toUpperCase(n.charAt(i+1)));
      else
        b.append(n.charAt(i));
    }
    
    return b.toString();
  };
  
  static final Function<String, String> fieldNameToYaml = n -> {
    StringBuilder b = new StringBuilder();
    
    for (int i = 0; i < n.length(); ++i)
    {
      char c = n.charAt(i);
      if (Character.isUpperCase(c))
        b.append('_').append(Character.toLowerCase(c));
      else
        b.append(c);
    }
    
    return b.toString();
  };
  
  
  public static void main( String[] args )
  {
    try
    {      
      final Path base = Paths.get("../../data/yaml");
      final Data data = new Data();
      
      YamlParser parser = new YamlParser();
      
      parser.setReflectiveUnserializeFieldRemapper(fieldNameToYaml);
      
      parser.registerUnserializer(SpriteInfo.class, y -> {
        if (y.get(0).asString().equals("lbx"))
        {
          String lbx = y.get(1).asString() + ".lbx";
          int index = y.get(2).asInt();
          return new SpriteInfoLBX(lbx, index);
        }
        
        return null;
      });
      
      parser.registerUnserializer(Ranged.class, y -> {
        if (y.size() != 3)
          return new Ranged(Ranged.Type.none);
        else
        {
          int strength = y.get(0).asInt();
          Ranged.Type type = y.environment().findUnserializer(Ranged.Type.class).unserialize(y.get(1));
          int ammo = y.get(2).asInt();
          return new Ranged(strength, type, ammo);
        }
      });
      
      parser.registerUnserializer(HouseType.class, new EnumUnserializer<HouseType>(HouseType.class));
      
      


      /* races.yaml */
      {
        Path path = base.resolve("races.yaml");
        
        YamlNode root = parser.parse(path).get("races");
        YamlUnserializer<List<Race>> unserializer = new ListUnserializer<>(Race.class);
        List<Race> races = unserializer.unserialize(root);
        data.races = ModifiableDataSource.of(races);
      }
      
      
      /* units.yaml */
      {
        Path path = base.resolve("units.yaml");
     

        
        YamlNode root = parser.parse(path).get("units");
        
        ReflectiveUnserializer<Unit> uns = new ReflectiveUnserializer<>(Unit.class);
        
        List<Unit> units = new ArrayList<>();
        
        for (YamlNode node : root)
        {
          Unit unit = uns.unserialize(node);
          units.add(unit);
          System.out.println(unit.identifier+", "+unit.type+", "+unit.upkeep+", "+unit.visuals.i18n);        
        }
        
        data.units = ModifiableDataSource.of(units);

      }

      UIUtils.setNimbusLNF();

      MainPanel mainPanel = new MainPanel(data);
      
      WrapperFrame<?> frame = UIUtils.buildFrame(mainPanel, "OpenMoM Editor v0.1");
      frame.exitOnClose();
      frame.setVisible(true);
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }
}
