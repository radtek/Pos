using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace SourceVersionUpdater {
    public class SvuPatcher {
        void
        PatchFile(
            SvuPatchData fileToUpdate,
            String changeMatchTo)
        {
            String[] contents =
                ReadFileContents(fileToUpdate.PatchFilePath);
            Match m;
            Regex re = new Regex(fileToUpdate.PatchPattern);

            fileToUpdate.BackupFile();

            for (int i = 0; i < contents.Length; i++)
                if ((m = re.Match(contents[i])).Success)
                    contents[i] = re.Replace(contents[i], changeMatchTo);

            WriteFileContents(contents, fileToUpdate.PatchFilePath);
        }

        public void
        PerformPatch()
        {
            String replacement = ScrapeSource();

            foreach (SvuPatchData f in filesToPatch)
                PatchFile(f, replacement);
        }

        String[]
        ReadFileContents(
            String sourcePath)
        {
            List<string> accum = new List<string>();
            StreamReader sr = new StreamReader(sourcePath);
            String m = sr.ReadLine();

            while (m != null)
            {
                accum.Add(m);
                m = sr.ReadLine();
            }

            sr.Close();
            return accum.ToArray();
        }

        String
        ScrapeSource()
        {
            String[] c = ReadFileContents(scrapeFromFile.FullName);
            Match m;
            Regex re = new Regex(scrapePattern);

            foreach (string n in c)
                if ((m = re.Match(n)).Success)
                    return Regex.Replace(m.Groups[1].Captures[0].Value,
                                         @"\,", ".");;

            return null;
        }

        public
        SvuPatcher(
            List<SvuPatchData> filesToPatch,
            FileInfo scrapeFromFile,
            String scrapePattern)
        {
            this.filesToPatch = filesToPatch;
            this.scrapePattern = scrapePattern;
            this.scrapeFromFile = scrapeFromFile;
        }

        ~SvuPatcher()
        {
        }

        void
        WriteFileContents(
            String[] content,
            String targetPath)
        {
            StreamWriter sw = new StreamWriter(targetPath);

            foreach (string m in content)
                sw.WriteLine(m);

            sw.Flush();
            sw.Close();
        }

        List<SvuPatchData> filesToPatch = null;
        FileInfo scrapeFromFile = null;
        String scrapePattern = null;
    }

    public class SvuPatchData {
        public void
        BackupFile()
        {
            patchThisFile.CopyTo(patchThisFile.FullName + ".orig");
        }

        public
        SvuPatchData(
            String patchPattern,
            FileInfo patchThisFile)
        {
            this.patchPattern = patchPattern;
            this.patchThisFile = patchThisFile;
        }

        ~SvuPatchData()
        {
        }

        public string PatchFilePath {
            get {
                return patchThisFile.FullName;
            }
        }

        public string PatchPattern {
            get {
                return patchPattern;
            }
        }

        FileInfo patchThisFile = null;
        String patchPattern = null;
    };

    public class SvuUserInterface {
        public
        SvuUserInterface()
        {
        }

        ~SvuUserInterface()
        {
        }

        public static int
        Main(
            String[] arguments)
        {
            List<SvuPatchData> filesToPatch;
            SvuPatcher patcher;

            if (arguments.Length < 2) {
                Console.WriteLine(
                    "SvuPatcher.exe VersionResFile FileToUpdate1 ... FileToUpdateN");
                return -1;
            }

            filesToPatch = new List<SvuPatchData>();
            
            for (int i = 1; i < arguments.Length; i++)
                filesToPatch.Add(new SvuPatchData("__MM_VERSION__",
                                                  new FileInfo(arguments[i])));

            patcher = new SvuPatcher(filesToPatch,
                                     new FileInfo(arguments[0]),
                                     @".*PRODUCTVER\s*(.*)");

            patcher.PerformPatch();

            return 0;
        }
    };
};
