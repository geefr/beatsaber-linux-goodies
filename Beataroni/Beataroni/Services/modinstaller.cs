using Beataroni.Models.BeatMods;
using Beataroni.ViewModels;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Beataroni.Services
{
  public class ModInstaller
  {
    private List<string> BSPathsToFix = new List<string>
    {
      "Beat Saber_Data",
      "DLC",
      "MonoBleedingEdge",
      "Plugins",
      "UserData",
      "IPA",
      "Libs",
      "CustomAvatars",
      "CustomSabers",
      "CustomNotes",
      "CustomCampaigns",
      "CustomPlatforms",
      "Playlists",
    };

    public delegate void InstallLogLine(string msg);

    /**
     * Ensure the case of path is correct, so that files are placed
     * in the correct folders under BS install
     */
    private string FixPath(string path)
    {
      var foundPath = BSPathsToFix.Find(x =>
      {
        if (path.Length < x.Length) return false;
        var testPath = path.Remove(x.Length);
        return string.Equals(testPath, x, StringComparison.OrdinalIgnoreCase);
      });
      if (string.IsNullOrEmpty(foundPath)) return path;
      return $"{foundPath}{path.Substring(foundPath.Length)}";
    }

    public bool InstallMod(Mod m, string bsInstall, InstallLogLine log)
    {
      foreach (var dl in m.downloads)
      {
        if (!(dl.type.Equals("steam") || dl.type.Equals("universal")))
        {
          // TODO: At the moment Beataroni only supports steam installs - As Oculus doesn't work on Linux
        }

        // Download the file
        var tmpFile = Path.GetTempFileName();
        if (!BeatModsV1.DownloadMod(dl, tmpFile))
        {
          return false;
        }

        // Extract the archive
        bsInstall = Path.GetFullPath(bsInstall);

        // Ensures that the last character on the extraction path
        // is the directory separator char.
        // Without this, a malicious zip file could try to traverse outside of the expected
        // extraction path.
        if (!bsInstall.EndsWith(Path.DirectorySeparatorChar.ToString(), StringComparison.Ordinal))
          bsInstall += Path.DirectorySeparatorChar;

        try
        {
          using (ZipArchive archive = ZipFile.OpenRead(tmpFile))
          {
            foreach (ZipArchiveEntry entry in archive.Entries)
            {
              // Ported from QBeat/util.cpp
              foreach (var correctPath in BSPathsToFix)
              {
                var path = entry.FullName;
                if (path.StartsWith(correctPath, StringComparison.OrdinalIgnoreCase))
                {
                  path = path.Remove(0, correctPath.Length);
                  path = $"{correctPath}{path}";
                }
              }

              // Gets the full path to ensure that relative segments are removed.
              string destinationPath = Path.GetFullPath(Path.Combine(bsInstall, entry.FullName));

              // Ordinal match is safest, case-sensitive volumes can be mounted within volumes that
              // are case-insensitive.
              if (destinationPath.StartsWith(bsInstall, StringComparison.Ordinal))
              {
                if (entry.FullName.EndsWith('/'))
                {
                  // It's not well documented, but this is a directory
                  Directory.CreateDirectory(destinationPath);
                }
                else
                {
                  // Otherwise it must be a file
                  Directory.CreateDirectory(Path.GetDirectoryName(destinationPath));
                  entry.ExtractToFile(destinationPath, true);

                  //log($"Installed: {destinationPath}");
                }
              }
            }
          }

          File.Delete(tmpFile);
        }
        catch( Exception e )
        {
          log($"Error Extracting Mod: {dl.url}, {e.Message}");
          return false;
        }
      }
      return true;
    }

    public bool PatchBeatSaber(string bsInstall, InstallLogLine log)
    {
      // To patch beat saber we just need to run IPA.exe in the
      // directory
      // This method requires that BSIPA be installed first
      //log("Patching Beat Saber");

      var runningOnLinux = RuntimeInformation.IsOSPlatform(OSPlatform.Linux);
      var ipaExe = runningOnLinux ? "IPA-Minimal" : "IPA.exe";
      if (runningOnLinux)
      {
        if (!PatchSteamProtonPrefix(bsInstall, log))
        {
          Console.WriteLine("PatchBeatSaber: Failed to patch steam's proton prefix");
          return false;
        }

        // Copy the Linux build of IPA into bsInstall
        // TODO: The packaging/build setup for this binary isn't great, consider
        // pulling it in directly as a library?
        try
        {
          var roniDir = "";
          if (Debugger.IsAttached)
          {
            // This is quite annoying. When running under the debugger the executing process
            // is in /usr/share/dotnet, which sadly isn't where the Beataroni install is
            // Under a debugger however we're not packaged as a single executable,
            // so can look up the current assembly to work out where we are
            // TODO: Using the dev copy for now, this won't be especially reliable
            // roniDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            // roniDir = $"{roniDir}/../../../../../IPA/bin-minimal-linux64/";
          }
          else
          {
            // When not debugging we have to assume we're running in a packaged form
            // So get the Beataroni dir based on the current root process.
            // In this case we can't use the current assembly as this is actually a copy
            // in a temporary dir one the single-file executable has been unpacked
            // TODO: Need some kind of preprocessor dance here to specify if we're
            // packaged as a single-file or not, because if we're not then the
            // executing assembly is the system dotnet runtime again, like under
            // debug
            roniDir = System.AppContext.BaseDirectory;
            // roniDir = Path.GetDirectoryName(Path.GetDirectoryName(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName));
          }

          log($"INFO: Linux Patching: roniDir: {roniDir}");
          File.Copy($"{roniDir}/{ipaExe}", $"{bsInstall}/{ipaExe}", true);
        }
        catch(Exception e)
        {
          log($"PatchBeatSaber: Failed to install IPA-Minimal: {e.Message}");
          return false;
        }
      }

      try
      {
        Process proc = new Process();
        proc.StartInfo.UseShellExecute = false;
        proc.StartInfo.FileName = $"{bsInstall}/{ipaExe}";
        proc.StartInfo.WorkingDirectory = $"{bsInstall}";
        proc.StartInfo.Arguments = "-n"; // Don't wait for user input
        proc.StartInfo.CreateNoWindow = true;
        proc.StartInfo.RedirectStandardOutput = true;
        proc.StartInfo.RedirectStandardError = true;
        proc.Start();
        // Assuming proc will kill itself here, if not we'll hang, or need to use the Kill method
        proc.WaitForExit();
        if( proc.ExitCode != 0 )
        {
          log($"PatchBeatSaber: IPA.exe returned non-zero({proc.ExitCode}):\n StdOut: {proc.StandardOutput.ReadToEnd()} \n StdErr: {proc.StandardError.ReadToEnd()}");
          return false;
        }
      }
      catch (Exception e)
      {
        log(e.Message);
        return false;
      }
      return true;
    }

    public bool PatchSteamProtonPrefix(string bsInstall, InstallLogLine log)
    {
      // Make a small modification to beat saber's proton prefix
      // in order to ensure winhttp.dll is loaded from IPA instead
      // of using the built-in wine variant
      //log("Patching Proton Prefix");
      try
      {
        var pfxFile = $"{bsInstall}/../../compatdata/620980/pfx/user.reg";
        using (StreamWriter w = File.AppendText(pfxFile))
        {
          w.WriteLine("[Software\\Wine\\DllOverrides]");
          w.WriteLine("\"winhttp\"=\"native,builtin\"");
        }
      } 
      catch(Exception e)
      {
        log($"PatchSteamProtonPrefix: Failed to patch pfx/user.reg: {e.Message}");
        return false;
      }
      return true;
    }

    public bool UninstallMod(Mod m, InstallLogLine log)
    {
      // TODO
      return false;
    }

    public bool ValidateMod(Mod m, InstallLogLine log)
    {
      // TODO
      return false;
    }

    public bool IsModInstalled(Mod m, InstallLogLine log)
    {
      // TODO - Return true if mod at least partially installed
      return false;
    }
  }
}
