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
        public string FormatReceipt(List<OutputText> textList)
        {
            string value = "";
            foreach (var item in textList)
            {
                if (item.Text.Trim() == "")
                {
                    if (item.EndOfLineFlag == "true")
                    {
                        value += @"\n";
                    }
                }
                else
                {
                    string text = System.Uri.UnescapeDataString(item.Text);
                    var pair = HttpUtility.ParseQueryString(text);
                    value += pair["name"];
                    string s2 = pair["key"];
                    value += " ";
                    value += pair["value"];
                    int i = 0;
                    if (item.EndOfLineFlag == "true")
                        value += "\n";
                }
            }
            return value;
        }
    }
}
