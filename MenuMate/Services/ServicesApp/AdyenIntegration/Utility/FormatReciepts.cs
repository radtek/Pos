using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AdyenIntegration.Domain;
using System.Web;

namespace AdyenIntegration.Utility
{
    public class FormatReciepts
    {
        //public string[] FormatReceipt(List<OutputText> textList)
        //{
        //    string[] value = textList.Count();
        //    int index = 0;
        //    foreach (var item in textList)
        //    {
        //        if (item.Text.Trim() == "")
        //        {
        //            if (item.EndOfLineFlag == "true")
        //            {
        //                value[index] = @"\n";
        //            }
        //        }
        //        else
        //        {
        //            string text = System.Uri.UnescapeDataString(item.Text);
        //            var pair = HttpUtility.ParseQueryString(text);
        //            value += pair["name"];
        //            string s2 = pair["key"];
        //            value += " ";
        //            value += pair["value"];
        //            int i = 0;
        //            if (item.EndOfLineFlag == "true")
        //                value += "\n";
        //        }

        //    }
        //    return value;
        //}
        public List<string> FormatReceipt(List<OutputText> textList)
        {
            List<string> stringList = new List<string>();
            foreach (var item in textList)
            {
                if (item.Text.Trim() == "")
                {
                    if (item.EndOfLineFlag == "true")
                    {
                        stringList.Add(@"\n");
                    }
                }
                else
                {
                    string text = System.Uri.UnescapeDataString(item.Text);
                    string value = "";
                    var pair = HttpUtility.ParseQueryString(text);
                    value += pair["name"];
                    string s2 = pair["key"];
                    value += " ";
                    value += pair["value"];

                    if (value.Trim() != "") 
                        stringList.Add(value);

                    /*bool isNewline = true;
                    if (text.Trim() != "")
                    {
                        string data = text.ToUpper();
                        isNewline = data.Contains("NAME");
                    }*/
                    //if (item.EndOfLineFlag == "true" && isNewline)
                    //   stringList.Add("\n");
                }

            }
            stringList.Add("\n");
            return stringList;
        }
    }
}
