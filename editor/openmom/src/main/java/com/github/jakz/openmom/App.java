package com.github.jakz.openmom;

import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.Map;

import com.esotericsoftware.yamlbeans.YamlReader;
import com.github.jakz.openmom.data.Unit;
import com.github.jakz.openmom.yaml.YamlNode;
import com.github.jakz.openmom.yaml.YamlParser;
import com.github.jakz.openmom.yaml.unserializers.ReflectiveUnserializer;
import com.pixbits.lib.lang.Pair;

public class App 
{
  public static void main( String[] args )
  {
    try
    {
      Path path = Paths.get("../../data/yaml/units.yaml");
      YamlNode root = new YamlParser().parse(path).get("units");
      
      ReflectiveUnserializer<Unit> uns = new ReflectiveUnserializer<>(Unit.class);
      
      for (YamlNode node : root)
      {
        Unit unit = uns.unserialize(node);
        System.out.println(unit.identifier+", "+unit.type+", "+unit.upkeep+", "+unit.visuals.i18n);        
      }
    }
    catch (Exception e)
    {
      e.printStackTrace();
    }
  }
}
