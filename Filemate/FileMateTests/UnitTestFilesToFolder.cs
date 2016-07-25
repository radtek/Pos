using System;
using System.IO;
using System.Text;
using System.Threading;
using System.Xml;
using FileMate.Core;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace FileMateTests {
    /// <summary>
    /// Summary description for UnitTestCore
    /// </summary>
    [TestClass]
    public class UnitTestFilesToFolder {
        public UnitTestFilesToFolder() {
            //
            // TODO: Add constructor logic here
            //
        }

        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext {
            get {
                return testContextInstance;
            }
            set {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        //
        // You can use the following additional attributes as you write your tests:
        //
        // Use ClassInitialize to run code before running the first test in the class
        // [ClassInitialize()]
        // public static void MyClassInitialize(TestContext testContext) { }
        //
        // Use ClassCleanup to run code after all tests in a class have run
        // [ClassCleanup()]
        // public static void MyClassCleanup() { }
        //
        // Use TestInitialize to run code before running each test 
        // [TestInitialize()]
        // public void MyTestInitialize() { }
        //
        // Use TestCleanup to run code after each test has run
        // [TestCleanup()]
        // public void MyTestCleanup() { }
        //
        #endregion

        private string SOURCE_FOLDER = @"c:\source\";
        private string TARGET_FOLDER = @"c:\target\";

        private string FILE_EXTENSION = ".dat";

        [TestInitialize]
        public void Setup() {
            Console.WriteLine("Setup...");

            // check dir structure
            if (!Directory.Exists(SOURCE_FOLDER)) {
                Directory.CreateDirectory(SOURCE_FOLDER);
            }

            if (!Directory.Exists(TARGET_FOLDER)) {
                Directory.CreateDirectory(TARGET_FOLDER);
            }

            Console.WriteLine("Creating test files");

            // create some test files
            for (int i = 0; i < 10; i++) {
                string fullPath = Path.Combine(SOURCE_FOLDER, "Test" + i.ToString() + FILE_EXTENSION);
                FileStream fs = File.Create(fullPath);

                byte[] data = { (byte)'1', (byte)'2', (byte)'3', (byte)'4' };
                fs.Write(data, 0, data.Length);
                fs.Flush();
                fs.Close();
            }



        }

        [TestCleanup]
        public void TearDown() {

            Console.WriteLine("TearDown...");
            Console.WriteLine("Deleting test files");

            // delete test files
            string[] files = Directory.GetFiles(SOURCE_FOLDER, "*" + FILE_EXTENSION);
            foreach (string file in files) {
                File.Delete(file);
            }

            string[] dirs = Directory.GetDirectories(TARGET_FOLDER, "*.*");
            foreach (string dir in dirs) {
                Directory.Delete(dir, true);
            }
        }

        [TestMethod]
        public void BasicFileCopyTest() {

            string pf = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);

            XmlDocument doc = new XmlDocument();

            // delete source is false
            doc.LoadXml(TestResource.TestConfig_FileToFolder);

            CopyGroup cg = new CopyGroup(doc);
            string[] files = cg.CopyOperations[0].SourceFileRepository.GetFileList();
            cg.ExecuteCopyOperations();
            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].Targets[0].GetFileList().Length);

            Thread.Sleep(1000); // wait for a bit

            // do it again should get another target folder based on date, i.e., two folders
            cg.ExecuteCopyOperations();
            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].Targets[0].GetFileList().Length);
            Assert.AreEqual<int>(2, Directory.GetDirectories(Path.Combine(cg.CopyOperations[0].Targets[0].Url, ".."),
                "*.*").Length);

        }

        /// <summary>
        /// Test that the 'every five minutes' functionality of FileMate is correct.
        /// </summary>
        [TestMethod]
        public void TestInterval() {
            XmlDocument doc = new XmlDocument();

            // delete source is false
            doc.LoadXml(TestResource.FileMateConfig_FilesToFolderWithSchedule);
            CopyGroup cg = new CopyGroup(doc);
            string[] files = cg.CopyOperations[0].SourceFileRepository.GetFileList();
            Thread.Sleep(70000);
            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].Targets[0].GetFileList().Length);
        }

        /// <summary>
        /// Test that files are not deleted if the target is invalid/offline.
        /// </summary>
        [TestMethod]
        public void BasicFileCopyOfflineTarget() {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(TestResource.FileMateConfig_FileToFolderOfflineTarget);
            CopyGroup cg = new CopyGroup(doc);
            string[] files = cg.CopyOperations[0].SourceFileRepository.GetFileList();

            cg.ExecuteCopyOperations();

            Assert.AreEqual<int>(0, cg.CopyOperations[0].Targets[0].GetFileList().Length);
            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].SourceFileRepository.GetFileList().Length);
        }

        /// <summary>
        /// Test the old behaviour of a specific time to upload file is working for FileMate.
        /// </summary>
        [TestMethod]
        public void TestSchedule() {
            XmlDocument doc = new XmlDocument();

            doc.LoadXml(TestResource.FileMateTestConfig_scheduleSpecificTime);

            // build a schedule in the future
            DateTime testSchedule = DateTime.Now;
            testSchedule = testSchedule.AddMinutes(2);
            int hr = testSchedule.Hour;
            int min = testSchedule.Minute;
            StringBuilder b = new StringBuilder(hr.ToString());
            b.Append(":");
            b.Append(min.ToString());

            XmlNode scheduleNode = doc.SelectSingleNode("//schedule");
            scheduleNode.InnerText = b.ToString();

            CopyGroup cg = new CopyGroup(doc);
            string[] files = cg.CopyOperations[0].SourceFileRepository.GetFileList();

            // wait
            Thread.Sleep(new TimeSpan(0, 3, 0));
            
            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].Targets[0].GetFileList().Length);

        }
    }
}
