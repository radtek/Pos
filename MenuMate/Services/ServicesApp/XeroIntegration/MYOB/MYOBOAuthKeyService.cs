using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.IsolatedStorage;
using MYOB.AccountRight.SDK;
using Newtonsoft.Json;
using MYOB.AccountRight.SDK.Contracts;


namespace AccountingIntegration.MYOB
{
    public class MYOBOAuthKeyService : IOAuthKeyService
    {
        private const string CsTokensFile = "Tokens.json";

        private OAuthTokens _tokens;

        /// <summary>
        /// On creation read any settings from file
        /// </summary>
        /// <remarks></remarks>
        public MYOBOAuthKeyService()
        {
            ReadFromFile();
        }

        #region IOAuthKeyService Members

        /// <summary>
        /// Implements the property for OAuthResponse which holdes theTokens
        /// </summary>
        /// <value>object containing OAuthTokens</value>
        /// <returns>Contracts.OAuthTokens</returns>
        /// <remarks>Saves to isolated storage when set</remarks>
        public OAuthTokens OAuthResponse
        {
            get { return _tokens; }
            set
            {
                _tokens = value;
                SaveToFile();
            }
        }

        #endregion

        /// <summary>
        /// Method to read Tokens from Isolated storage
        /// </summary>
        /// <remarks></remarks>
        private void ReadFromFile()
        {
            try
            {
                string path = System.IO.Path.GetDirectoryName(
                System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
                if (path.Contains(@"file:\"))
                {
                    path = path.Replace(@"file:\", "");
                }
                string fileName = Path.Combine(path, CsTokensFile);
                using (StreamReader file = File.OpenText(fileName))
                {
                    JsonSerializer serializer = new JsonSerializer();
                    _tokens = (OAuthTokens)serializer.Deserialize(file, typeof(OAuthTokens));
                }
            }
            catch (FileNotFoundException)
            {
                // Expected exception if a file cannot be found. This indicates that we have a new user.
                ServiceLogger.Log(String.Format("Token File: {0}", CsTokensFile));
                _tokens = null;
            }
        }


        /// <summary>
        /// Method to save tokens to isolated storage
        /// </summary>
        /// <remarks></remarks>
        private void SaveToFile()
        {
            string path = System.IO.Path.GetDirectoryName(
            System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
            if (path.Contains(@"file:\"))
            {
                path = path.Replace(@"file:\", "");
            }
            string fileName = Path.Combine(path, CsTokensFile);
            using (StreamWriter file = File.CreateText(fileName))
            {
                JsonSerializer serializer = new JsonSerializer();
                serializer.Serialize(file, _tokens);
            }
        }
    }
}
