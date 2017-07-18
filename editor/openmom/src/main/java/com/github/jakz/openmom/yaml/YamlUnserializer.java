package com.github.jakz.openmom.yaml;

@FunctionalInterface
public interface YamlUnserializer<T>
{
  public T unserialize(YamlNode node);
}
