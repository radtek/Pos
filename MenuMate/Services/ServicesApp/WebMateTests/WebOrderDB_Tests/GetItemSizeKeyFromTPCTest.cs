﻿using MenumateServices.WebMate.InternalClasses;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace WebMateTests
{
    
    
    /// <summary>
    ///This is a test class for WebOrderDBTest and is intended
    ///to contain all WebOrderDBTest Unit Tests
    ///</summary>
    [TestClass()]
    public class GetItemSizeKeyFromTPCTest
    {

        private static WebOrderDB_Accessor target = new WebOrderDB_Accessor();
        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        [ClassInitialize()]
        public static void MyClassInitialize(TestContext testContext)
        {
            target.BeginTransaction();
        }
        
        //Use ClassCleanup to run code after all tests in a class have run
        [ClassCleanup()]
        public static void MyClassCleanup()
        {
            target.EndTransaction();
        }
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        ///A test for getItemSizeKeyFromThirdPartyCode
        ///</summary>
        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void getItemSizeKeyFromThirdPartyCodeTest_Valid()
        {
            int thirdPartyCode = WebMateTestSettings.ThirdPartyCodeKey;
            int expected = WebMateTestSettings.MatchingItemSizeKey; 
            int actual;
            actual = target.getItemSizeKeyFromThirdPartyCodeKey(thirdPartyCode);
            Assert.AreEqual(expected, actual);
        }

        [TestMethod()]
        [DeploymentItem("MenumateServices.exe")]
        public void getItemSizeKeyFromThirdPartyCodeTest_Invalid()
        {
            int thirdPartyCode = 25;
            int expected = 588246;
            int actual;
            actual = target.getItemSizeKeyFromThirdPartyCodeKey(thirdPartyCode);
            Assert.AreNotEqual(expected, actual);
        }
    }
}
