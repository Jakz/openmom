package com.github.jakz.openmom.yaml;

import java.util.Iterator;

public class YamlNodeIterator implements Iterator<YamlNode>
{
  private final YamlNode node;
  private final int size;
  int current;
  
  YamlNodeIterator(YamlNode node)
  {
    this.node = node;
    this.size = node.size();
  }
  
  @Override
  public boolean hasNext()
  {
    return current < size;
  }

  @Override
  public YamlNode next()
  {
    return node.get(current++);
  }

}
