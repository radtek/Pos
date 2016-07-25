using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace WebMateTests.WebMateWCFService
{
    [TestClass]
    public class OpenWebOrderTest
    {
        [TestMethod]
        public void OpenOrder()
        {
            using (WCFServiceTestSettings settings = new WCFServiceTestSettings())
            {
                string handle = "";
                settings.Client.OpenWebOrder(out handle);
                Assert.IsTrue(!string.IsNullOrEmpty(handle));
            }
        }
    }
}
