using System;
using System.Collections.Generic;
using System.Xml;
using FileMate.Core.Xml;

namespace FileMate.Core
{


    public static class RepositoryBuilder
    {

        public static List<IFileRepository> BuildTargetRepositories(XmlNode opNode)
        {

            List<IFileRepository> targets = new List<IFileRepository>();
            XmlNodeList targetNodeList = Helper.GetTargetsForOperation(opNode);

            foreach (XmlNode targetNode in targetNodeList)
            {

                IFileRepository t = null;

                // extract credential information
                XmlNode credentialsNode = Helper.GetTargetCredentialsNode(targetNode);

                string name = Helper.GetTargetName(targetNode);
                string username = null;
                string password = null;
                Helper.GetTargetUsernameAndPassword(credentialsNode, out username, out password);

                string fullPattern = Helper.GetFilePatternForOperation(opNode);

                // query url and create target repository based on url type
                string rawUrl = Helper.GetTargetUrl(targetNode);

                RepositoryType type = Helper.GetRepositoryType(rawUrl);

                OperationFileType operationFileType = Helper.GetFileTypeForOperation(opNode);

                switch (type)
                {

                    case RepositoryType.Disk:
                        t = new DiskFileRepository(name, rawUrl, username, password, fullPattern, operationFileType, Helper.GetAllowOverwriteValue(targetNode));
                        break;

                    case RepositoryType.Ftp:
                        t = new FtpFileRepository(name, rawUrl, username, password, fullPattern, operationFileType, Helper.GetAllowOverwriteValue(targetNode));
                        break;

                    case RepositoryType.None:
                    default:
                        throw new ArgumentException(string.Format("Unknown repository url: {0}", Helper.GetTargetUrl(targetNode)));
                }
                targets.Add(t);
            }
            return targets;
        }

        public static IFileRepository BuildSourceRepository(XmlNode opNode)
        {

            IFileRepository sourceRepository = null;
            XmlNode sourceNode = Helper.GetSourceNodeForOperation(opNode);

            string username = null;
            string password = null;
            Helper.GetSourceUsernameAndPassword(opNode, out username, out password);

            string fullPattern = Helper.GetFilePatternForOperation(opNode);
            OperationFileType operationFileType = Helper.GetFileTypeForOperation(opNode);

            string rawURL = Helper.GetSourceUrl(sourceNode);

            RepositoryType type = Helper.GetRepositoryType(rawURL);
            switch (type)
            {

                case RepositoryType.Disk:
                    sourceRepository = new DiskFileRepository(null, Helper.GetSourceUrl(sourceNode), username, password, fullPattern, operationFileType);
                    break;

                case RepositoryType.Ftp:
                    sourceRepository = new FtpFileRepository(null, Helper.GetSourceUrl(sourceNode), username, password, fullPattern, operationFileType);
                    break;

                case RepositoryType.None:
                default:
                    throw new ArgumentException(string.Format("Unknown repository url: {0}",
                        Helper.GetSourceUrl(sourceNode)));
            }
            return sourceRepository;
        }

        #region Private methods


        #endregion


    }
}
