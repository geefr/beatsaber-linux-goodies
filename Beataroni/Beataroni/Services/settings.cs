using System;

namespace Beataroni.Services
{
  public class Settings
  {
    // TODO: Most of these are blank - will be initialised to sensible defaults,
    // or need to be setup each time installer is launched. Will need persistent settings
    // storage for this.
    public string BSVersion {get;set;} = "";
    public string BSInstall {get;set;} = "";

    public Settings()
    {
      // var vers = BeatModsV1.FetchBSVersions();
      // if( vers.Count != 0 ) BSVersion = vers[0];
      // BSInstall = SearchForBeatSaber();
    }

    private string SearchForBeatSaber()
    {
      // TODO:
      return "";
    }
  }
}
