using System;
using System.IO;
using System.Net;
using System.Xml;
using FileMate.Core;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace FileMateTests {
    /// <summary>
    /// Summary description for UnitTestFilesToFtp
    /// </summary>
    [TestClass]
    public class UnitTestFilesToFtp {
        public UnitTestFilesToFtp() {
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

        string[] _files = null;

        [TestInitialize]
        public void Setup() {
            Console.WriteLine("Setup...");

            // check dir structure
            if (!Directory.Exists(SOURCE_FOLDER)) {
                Directory.CreateDirectory(SOURCE_FOLDER);
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

            _files = Directory.GetFiles(SOURCE_FOLDER);
        }

        [TestCleanup]
        public void TearDown() {

            Console.WriteLine("TearDown...");
            Console.WriteLine("Deleting test files");

      

            string[] dirs = Directory.GetDirectories(TARGET_FOLDER, "*.*");
            foreach (string dir in dirs) {
                Directory.Delete(dir, true);
            }

            foreach (string f in _files) {

                // delete files from ftp server
                string fileName = Path.GetFileName(f);
                FtpWebRequest ftpSession =
                    (FtpWebRequest)FtpWebRequest.Create(new Uri("ftp://ftp.menumate.com/tmp/test1/" + fileName));
                ftpSession.UsePassive = false;
                ftpSession.Credentials = new NetworkCredential("menumate", "menumate40");
                ftpSession.KeepAlive = true;
                ftpSession.UseBinary = true;
                ftpSession.Method = WebRequestMethods.Ftp.DeleteFile;
                FtpWebResponse response = (FtpWebResponse)ftpSession.GetResponse();
            }
        }


        [TestMethod]
        public void BasicSummaTest() {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(TestResource.FileMateConfig_FilesToFtp);
            CopyGroup cg = new CopyGroup(doc);
            string[] files = cg.CopyOperations[0].SourceFileRepository.GetFileList();
            cg.ExecuteCopyOperations();

            Assert.AreEqual<int>(0, cg.CopyOperations[0].SourceFileRepository.GetFileList().Length);

            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].Targets[0].GetFileList().Length);
            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].Targets[1].GetFileList().Length);
        }
    }
}
