//---------------------------------------------------------------------------

#ifndef MMSettingsH
#define MMSettingsH

//---------------------------------------------------------------------------

#include "MM_DBCore.h"
#include "Enum.h"
#include "PointsRulesSet.h"
#include <Graphics.hpp>

class TMMSettings
{
   public:
   TMMSettings();
   virtual ~TMMSettings();

   void Initialise(Database::TDBTransaction &DBTransaction);
   void InitializeMallExportConfig(Database::TDBTransaction &DBTransaction);
};

#endif
