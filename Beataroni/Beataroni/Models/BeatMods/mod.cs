using System.Collections.Generic;

#pragma warning disable CS0169

namespace Beataroni.Models.BeatMods
{
    public class Author
    {
        public string _id{get;set;}
        public string username{get;set;}
        public string lastLogin{get;set;}
    }

    public class Hash
    {
        public string hash{get;set;}
        public string file{get;set;}
    }

    public class Download
    {
        string type{get;set;}
        string url{get;set;}
        List<Hash> hashMd5{get;set;}
    }

    public class Mod
    {
        public string name{get;set;}
        public string version{get;set;}
        public string gameVersion{get;set;}
        public string authorId{get;set;}
        public string uploadDate{get;set;}
        public string updatedDate{get;set;}
        Author author{get;set;}
        public string status{get;set;}
        public string description{get;set;}
        public string link{get;set;}
        public string category{get;set;}
        List<Download> downloads{get;set;}
        public bool required{get;set;}
        public string _id{get;set;}
    }
}