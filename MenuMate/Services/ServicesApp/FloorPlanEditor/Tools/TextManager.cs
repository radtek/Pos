using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Xml;
using XMLManager;

namespace FloorPlanIntegration
{
    public enum TextManagerLanguages
    {
        lgDefault, lgEnglish
    };

    class TextManager
    {
        TextManagerLanguages _language;

        string _textsFileName = @"Texts";
        XmlDocument _textsFileDoc = new XmlDocument();
        bool _textsLoaded;
        XmlNode _languageNode;

        /// <summary>
        /// 
        /// </summary>
        public TextManager()
        {
            _textsLoaded = pInit();
            Language = TextManagerLanguages.lgDefault;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLanguage"></param>
        public TextManager(TextManagerLanguages inLanguage)
        {
            _textsLoaded = pInit();
            Language = inLanguage;
        }

        /// <summary>
        /// 
        /// </summary>
        public static TextManager Instance
        {
            get
            {
                return Nested.instance;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        class Nested
        {
            // Explicit static constructor to tell C# compiler
            // not to mark type as beforefieldinit
            static Nested()
            {
            }

            internal static readonly TextManager instance = new TextManager();
        }

        #region Properties

        /// <summary>
        /// 
        /// </summary>
        public TextManagerLanguages Language
        {
            get
            {
                return _language;
            }
            set
            {
                pSetLanguage(value);
            }
        }

        #endregion

        #region Public

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string TitleText()
        {
            return XML_ReadText(@"Title");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string SaveEditorText()
        {
            return XML_ReadText(@"SaveEditor");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string SubmitLocationAttrText()
        {
            return XML_ReadText(@"SubmitLocationAttr");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public string SubmitTableAttrText()
        {
            return XML_ReadText(@"SubmitTableAttr");
        }

        #endregion

        #region Private

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool pInit()
        {
            try
            {
                _textsFileName = Helper.GetFileURI(FloorPlanEditor.Properties.Settings.Default.TextsFilename);

                return File.Exists(_textsFileName) ? pLoadTextsFile(_textsFileName) : false;
            }
            catch (Exception)
            {
                return false;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inFileName"></param>
        /// <returns></returns>
        private bool pLoadTextsFile(string inFileName)
        {
            XmlTextReader reader = new XmlTextReader(inFileName);

            try
            {
                reader.Read();
                _textsFileDoc.Load(reader);
                reader.Close();
            }
            catch (Exception)
            {
                reader.Close();
                return false;
            }

            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLanguage"></param>
        private void pSetLanguage(TextManagerLanguages inLanguage)
        {
            _language = (inLanguage == TextManagerLanguages.lgDefault) ? XML_ReadDefaultLanguage() : inLanguage;
            _languageNode = XML_LanguageNode(_language);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLanguage"></param>
        /// <returns></returns>
        private string pLanguageName(TextManagerLanguages inLanguage)
        {
            switch(inLanguage)
            {
                case TextManagerLanguages.lgEnglish: return @"english";
                default: return @"english";
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inLanguageName"></param>
        /// <returns></returns>
        private TextManagerLanguages pStrToLanguage(string inLanguageName)
        {
            switch (inLanguageName.ToUpper())
            {
                case @"ENGLISH": return TextManagerLanguages.lgEnglish;

                default: return TextManagerLanguages.lgEnglish;
            }
        }

        #endregion


        #region XML

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private TextManagerLanguages XML_ReadDefaultLanguage()
        {
            TextManagerLanguages result;

            //::::::::::::::::::::::::::::::::

            if (_textsLoaded)
            {
                string languageStr = XMLDocManager.GetAttribute(XMLDocManager.GetRoot(_textsFileDoc), @"defaultLanguage", @"english");

                result = pStrToLanguage(languageStr);
            }
            else
            {
                result = TextManagerLanguages.lgEnglish;
            }

            //::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNodeName"></param>
        /// <returns></returns>
        private string XML_ReadText(string inNodeName)
        {
            return _textsLoaded ? XMLDocManager.GetAttribute(XML_ChildNode(_languageNode, inNodeName), @"text", @"") : @"";
        }

        /// <summary>
        /// Returns null if the language is not found in the xml file
        /// </summary>
        /// <param name="inLanguage"></param>
        /// <returns></returns>
        private XmlNode XML_LanguageNode(TextManagerLanguages inLanguage)
        {
            XmlNode result = null;

            //:::::::::::::::::::::::::::::::

            if (_textsLoaded)
            {
                XmlNodeList languageNodes = XMLDocManager.GetNodeList(_textsFileDoc, @"Languages/Language");

                foreach (XmlNode node in languageNodes)
                {
                    if (node.Attributes[@"name"].Value == pLanguageName(inLanguage))
                    {
                        result = node;
                        break;
                    }
                }
            }

            //::::::::::::::::::::::::::::::::

            return result;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="inNode"></param>
        /// <param name="inChildNodeName"></param>
        /// <returns></returns>
        private XmlNode XML_ChildNode(XmlNode inNode, string inChildNodeName)
        {
            XmlNode result = null;

            //:::::::::::::::::::::::::::::::

            if (_textsLoaded)
            {
                foreach (XmlNode node in inNode.ChildNodes)
                {
                    if (node.Name == inChildNodeName)
                    {
                        result = node;
                        break;
                    }
                }
            }

            //::::::::::::::::::::::::::::::::

            return result;
        }

        #endregion
    }
}
