using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MYOB.AccountRight.SDK;
using System.Windows.Forms;
using System.Drawing;
using System.Text.RegularExpressions;
using System.Web;
using System.Net;


namespace AccountingIntegration
{
    class OAuthLogin
    {
        private const string CsOAuthServer = "https://secure.myob.com/oauth2/account/authorize/";
        private const string CsOAuthScope = "CompanyFile";

        /// <summary>
        /// Function to return the OAuth code
        /// </summary>
        /// <param name="config">Contains the API configuration such as ClientId and Redirect URL</param>
        /// <returns>OAuth code</returns>
        /// <remarks></remarks>
        public static string GetAuthorizationCode(IApiConfiguration config)
        {
            //Format the URL so  User can login to OAuth server
            ServicePointManager.ServerCertificateValidationCallback += (sender, certificate, chain, sslPolicyErrors) => { return true; };
            string url = string.Format("{0}?client_id={1}&redirect_uri={2}&scope={3}&response_type=code", CsOAuthServer,
                                       config.ClientId, HttpUtility.UrlEncode(config.RedirectUrl), CsOAuthScope);
            // Create a new form with a web browser to display OAuth login page
            var frm = new Form();
            var webB = new WebBrowser();
            frm.Controls.Add(webB);
            webB.Dock = DockStyle.Fill;
            webB.ScriptErrorsSuppressed = true;
            // Add a handler for the web browser to capture content change 
            webB.DocumentTitleChanged += WebBDocumentTitleChanged;
            // navigat to url and display form
            webB.Navigate(url);
            frm.Size = new Size(800, 600);
            frm.ShowDialog();

            //Retrieve the code from the returned HTML
            return ExtractSubstring(webB.DocumentText, "code=", "<");
        }

        /// <summary>
        /// Handler that is called when HTML title is changed in browser (i.e. content is reloaded)
        /// Once user has signed in to OAth page and authorised this app the OAuth code is returned in the HTML content 
        /// </summary>
        /// <param name="sender">The web browser control</param>
        /// <param name="e">The event</param>
        /// <remarks>This assumes redirect URL is http://desktop</remarks>
        private static void WebBDocumentTitleChanged(object sender, EventArgs e)
        {
            var webB = (WebBrowser)sender;
            var frm = (Form)webB.Parent;

            //Check if OAuth code is returned

            if (webB.DocumentText.Contains("code="))
            {
                frm.Close();
            }
        }

        /// <summary>
        /// Function to retrieve content from a string based on begining and ending pattern
        /// </summary>
        /// <param name="input">input string</param>
        /// <param name="startsWith">start pattern</param>
        /// <param name="endsWith">end pattern</param>
        /// <returns></returns>
        /// <remarks></remarks>
        private static string ExtractSubstring(string input, string startsWith, string endsWith)
        {
            Match match = Regex.Match(input, startsWith + "(.*)" + endsWith);
            string code = match.Groups[1].Value;
            return code;
        }
    }
}
