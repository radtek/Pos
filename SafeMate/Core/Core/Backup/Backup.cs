using SC = Safemate.Core;
using SCB = Safemate.Core.Backup;
using SCC = Safemate.Core.Crm;
using SCLP = Safemate.Core.LogProxy;
using SCBE = Safemate.Core.Backup.Exceptions;

using FC = FileMate.Core;

using FBDC = FirebirdSql.Data.FirebirdClient;
using FBDS = FirebirdSql.Data.Services;

using IZ = Ionic.Zip;

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Security;
using System.Threading;

namespace Safemate.Core.Backup
{
    static public class Backup
    {
        static public void AddBackupOperation(String menumatePath,
                                              IEnumerable<FC.IFileRepository> targets)
        {
            if (backupOperations.Count > 1)
                throw new SCBE.RequestUnsupported(
                    "Multiple backup operations are not yet supported.",
                    null);
            else {
                DirectoryInfo menumateDir =
                    new DirectoryInfo(menumatePath);

                if (!menumateDir.Exists)
                    throw new Exception(String.Format(
                                            "The Menumate path \"{0}\" doesn't exist!",
                                            menumatePath));

                foreach (FileInfo db in
                         menumateDir.EnumerateFiles("*.fdb"))
                    backupOperations.Add(
                        new QueuedBackupOperation(
                            db.FullName, targets));
            }
        }

        static Backup()
        {
            FBDC.FbConnectionStringBuilder connectionInfo =
                new FBDC.FbConnectionStringBuilder();

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Backup Manager is now initializing...");

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Creating cache directories if necessary...");

            cacheDirectory = new DirectoryInfo(
                SC.Configuration.SafemateRootPath +
                @"\Cache");
            creationCacheDirectory = new DirectoryInfo(
                cacheDirectory.FullName + @"\Build");

            backupOperations =
                new List<QueuedBackupOperation>();

            backupService = new FBDS.FbBackup();

            restorationService = new FBDS.FbRestore();
            restorationPath = String.Format(
                                  "{0}\\{1}", creationCacheDirectory.FullName,
                                  "RestorationTesting.fdb");
            restorationService.Options =
                FBDS.FbRestoreFlags.Replace;

            connectionInfo.Charset = "NONE";
            connectionInfo.Database = restorationPath;
            connectionInfo.DataSource = "localhost";
            connectionInfo.Password = "masterkey";
            connectionInfo.UserID = "sysdba";

            restorationService.ConnectionString =
                connectionInfo.ToString();

            backupService.Verbose = restorationService.Verbose = true;

            backupService.ServiceOutput += ServiceOutputHandler;
            restorationService.ServiceOutput += ServiceOutputHandler;

#if __Safemate_Core_Backup_Debug
            fragmentThreshold = 1 << 30;
#endif
            CreateCreationCache();
        }

        static public void Cleanup()
        {
            if (backupOperations != null)
                backupOperations.Clear();
        }

