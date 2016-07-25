using System;
using System.IO;
using System.Net;
using System.Text;
using System.Xml;
using FileMate.Core;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace FileMateTests {
    /// <summary>
    /// Summary description for UnitTestFtpToFolder
    /// </summary>
    [TestClass]
    public class UnitTestFtpToFolder {
        public UnitTestFtpToFolder() {
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

        private string SOURCE_FOLDER = "ftp://ftp.menumate.com/tmp/test1/";
        private string FILE_EXTENSION = ".test";

        [TestInitialize]
        public void Setup() {
            Console.WriteLine("Setup...");



            string tempFilePath = Path.GetTempPath();

            Console.WriteLine("Creating test files");

            // create some test files
            for (int i = 0; i < 10; i++) {
                string fullPath = Path.Combine(tempFilePath, "Test" + i.ToString() + FILE_EXTENSION);
                FileStream fs = File.Create(fullPath);

                byte[] data = { (byte)'1', (byte)'2', (byte)'3', (byte)'4' };
                fs.Write(data, 0, data.Length);
                fs.Flush();
                fs.Close();
            }


            string[] files = Directory.GetFiles(tempFilePath, "*" + FILE_EXTENSION, SearchOption.TopDirectoryOnly);

            Assert.IsTrue(10 == files.Length);


            Console.WriteLine("Uploading test files");

            FtpWebRequest ftpSession = null;

            foreach (string f in files) {

                try {

                    string fileName = Path.GetFileName(f);
                    ftpSession = (FtpWebRequest)FtpWebRequest.Create(new Uri(SOURCE_FOLDER + fileName));
                    ftpSession.UsePassive = false;
                    ftpSession.Credentials = new NetworkCredential("menumate", "menumate40");
                    ftpSession.KeepAlive = true;
                    ftpSession.UseBinary = true;

                    // trying to fix an issue with .net
                    // see http://geekswithblogs.net/denis/archive/2005/08/16/50365.aspx
                    ftpSession.KeepAlive = false;
                    

                    byte[] buffer = null;

                    // read whole file into buffer
                    using (Stream sourceFileStream = File.Open(f, FileMode.Open)) {
                        using (StreamReader sourceFileReader = new StreamReader(sourceFileStream)) {
                            buffer = Encoding.UTF8.GetBytes(sourceFileReader.ReadToEnd());
                        }
                    }

                    // upload the file
                    ftpSession.Method = WebRequestMethods.Ftp.UploadFile;

                    // length
                    ftpSession.ContentLength = buffer.Length;

                    Stream targetFtpStream = ftpSession.GetRequestStream();
                    targetFtpStream.Write(buffer, 0, buffer.Length);
                    targetFtpStream.Flush();
                    targetFtpStream.Close();



                    FtpWebResponse response = (FtpWebResponse)ftpSession.GetResponse();
                    Console.WriteLine(string.Format("[{0}]{1}", response.StatusCode.ToString(), response.StatusDescription));
                    response.Close();

                }
                catch (WebException we) {

                    if (we.Response != null) {

                        Console.WriteLine("Status Code : {0}", ((FtpWebResponse)we.Response).StatusCode);
                        Console.WriteLine("Status Description : {0}", ((FtpWebResponse)we.Response).StatusDescription);
                    }
                    Console.WriteLine(we.ToString());
                }
                catch (SystemException se) {
                    Console.WriteLine(se.ToString());
                    throw;
                }
            }
        }

        [TestCleanup]
        public void TearDown() {

            Console.WriteLine("TearDown...");
            Console.WriteLine("Deleting test files");



            string tempFilePath = Path.GetTempPath();

            string[] files = Directory.GetFiles(tempFilePath, "*" + FILE_EXTENSION, SearchOption.TopDirectoryOnly);

            Assert.IsTrue(10 == files.Length);

            foreach (string f in files) {

                try {

                    // delete files from ftp server
                    string fileName = Path.GetFileName(f);
                    FtpWebRequest ftpSession = (FtpWebRequest)FtpWebRequest.Create(new Uri(SOURCE_FOLDER + fileName));
                    ftpSession.UsePassive = false;
                    ftpSession.Credentials = new NetworkCredential("menumate", "menumate40");
                    ftpSession.KeepAlive = true;
                    ftpSession.UseBinary = true;

                    ftpSession.Method = WebRequestMethods.Ftp.DeleteFile;
                    FtpWebResponse response = (FtpWebResponse)ftpSession.GetResponse();
                    Console.WriteLine(string.Format("[{0}]{1}", response.StatusCode.ToString(), response.StatusDescription));

                }
                catch (SystemException se) {
                    Console.WriteLine(se.ToString());
                }

                // delete files from temp folder
                File.Delete(f);
            }
        }


        [TestMethod]
        public void BasicFtpToFolderTest() {

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(TestResource.FileMateConfig_FtpToFolder);
            CopyGroup cg = new CopyGroup(doc);

            string[] files = cg.CopyOperations[0].SourceFileRepository.GetFileList();

            cg.ExecuteCopyOperations();

            Assert.AreEqual<int>(files.Length, cg.CopyOperations[0].Targets[0].GetFileList().Length);
        }
    }
}
