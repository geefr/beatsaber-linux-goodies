using System;
using System.Collections.Generic;
using System.Text.Json.Serialization;

#pragma warning disable CS0169

namespace Beataroni.Models.BeatMods
{

  /// <summary>
  /// Root-level Mod instance from the BeatMods api
  /// </summary>
  public class Mod
  {
    public string name { get; set; }
    public string version { get; set; }
    public string gameVersion { get; set; }
    public string authorId { get; set; }
    public string uploadDate { get; set; }
    public string updatedDate { get; set; }
    public Author author { get; set; }
    public string status { get; set; }
    public string description { get; set; }
    public string link { get; set; }
    public string category { get; set; }
    public List<Download> downloads { get; set; }
    public bool required { get; set; }
    public List<ModDependency> dependencies { get; set; }
    public string _id { get; set; }

    /// <summary>
    /// An expanded list of dependencies, translated from ModDependency to Mod references
    /// </summary>
    [JsonIgnore]
    public HashSet<Mod> DependencyRefs = new HashSet<Mod>();
    public void ExpandDependencyRefs( List<Mod> mods )
    {
      foreach( var d in dependencies )
      {
        var m = mods.Find(x => x.name.Equals(d.name));
        if( m != null )
        {
          DependencyRefs.Add(m);
        }
        else
        {
          Console.WriteLine($"ExpandDependencyRefs[{_id}:{name}]: Couldn't find mod {d._id} : {d.name}");
        }
      }
    }


    /// <summary>
    /// For a given mod extract all of its dependencies into a flat set, recursing if needed
    /// </summary>
    /// <param name="mod">The mod to process</param>
    /// <param name="dependencies">All of the mod's depdendencies, and their dependencies will be added to this set</param>
    public static void GetDependencySet(Mod mod, ref HashSet<Mod> dependencies)
    {
      foreach( var d in mod.DependencyRefs )
      {
        dependencies.Add(d);
        GetDependencySet(d, ref dependencies);
      }
    }
  }

  /// Comparator class for Mod
  public class ModComparatorByName : IComparer<Mod>
  {
    public int Compare(Mod a, Mod b) => string.Compare(a.name, b.name, true);
  }


  /// <summary>
  /// A Mod within Mod.dependencies
  /// This is only a different class due to the way BeatMods returns dependencies
  /// - [0].dependencies[0] -> Structure matches the Mod class
  /// - [0].dependencies[0].dependencies[0] -> Just a mod ID
  /// So this class is just a cut down Mod, with enough information to select a proper mod from the list later on
  /// </summary>
  public class ModDependency
  {
    public string name { get; set; }
    public string _id { get; set; }
    public List<string> dependencies { get; set; }
  }

  public class Author
  {
    public string _id { get; set; }
    public string username { get; set; }
    public string lastLogin { get; set; }
  }

  public class Hash
  {
    public string hash { get; set; }
    public string file { get; set; }
  }

  public class Download
  {
    public string type { get; set; }
    public string url { get; set; }
    public List<Hash> hashMd5 { get; set; }
  }


  
}