//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "tinyxml.h"
#include "MM_DBCore.h"
#include "Version.h"
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "touchbtn"
#pragma link "touchcontrols"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
   : TForm(Owner)
{
   Log("Version " + GetFileVersion());
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::btnExtractClick(TObject *Sender)
{
   dlgOpenGDB->Execute();
	Database::TDBControl DBControl;
   DBControl.Disconnect();
   DBControl.Init(Database::TDBSettings("localhost",dlgOpenGDB->FileName,false));
   DBControl.Connect();
   Database::TDBUtilities Utils(DBControl);
   Utils.SetForcedWrites();

   Database::TTableNames TableNames;
   Utils.GetTableNames(TableNames);

   TiXmlDocument doc;
   TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
   doc.LinkEndChild( decl );

   TiXmlElement * Tables = new TiXmlElement( "Tables" );
   doc.LinkEndChild( Tables );


   for (std::vector<AnsiString>::iterator ptrString = TableNames.begin();
        ptrString != TableNames.end();
        ptrString++)
   {
      TiXmlElement * xmlTable = new TiXmlElement( "Table" );
      xmlTable->SetAttribute("Name", (*ptrString).c_str());
      TDBTables::LinkEndChild( xmlTable );

      Database::TFieldNames Fields;
      Utils.GetFieldNames(*ptrString, Fields);

      TiXmlElement * xmlFields = new TiXmlElement( "Fields" );
      xmlTable->LinkEndChild( xmlFields );

      for (Database::TFieldNames::iterator ptrStrFields = Fields.begin();
           ptrStrFields != Fields.end();
           ptrStrFields++)
      {
         TiXmlElement * xmlField = new TiXmlElement( "Field" );
         xmlField->SetAttribute("Name", (*ptrStrFields).c_str());

         Database::TFieldDefinition FieldDefinition;
         Utils.GetFieldDefinition(*ptrString, *ptrStrFields, FieldDefinition);

         xmlField->SetAttribute("Type", static_cast<int>(FieldDefinition.FieldType));
         xmlField->SetAttribute("Scale", static_cast<int>(FieldDefinition.Scale));
         xmlField->SetAttribute("Length", static_cast<int>(FieldDefinition.Length));
         xmlField->SetAttribute("Precision", static_cast<int>(FieldDefinition.Precision));

         xmlFields->LinkEndChild( xmlField );
      }

      Database::TIndexNames Indexes;
      Utils.GetIndexNames(*ptrString, Indexes);

      TiXmlElement * xmlIndexes = new TiXmlElement( "Indexes" );
      xmlTable->LinkEndChild( xmlIndexes );

      for (Database::TIndexNames::iterator ptrIndex = Indexes.begin();
           ptrIndex != Indexes.end();
           ptrIndex++)
      {
         Database::TIndexDefinition IndexDefinition;
         Utils.GetIndexDefinition(*ptrIndex, IndexDefinition);

         TiXmlElement * xmlIndex = new TiXmlElement( "Index" );
         xmlIndex->SetAttribute("Name", (*ptrIndex).c_str());
         xmlIndex->SetAttribute("TableName", IndexDefinition.TableName.c_str());
         xmlIndex->SetAttribute("Unique", IndexDefinition.Unique);
         xmlIndex->SetAttribute("ForeignKey", IndexDefinition.ForeignKey.c_str());
         xmlIndex->SetAttribute("ForeignTable", IndexDefinition.ForeignTable.c_str());
         xmlIndexes->LinkEndChild( xmlIndex );

         TiXmlElement * xmlIndexFields = new TiXmlElement( "IndexFields" );
         xmlIndex->LinkEndChild( xmlIndexFields );

         Database::TIndexFields IndexFields;
         Utils.GetIndexFields(*ptrIndex, IndexFields);

         for (Database::TIndexFields::iterator ptrStrIndexFields = IndexFields.begin();
              ptrStrIndexFields != IndexFields.end();
              ptrStrIndexFields++)
         {
            TiXmlElement * xmlIndexField = new TiXmlElement( "Field" );
            xmlIndexField->SetAttribute("Name", (*ptrStrIndexFields).c_str());
            xmlIndexFields->LinkEndChild( xmlIndexField );
         }
      }
   }
   AnsiString SaveAs = ChangeFileExt(dlgOpenGDB->FileName, ".gdb.xml");
   DeleteFile(SaveAs);
   doc.SaveFile( SaveAs.c_str() );
   Log("Extraction to " + SaveAs + " Complete.");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnCheckClick(TObject *Sender)
{
   dlgOpenXML->Execute();
   TiXmlDocument doc(dlgOpenXML->FileName.c_str());
	if (doc.LoadFile())
	{
      TiXmlHandle hDoc(&doc);

      dlgOpenGDB->Execute();
      Database::TDBControl DBControl;
      DBControl.Disconnect();
      DBControl.Init(Database::TDBSettings("localhost",dlgOpenGDB->FileName,false));
      DBControl.Connect();
      Database::TDBUtilities Utils(DBControl);
      Utils.SetForcedWrites();

      Database::TTableNames TableNames;
      Utils.GetTableNames(TableNames);

      TiXmlElement* pElement;
      TiXmlHandle pTableSet(0);
      TiXmlHandle pTopLevel(0);
      pElement = hDoc.FirstChildElement().Element();
      if (pElement)
      {
         // Save off the Curernt Root Handle.
         pTopLevel = TiXmlHandle(pElement);

         // Save off the Curernt Button Set.
         pTableSet = TiXmlHandle(pElement);

         TiXmlElement* pTableNode = pTableSet.FirstChild( "Table" ).Element();
         for(; pTableNode; pTableNode = pTableNode->NextSiblingElement())
         {
            AnsiString TableName = pTableNode->Attribute("Name");
            Database::TTableNames::iterator Search = std::find(TableNames.begin(),TableNames.end(),TableName);
            if(Search != TableNames.end())
            {
               // Check this tables Fields
               Database::TFieldNames Fields;
               Utils.GetFieldNames(TableName, Fields);

               TiXmlElement* pFieldsNode = TiXmlHandle(pTableNode).FirstChild( "Fields" ).Element();
               if(pFieldsNode)
               {
                  TiXmlElement* pFieldNode = TiXmlHandle(pFieldsNode).FirstChild( "Field" ).Element();
                  for(; pFieldNode; pFieldNode = pFieldNode->NextSiblingElement())
                  {
                     AnsiString FieldName = pFieldNode->Attribute("Name");
                     Database::TFieldNames::iterator Search = std::find(Fields.begin(),Fields.end(),FieldName);
                     if(Search != Fields.end())
                     {
                        Database::TFieldDefinition FieldDefinition;
                        Utils.GetFieldDefinition(TableName, FieldName, FieldDefinition);
                        int Type = -1;
                        pFieldNode->QueryIntAttribute("Type",&Type);
                        int Scale = -1;
                        pFieldNode->Attribute("Scale",&Scale);
                        int Length = -1;
                        pFieldNode->Attribute("Length",&Length);
                        int Precision = -1;
                        pFieldNode->Attribute("Precision",&Precision);

                        if(Type != static_cast<int>(FieldDefinition.FieldType))
                        {
                           Log("Field " + FieldName + " Table " + TableName + " Type mismatch Xml " + IntToStr(Type) + " GDB " + IntToStr(static_cast<int>(FieldDefinition.FieldType)));
                        }
                        if(Scale != static_cast<int>(FieldDefinition.Scale))
                        {
                           Log("Field " + FieldName + " Table " + TableName + " Scale mismatch Xml " + IntToStr(Scale) + " GDB " + IntToStr(static_cast<int>(FieldDefinition.Scale)));
                        }
                        if(Length != static_cast<int>(FieldDefinition.Length))
                        {
                           Log("Field " + FieldName + " Table " + TableName + " Length mismatch Xml " + IntToStr(Length) + " GDB " + IntToStr(static_cast<int>(FieldDefinition.Length)));
                        }
                        if(Precision != static_cast<int>(FieldDefinition.Precision))
                        {
                           Log("Field " + FieldName + " Table " + TableName + " Precision mismatch Xml " + IntToStr(Precision) + " GDB " + IntToStr(static_cast<int>(FieldDefinition.Precision)));
                        }
                     }
                     else
                     {
                        Log("Missing Field " + FieldName + " in Table " + TableName + " in GDB.");
                     }
                  }
               }
               else
               {
                  Log("No Fields for table " + TableName + " found in XML blueprint.");
               }

               // Check this tables Fields
               Database::TIndexNames Indexes;
               Utils.GetIndexNames(TableName, Indexes);

               TiXmlElement* pIndexesNode = TiXmlHandle(pTableNode).FirstChild( "Indexes" ).Element();
               if(pIndexesNode)
               {
                  TiXmlElement* pIndexNode = TiXmlHandle(pIndexesNode).FirstChild( "Index" ).Element();
                  for(; pIndexNode; pIndexNode = pIndexNode->NextSiblingElement())
                  {
                     AnsiString IndexName = pIndexNode->Attribute("Name");
                     Database::TIndexNames::iterator Search = std::find(Indexes.begin(),Indexes.end(),IndexName);
                     if(Search == Indexes.end())
                     {
                        // Cannot find the indexes by Name have to do a more in-deapth check.

                        Database::TIndexDefinition XMLIndexDefinition;
                        Database::TIndexFields XMLIndexFields;
                        XMLIndexDefinition.TableName     = pIndexNode->Attribute("TableName");
                        XMLIndexDefinition.Unique        = pIndexNode->Attribute("Unique");
                        XMLIndexDefinition.ForeignKey    = pIndexNode->Attribute("ForeignKey");
                        XMLIndexDefinition.ForeignTable  = pIndexNode->Attribute("ForeignTable");
                        // Load up the XMLIndexesFields.
                        TiXmlElement* pFieldIndexesNode = TiXmlHandle(pIndexNode).FirstChild( "IndexFields" ).Element();
                        if(pIndexesNode)
                        {
                           TiXmlElement* pIndexNode = TiXmlHandle(pFieldIndexesNode).FirstChild( "Field" ).Element();
                           for(; pIndexNode; pIndexNode = pIndexNode->NextSiblingElement())
                           {
                              XMLIndexFields.insert(pIndexNode->Attribute("Name"));
                           }
                        }

                        // Loop though all the indexes for this table and see if theres a match.
                        bool MatchFound = false;
                        for (Search = Indexes.begin(); Search != Indexes.end() && !MatchFound; Search++)
                        {
                           Database::TIndexDefinition IndexDefinition;
                           Database::TIndexFields IndexFields;
                           Utils.GetIndexDefinition(*Search, IndexDefinition);
                           Utils.GetIndexFields(*Search, IndexFields);
                           
/*                         Log("GDB Index Fields ...." + AnsiString(*Search));
                           Log("Found Index "         + *Search + " in Table " + TableName + " in GDB.");
                           Log("Index Table Name "    + IndexDefinition.TableName);
                           Log("Index Unique "        + AnsiString(IndexDefinition.Unique ? "True":"False"));
                           Log("Index ForeignKey "    + IndexDefinition.ForeignKey);
                           Log("Index ForeignTable "  + IndexDefinition.ForeignTable);
                           Log("GDB Index Fields ...." );

                           Database::TIndexFields::iterator ListIndexFields = IndexFields.begin();
                           for (; ListIndexFields != IndexFields.end();ListIndexFields++)
                           {
                              Log(*ListIndexFields);
                           }*/

                           bool CompareIndexesForUniqueness = cbCompareUniqueIndex->Checked;
                           if( XMLIndexDefinition.TableName == IndexDefinition.TableName &&
                               ((!CompareIndexesForUniqueness) || (XMLIndexDefinition.Unique == IndexDefinition.Unique)) &&
                               XMLIndexDefinition.ForeignTable == IndexDefinition.ForeignTable &&
//                               XMLIndexDefinition.ForeignKey == IndexDefinition.ForeignKey &&
                               equal(XMLIndexFields.begin(),XMLIndexFields.end(),IndexFields.begin())
                             )
                           {
                              MatchFound = true;
                           }
                        }

                        if(!MatchFound)
                        {
                           Log("Missing Index "       + IndexName + " in Table " + TableName + " in GDB.");
                           Log(XMLIndexDefinition.TableName + " -> "  + XMLIndexDefinition.ForeignTable);
                           Log("Index Unique "        + AnsiString(XMLIndexDefinition.Unique ? "True":"False"));
                           //Log("Index ForeignKey "    + XMLIndexDefinition.ForeignKey);
                           AnsiString Data = "XML Index Fields ....";
                           Database::TIndexFields::iterator ListIndexFields = XMLIndexFields.begin();
                           for (; ListIndexFields != XMLIndexFields.end();ListIndexFields++)
                           {
                              Data += " " + *ListIndexFields;
                           }
                           Log(Data);
                        }
                     }
                  }
               }
               else
               {
                  Log("No Indexes for table " + TableName + " found in XML blueprint.");
               }
            }
            else
            {
               Log("Table " + TableName + " not found in GDB.");
            }
         }
      }
      else
      {
         Log("XML Error no root node.");
      }

	}
	else
	{
      Log("Unable to Open File");
	}
   Log("Comparsion of " + dlgOpenGDB->FileName + " to Template " + dlgOpenXML->FileName + " Complete");
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::Log(AnsiString LogInfo)
{
   memTManagerLogs::Instance().Lines->Add(LogInfo);
}

void __fastcall TfrmMain::TouchBtn1MouseClick(TObject *Sender)
{
   Close();   
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tbtnClearMouseClick(TObject *Sender)
{
   memTManagerLogs::Instance().Lines->Clear();
}
//---------------------------------------------------------------------------

