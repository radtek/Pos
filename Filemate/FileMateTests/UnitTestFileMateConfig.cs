using Microsoft.VisualStudio.TestTools.UnitTesting;
using FileMate.Core.Xml;
using System.Xml;

namespace FileMateTests {
    /// <summary>
    /// Summary description for UnitTestFileMateConfig
    /// </summary>
    [TestClass]
    public class UnitTestFileMateConfig {
        public UnitTestFileMateConfig() {
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

        [TestMethod]
        public void TestBasicLoad() {
          
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(TestResource.FileMate_Test_Config);

            CopyGroupDTO cgDto = new CopyGroupDTO(doc);

            Assert.AreEqual<int>(1, cgDto.Operations.Count);
            CopyOperationDTO coDto = cgDto.Operations[0];

            Assert.AreEqual<string>("summa", coDto.Name);
            Assert.IsFalse(coDto.CalledFromExternal);
            Assert.IsFalse(coDto.DeleteSource);
            Assert.AreEqual<string>("*.dat", coDto.Pattern);
            Assert.AreEqual<string>("1m", coDto.Schedule);
            Assert.AreEqual<string>(@"c:\source\", coDto.SourceUrl);

            // check targets
            Assert.AreEqual<int>(1, coDto.Targets.Count);
            FileMateTargetDTO tDto = coDto.Targets[0];
            Assert.AreEqual<string>(@"c:\target\", tDto.TargetUrl);
        }


        [TestMethod]
        public void TestBasicSave() {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(TestResource.FileMate_Test_Config);

            CopyGroupDTO cgDto = new CopyGroupDTO(doc);

            XmlDocument testDoc = cgDto.AsXml();

            // assert some things about the produced xml
            XmlNodeList ops = Helper.GetCopyOperationNodes(testDoc);
            Assert.AreEqual<int>(1, ops.Count);

            Assert.AreEqual<string>("summa", Helper.GetCopyOperationName(ops[0]));
            Assert.IsFalse(Helper.GetCalledFromForOperation(ops[0]));
            Assert.IsFalse(Helper.GetCopyOperationDeleteSource(ops[0]));
            Assert.AreEqual<string>(@"c:\source\", Helper.GetSourceUrl(Helper.GetSourceNodeForOperation(ops[0])));
            Assert.AreEqual<int>(1, Helper.GetTargetsForOperation(ops[0]).Count);

            XmlNode targetNode = Helper.GetTargetsForOperation(ops[0])[0];
            Assert.AreEqual<string>(@"c:\target\", Helper.GetTargetUrl(targetNode));
            
            Assert.AreEqual<string>("1m", Helper.GetScheduleForOperation(ops[0]));
            Assert.AreEqual<string>("*.dat", Helper.GetFilePatternForOperation(ops[0]));
        }
    }
}
