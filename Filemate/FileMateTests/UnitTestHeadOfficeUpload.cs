using System;
using System.IO;
using System.Net;
using System.Xml;
using FileMate.Core;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace FileMateTests {
    /// <summary>
    /// Summary description for UnitTestHeadOfficeUpload
    /// </summary>
    [TestClass]
    public class UnitTestHeadOfficeUpload {
        public UnitTestHeadOfficeUpload() {
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

        [TestInitialize]
        public void Setup() {
            Console.WriteLine("Setup...");

            // check dir structure
            if (!Directory.Exists(SOURCE_FOLDER)) {
                Directory.CreateDirectory(SOURCE_FOLDER);
            }

            Console.WriteLine("Creating test files");

            string fullPath = null;
            FileStream fs = null;
            byte[] data = null;

            // create food.csv
            fullPath = Path.Combine(SOURCE_FOLDER, "food.csv");
            fs = File.Create(fullPath);
            data = new byte[] { (byte)'1', (byte)'2', (byte)'3', (byte)'4' };
            fs.Write(data, 0, data.Length);
            fs.Flush();
            fs.Close();

            // create napier-food.csv
            fullPath = Path.Combine(SOURCE_FOLDER, "napier-food.csv");
            fs = File.Create(fullPath);
            data = new byte[] { (byte)'1', (byte)'2', (byte)'3', (byte)'4' };
            fs.Write(data, 0, data.Length);
            fs.Flush();
            fs.Close();


        }

        [TestCleanup]
        public void TearDown() {

            Console.WriteLine("TearDown...");
            Console.WriteLine("Deleting test files");




            // delete files from ftp server

            FtpWebRequest ftpSession =
                (FtpWebRequest)FtpWebRequest.Create(new Uri("ftp://ftp.menumate.com/tmp/test1/napier/menus/napier-food.csv"));
            ftpSession.UsePassive = false;
            ftpSession.Credentials = new NetworkCredential("menumate", "menumate40");
            ftpSession.KeepAlive = true;
            ftpSession.UseBinary = true;
            ftpSession.Method = WebRequestMethods.Ftp.DeleteFile;
            FtpWebResponse response = (FtpWebResponse)ftpSession.GetResponse();

            ftpSession =
                (FtpWebRequest)FtpWebRequest.Create(new Uri("ftp://ftp.menumate.com/tmp/test1/site1/menus/food.csv"));
            ftpSession.UsePassive = false;
            ftpSession.Credentials = new NetworkCredential("menumate", "menumate40");
            ftpSession.KeepAlive = true;
            ftpSession.UseBinary = true;
            ftpSession.Method = WebRequestMethods.Ftp.DeleteFile;
            response = (FtpWebResponse)ftpSession.GetResponse();

            ftpSession =
                (FtpWebRequest)FtpWebRequest.Create(new Uri("ftp://ftp.menumate.com/tmp/test1/site2/menus/food.csv"));
            ftpSession.UsePassive = false;
            ftpSession.Credentials = new NetworkCredential("menumate", "menumate40");
            ftpSession.KeepAlive = true;
            ftpSession.UseBinary = true;
            ftpSession.Method = WebRequestMethods.Ftp.DeleteFile;
            response = (FtpWebResponse)ftpSession.GetResponse();


        }

        [TestMethod]
        public void TestHeadOfficeUploadBasic() {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(TestResource.FileMateConfig_HeadOfficeUpload);
            CopyGroup cg = new CopyGroup(doc);
            string[] files = cg.CopyOperations[0].SourceFileRepository.GetFileList();

            cg.ExecuteCopyOperations();

            Assert.AreEqual<int>(1, cg.CopyOperations[0].Targets[0].GetFileList().Length);
            Assert.AreEqual<int>(1, cg.CopyOperations[0].Targets[1].GetFileList().Length);
            Assert.AreEqual<int>(1, cg.CopyOperations[1].Targets[0].GetFileList().Length);

        }
    }
}
