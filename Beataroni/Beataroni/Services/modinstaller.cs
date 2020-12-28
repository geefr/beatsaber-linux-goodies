using Beataroni.Models.BeatMods;
using Beataroni.ViewModels;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using IPA;

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
    private string FixPath(string entry)
    {
      // Ported from QBeat/util.cpp
      foreach (var correctPath in BSPathsToFix)
      {
        var path = entry;
        if (path.StartsWith(correctPath, StringComparison.OrdinalIgnoreCase))
        {
          path = path.Remove(0, correctPath.Length);
          path = $"{correctPath}{path}";
          return path;
        }
      }
      return entry;
    }

    public bool InstallMod(Mod m, string bsInstall, InstallLogLine log)
    {
      foreach (var dl in m.downloads)
      {
        if (!(dl.type.Equals("steam") || dl.type.Equals("universal")))
        {
          // TODO: At the moment Beataroni only supports steam installs - As Oculus doesn't work on Linux
          continue;
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
              var path = FixPath(entry.FullName);

              // Gets the full path to ensure that relative segments are removed.
              string destinationPath = Path.GetFullPath(Path.Combine(bsInstall, path));

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
        catch (Exception e)
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
      
      if (runningOnLinux)
      {
        if (!PatchSteamProtonPrefix(bsInstall, log))
        {
          Console.WriteLine("PatchBeatSaber: Failed to patch steam's proton prefix");
          return false;
        }
      }

      try
      {
	      log($"PatchBeatSaber: Running IPA installation");

        // TODO: Should rework IPA-Minimal lib to return status code, need to detect if things fail
        // TODO: Should capture stdout/stderr from IPA-Minimal lib, can we do that without major rework?
        var origPwd = Directory.GetCurrentDirectory();
        Directory.SetCurrentDirectory(bsInstall);
        string[] args = {"IPA-Minimal", "-n", "-f", "Beat Saber.exe", "--relativeToPwd"};
        IPA.Program.Main(args);
        Directory.SetCurrentDirectory(origPwd);

        // if (proc.ExitCode != 0)
        // {
        //   log($"PatchBeatSaber: IPA returned non-zero({proc.ExitCode}):\n StdOut: {proc.StandardOutput.ReadToEnd()} \n StdErr: {proc.StandardError.ReadToEnd()}");
        //   return false;
        // }
	      // } else {
        //   log("PatchBeatSaber: Failed to start IPA process, don't know why...");
        // }
      }
      catch (Exception e)
      {
        log(e.ToString());
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
          // We need double-slashes within the resulting file
          w.WriteLine("[Software\\\\Wine\\\\DllOverrides]");
          w.WriteLine("\"winhttp\"=\"native,builtin\"");
        }
      }
      catch (Exception e)
      {
        log($"PatchSteamProtonPrefix: Failed to patch pfx/user.reg:\n {e.Message}");
        return false;
      }
      return true;
    }

    /// <summary>
    /// Uninstall/delete a mod from the system
    /// TODO: If the installed version of a mod has a different file list than
    /// the current version this might not work correctly. May need to track
    /// installed mod versions as well, which tbh would be helpful functionality
    /// for updates.
    /// </summary>
    public void UninstallMod(Mod m, string bsInstall, InstallLogLine log)
    {
      foreach (var dl in m.downloads)
      {
        if (!(dl.type.Equals("steam") || dl.type.Equals("universal")))
        {
          // TODO: At the moment Beataroni only supports steam installs - As Oculus doesn't work on Linux
          continue;
        }

        foreach (var hash in dl.hashMd5)
        {
          var path = FixPath(hash.file);
          path = Path.Combine(bsInstall, path);
          try
          {
            File.Delete(path);
          }
          catch(Exception)
          {
            // We don't care
          }
        }
      }
    }

    /// <summary>
    /// Validate installed files for a mod against beatmod's hashes
    /// </summary>
    public bool ValidateMod(Mod m, string bsInstall, InstallLogLine log)
    {
      foreach (var dl in m.downloads)
      {
        if (!(dl.type.Equals("steam") || dl.type.Equals("universal")))
        {
          // TODO: At the moment Beataroni only supports steam installs - As Oculus doesn't work on Linux
          continue;
        }

        foreach (var hash in dl.hashMd5)
        {
          var path = FixPath(hash.file);
          path = Path.Combine(bsInstall, path);
          try
          {
            using (var md5 = MD5.Create())
            {
              using (var stream = File.OpenRead(path))
              {
                var fileHash = BitConverter.ToString(md5.ComputeHash(stream)).Replace("-", "");
                if (!string.Equals(fileHash, hash.hash, StringComparison.OrdinalIgnoreCase))
                {
                  log($"ValidateMod: Failed to validate {path} : Expected {hash.hash} : Actual {fileHash}");
                  return false;
                }
              }
            }
          }
          catch (Exception e)
          {
            log($"ValidateMod: Failed to calc md5 for {path}: {e.Message}");
          }
        }
      }
      return true;
    }

    /// <summary>
    /// Check if a mod is (partially) installed
    /// Intention of this method is to intialise the list of installed mods,
    /// and any selected mods will be reinstalled/updated after mod selection
    /// TODO: If we just overwrite already-installed mods does that break any
    /// config files, reset things like twitch credentials?
    /// </summary>
    public bool IsModInstalled(Mod m, string bsInstall)
    {
      var anyPresent = false;
      foreach (var dl in m.downloads)
      {
        if (!(dl.type.Equals("steam") || dl.type.Equals("universal")))
        {
          // TODO: At the moment Beataroni only supports steam installs - As Oculus doesn't work on Linux
          continue;
        }

        foreach (var hash in dl.hashMd5)
        {
          var path = FixPath(hash.file);
          path = Path.Combine(bsInstall, path);

          if( File.Exists(path) ) {
            anyPresent = true;
          }
        }
      }
      return anyPresent;
    }
  }
}
