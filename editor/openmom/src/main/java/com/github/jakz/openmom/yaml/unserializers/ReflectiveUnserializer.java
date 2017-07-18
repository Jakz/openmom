package com.github.jakz.openmom.yaml.unserializers;

import java.lang.reflect.Field;

import com.github.jakz.openmom.yaml.YamlException;
import com.github.jakz.openmom.yaml.YamlNode;
import com.github.jakz.openmom.yaml.YamlUnserializer;

public class ReflectiveUnserializer<T> implements YamlUnserializer<T>
{
  private final Class<T> type;
  private final Field[] fields;
  
  public ReflectiveUnserializer(Class<T> type)
  {
    this.type = type;
    this.fields = type.getFields();
  }

  @Override
  public T unserialize(YamlNode node)
  {
    T object;
    
    try
    {
      object = type.newInstance();
    }
    catch (Exception e)
    {
      throw new YamlException("Unable to istantiate type "+type.getSimpleName(), e);
    }
    
    try
    {
      for (Field field : fields)
      {
        YamlNode child = node.get(field.getName());
        
        if (child.exists())
        {
          YamlUnserializer<?> unserializer = node.environment().findUnserializer(field.getType());
          field.set(object, unserializer.unserialize(child));
        }
      }
    } 
    catch (IllegalArgumentException | IllegalAccessException e)
    {
      e.printStackTrace();
    }
    
    return object;
  }
}
