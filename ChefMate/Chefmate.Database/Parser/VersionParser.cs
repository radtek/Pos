using System.Collections.Generic;
using Chefmate.Database.DbModels;

namespace ChefMate.Database.Parser
{
    public abstract class VersionParser
    {
        public abstract void RunParser();
        public bool IsVersionExist(string versionNumber)
        {
            return DbVersion.CheckVersionExist(versionNumber);
        }
        public void UpdateVersionInDatabase(string versionNumber)
        {
            DbVersion.SaveVersion(versionNumber);
        }
        public void CheckAndCreateGenerator(string inGeneratorName)
        {
            if (!DatabaseCore.Instance.CheckGeneratorExist(inGeneratorName))
            {
                string query = "CREATE GENERATOR " + inGeneratorName;
                DatabaseCore.Instance.ExecuteNonQuery(query);
            }
        }
        public void CheckAndCreateColumn(string inTableName, string inColumnName, string query)
        {
            if (!DatabaseCore.Instance.CheckColumnExist(inTableName, inColumnName))
            {
                DatabaseCore.Instance.ExecuteNonQuery(query);
            }
        }
        public void CheckAndCreateTable(string inTableName, string query)
        {
            if (!DatabaseCore.Instance.CheckTableExist(inTableName))
            {
                DatabaseCore.Instance.ExecuteNonQuery(query);
            }
        }
    }
}