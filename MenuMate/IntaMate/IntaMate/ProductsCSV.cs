using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;

namespace IntaMate {
    public class ProductsCSV : CsvMulti {

        public ProductsCSV(string basePath) {
            FULLFILEPATH = basePath + "Products.txt";
            fieldLengths = new int[] { 10, 100, 100, 10, 10, 10, 10, 10, 10 };
            shouldQuoteField = new bool[] { false, true, false, false, false, false, false, false, false };
            csvData = new List<List<string>>();
            csvLineData = new List<StringBuilder>();
        }

        public override void SetupData(XmlDocument doc) {
            List<string> productIds = new List<string>();

            XmlNodeList menus = null;
            try {
                menus = doc.DocumentElement.SelectNodes("//Menu");

                int lineCounter = 0;
                foreach (XmlNode menuNode in menus) {
                    XmlNodeList courses = null;

                    courses = menuNode.SelectNodes("./Courses/Course");


                    foreach (XmlNode courseNode in courses) {
                        XmlNodeList items = null;

                        items = courseNode.SelectNodes("./Items/Item");


                        foreach (XmlNode itemNode in items) {

                            XmlNodeList sizes = null;

                            sizes = itemNode.SelectNodes("./Sizes/Size");


                            foreach (XmlNode sizeNode in sizes) {
                                csvData.Add(new List<string>());

                                string productId = SummaProductsListCommon.GetProductId(menuNode, courseNode, itemNode, sizeNode);

                                if (productIds.Contains(productId)) {
                                    Summa.logger.Debug("Duplicate Product Id found: {0}, {1] ",
                                        productId, 
                                        SummaProductsListCommon.GetProductName(menuNode, courseNode, itemNode, sizeNode));
                                }
                                else {
                                    productIds.Add(productId);

                                    csvData[lineCounter].Add(productId);
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetProductName(menuNode, courseNode, itemNode, sizeNode));
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetProductBarcode(itemNode));
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetProductPrice1(sizeNode));
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetProductPrice2(sizeNode));
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetProductGroupId(sizeNode));
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetProductDepartmentId(sizeNode));
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetProductCost(sizeNode));
                                    csvData[lineCounter].Add(SummaProductsListCommon.GetStoreId(doc));

                                    lineCounter++;
                                    csvLineData.Add(new StringBuilder());
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception e) {
                SummaException se = new SummaException("Problem processing products", e);
                se.Data = doc.OuterXml;
                throw se;
            }
        }
    }
}
