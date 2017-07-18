package com.github.jakz.openmom.yaml;

import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Path;

import com.esotericsoftware.yamlbeans.YamlReader;

public class YamlParser
{
  private final YamlEnvironment env;
  
  public YamlParser()
  {
    this.env = new YamlEnvironment(this);
  }
  
  public YamlNode parse(Path path) throws IOException
  {
    try (FileReader freader = new FileReader(path.toString()))
    {
      YamlReader reader = new YamlReader(freader);
      YamlNode root = new YamlNode(env, reader.read());
      return root;  
    }
  }
}
