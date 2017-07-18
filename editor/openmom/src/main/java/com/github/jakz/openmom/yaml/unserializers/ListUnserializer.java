package com.github.jakz.openmom.yaml.unserializers;

import java.util.ArrayList;
import java.util.List;

import com.github.jakz.openmom.yaml.YamlException;
import com.github.jakz.openmom.yaml.YamlNode;
import com.github.jakz.openmom.yaml.YamlUnserializer;

public class ListUnserializer<T> implements YamlUnserializer<T>
{
  Class<T> type;
  
  public ListUnserializer(Class<T> type)
  {
    this.type = type;
  }
  
  @Override
  public T unserialize(YamlNode node)
  {
    return null;
  }

}