        static void ClearCaches()
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Cleaning caches of artefacts. Cache contents redundant as new backup has been issued.");
            for (int i = 0; i < 8; i++)
                try {
                    foreach (DirectoryInfo cd in
                             cacheDirectory.EnumerateDirectories())
                        foreach (FileInfo f in cd.EnumerateFiles())
                            f.Delete();
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                      "Caches cleaned successfully.");
                    return;
                } catch (Exception e) {
                    if (i == 7)
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          "Couldn't clear caches, expect the next backup to be larger than usual.");
                    else {
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          "Cache clear failed! Waiting 5 seconds for retry...");
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          String.Format("Cause reports was: {0}\n", e.Message));
                        Thread.Sleep(5000);
                    }
                }
        }

        static void CompressBackups()
        {
            backupArchive = new IZ.ZipFile(
                String.Format("{0}\\menumate-backup-{1}.zip",
                              creationCacheDirectory.FullName,
                              backupTime));
            IEnumerable<FileInfo> files =
                creationCacheDirectory.EnumerateFiles("*.fbk");

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              String.Format(
                                  "Archiving backup to {0} for transit...",
                                  backupArchive.Name));

            backupArchive.SaveProgress +=
                ZipSaveCompletionHandler;

            foreach (FileInfo f in files) {
                SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                  String.Format(
                                      "Archiving {0} ...", f.FullName));
                backupArchive.AddFile(f.FullName, "");
            }

            backupArchive.Save();
            SCLP.LogProxy.Log(SCLP.LogLevel.Info, "Archive built.");
        }

        static void CreateCreationCache()
        {
            DirectoryInfo transitDirectory =
                new DirectoryInfo(cacheDirectory + @"\Ready for transit");

            creationCacheDirectory.Create();
            transitDirectory.Create();
        }

        static void LinkToCaches(String archiveName)
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              archiveName + " is now being linked to all target caches...");

            foreach (DirectoryInfo cd in
                     cacheDirectory.EnumerateDirectories())
                if (cd.FullName !=
                    creationCacheDirectory.FullName) {
                    NtfsWrapper.CreateLink(archiveName,
                                           String.Format("{0}\\{1}",
                                                         cd.FullName, Path.GetFileName(
                                                                 archiveName)));
                    SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                      archiveName + " has been linked to " +
                                      cd.FullName);
                }
            NtfsWrapper.DirectDeleteFile(archiveName);
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              archiveName + " has been removed from Cache\\Build");
        }

        static public void PerformBackups()
        {
            backupTime = DateTime.Now.ToString("yyyyMMddhhmm");

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Backup requested, executing operations in queue...");

            if (backupArchive != null)
                backupArchive.Dispose();

            ClearCaches();

            foreach (QueuedBackupOperation qbo in
                     backupOperations)
                qbo.Execute(backupTime);

            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              "Queued operations have been processed.");
            CompressBackups();
        }

        static void ZipSaveCompletionHandler(Object sender,
                                             IZ.SaveProgressEventArgs eventArgs)
        {
            IEnumerable<FileInfo> files =
                creationCacheDirectory.EnumerateFiles("*.fbk");
            for (int i = 0; i < 8; i++)
                try {
                    if (eventArgs.EventType ==
                        IZ.ZipProgressEventType.Saving_Completed) {
                        Thread.Sleep(1000);
                        foreach (FileInfo f in files)
                            f.Delete();
                        LinkToCaches(backupArchive.Name);
                        return;
                    }
                } catch (Exception e) {
                    if (i == 7)
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          "Couldn't clear build cache, expect the next backup to be larger than usual.");
                    else {
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          "Build cache clear failed! Waiting 5 seconds for retry...");
                        SCLP.LogProxy.Log(SCLP.LogLevel.Error,
                                          String.Format("Cause reports was: {0}\n", e.Message));
                        Thread.Sleep(5000);
                    }
                }
        }

        static void ServiceOutputHandler(Object sender, FBDS.ServiceOutputEventArgs eventArgs)
        {
        }

        static public FBDS.FbBackup BackupService
        {
            get {
                return backupService;
            }
        }

        static public String CreationCachePath
        {
            get {
                return creationCacheDirectory.FullName;
            }
        }

        static public int FragmentThreshold
        {
            get {
                return fragmentThreshold;
            }
        }

        static public String RestorationPath
        {
            get {
                return restorationPath;
            }
        }

        static public FBDS.FbRestore RestorationService
        {
            get {
                return restorationService;
            }
        }

        static public String RootCachePath
        {
            get {
                return cacheDirectory.FullName;
            }
        }

        static public SCC.Base CrmInstance
        {
            get {
                return crmInstance;
            } set {
                crmInstance = value;
            }
        }

        static IZ.ZipFile backupArchive = null;
        static List<QueuedBackupOperation> backupOperations;
        static FBDS.FbBackup backupService;
        static String backupTime;
        static DirectoryInfo cacheDirectory;
        static DirectoryInfo creationCacheDirectory;
        static SCC.Base crmInstance;
        static int fragmentThreshold = 1 << 30;
        static String restorationPath;
        static FBDS.FbRestore restorationService;
    }

    class QueuedBackupOperation
    {
        void Backup()
        {
            try {
                FBDS.FbBackupFile backupFile;
                long fragments = (database.Length /
                                  SCB.Backup.FragmentThreshold);
                int i = 0;
                long trailingFragmentSize = (database.Length %
                                             SCB.Backup.FragmentThreshold);

                foreach (FC.IFileRepository fr in targets)
                    SCB.Backup.CrmInstance.LogBackupOperation(
                        "Databases are in the process of being backed up.", fr.Name, "In progress", 0);

                SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                  String.Format(
                                      "{0} has {1} fragment(s), split threshold is {2}KiB.",
                                      database.Name,
                                      fragments + (trailingFragmentSize > 0 ? 1 : 0),
                                      SCB.Backup.FragmentThreshold >> 10));

                while (i < fragments) {
                    backupFile = new FBDS.FbBackupFile(
                        CreateBackupFilenamePath(i++),
                        (int)SCB.Backup.FragmentThreshold);
                    SCB.Backup.BackupService.BackupFiles.Add(
                        backupFile);
                }

                if (trailingFragmentSize > 0) {
                    backupFile = new FBDS.FbBackupFile(
                        CreateBackupFilenamePath(i),
                        (int)trailingFragmentSize);
                    SCB.Backup.BackupService.BackupFiles.Add(
                        backupFile);
                }

                SCB.Backup.BackupService.Execute();
            } catch (FBDC.FbException fbe) {
                String message = database.FullName +
                                 " failed during backup!\nFragments processed:\n";
                foreach (FBDS.FbBackupFile f in
                         SCB.Backup.BackupService.BackupFiles)
                    message += String.Format("{0}\n", f.BackupFile);

                foreach (FC.IFileRepository fr in targets)
                    SCB.Backup.CrmInstance.LogBackupOperation(
                        message + fbe.ToString(), fr.Name, "Backup Failure", 0);

                throw new SCBE.BackupFailure(message, fbe);
            }
        }

        String CreateBackupFilenamePath(long currentFragment)
        {
            return SCB.Backup.CreationCachePath + String.Format(
                       "\\{2}-{1}-{0}.fbk", currentFragment, timestamp,
                       Path.GetFileNameWithoutExtension(
                           database.FullName));
        }

        public void Execute(String timestamp)
        {
            SCB.Backup.BackupService.ConnectionString =
                connectionString.ToString();
            this.timestamp = timestamp;
            Backup();
            ValidateBackup();
            Thread.Sleep(256);
        }

        ~QueuedBackupOperation()
        {
        }


        public QueuedBackupOperation(String database,
                                     IEnumerable<FC.IFileRepository> targets)
        {
            SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                              database + " queued for backup.");
            this.database = new FileInfo(database);

            connectionString =
                new FBDC.FbConnectionStringBuilder();
            validationDirectory = new DirectoryInfo(
                SCB.Backup.CreationCachePath);

            connectionString.Charset = "NONE";
            connectionString.Database = database;
            connectionString.DataSource = "localhost";
            connectionString.Password = "masterkey";
            connectionString.UserID = "sysdba";

            this.targets = targets;
        }

        void ValidateBackup()
        {
            try {
                IEnumerable<FBDS.FbBackupFile> files =
                    SCB.Backup.BackupService.BackupFiles;
                FileInfo info;

                SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                  "Ensuring database is valid and restoration of " +
                                  database.Name + " is possible...");

                foreach (FC.IFileRepository fr in targets)
                    SCB.Backup.CrmInstance.LogBackupOperation(
                        "Testing database restoration...", fr.Name, "Restoring", 0);

                foreach (FBDS.FbBackupFile bf in files) {
                    for (info = new FileInfo(bf.BackupFile);
                         !info.Exists; info.Refresh()) {
                        ;
                    }
                    SCB.Backup.RestorationService.BackupFiles.Add(
                        new FBDS.FbBackupFile(bf.BackupFile,
                                              (int)info.Length));
                    Thread.Sleep(256);
                }

                SCB.Backup.BackupService.BackupFiles.Clear();
                SCB.Backup.RestorationService.Execute();
                SCB.Backup.RestorationService.BackupFiles.Clear();

                SCLP.LogProxy.Log(SCLP.LogLevel.Info,
                                  "Validated. Restoration successful.");
                foreach (FC.IFileRepository fr in targets)
                    SCB.Backup.CrmInstance.LogBackupOperation(
                        "Backup has been validated successfully.", fr.Name, "Validated", 0);
            } catch (FBDC.FbException fbe) {
                String message = database.Name + " failed validation!";
                foreach (FBDS.FbBackupFile f in
                         SCB.Backup.BackupService.BackupFiles)
                    message += String.Format("{0}\n", f.BackupFile);
                foreach (FC.IFileRepository fr in targets)
                    SCB.Backup.CrmInstance.LogBackupOperation(
                        message + fbe.ToString(), fr.Name, "Failure", 0);
                throw new SCBE.ValidationRestorationFailure(message, fbe);
            }
        }

        FBDC.FbConnectionStringBuilder connectionString;
        FileInfo database;
        String timestamp;
        IEnumerable<FC.IFileRepository> targets;
        static DirectoryInfo validationDirectory;
    };
};
