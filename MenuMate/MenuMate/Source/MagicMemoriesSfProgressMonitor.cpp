#pragma hdrstop

#include "MagicMemoriesSfProgressMonitor.h"

#pragma package(smart_init)

void SalesforceProgressMonitor::push_notification(
  Sf_notification notification,
  Sf_data_object_type type)
{
   switch (notification) {
   case Sf_notification::AUTHENTICATING:
      processing_dialog->Message = "Authenticating with Salesforce ...";
      break;
   case Sf_notification::AUTHENTICATION_SUCCESS:
      processing_dialog->Message = "Authenticated with Salesforce.";
      break;
   case Sf_notification::UPLOADING:
      processing_dialog->Message =
        "Uploading " + SfIntegration::Sf_data_object_types[type] + "...";
      break;
   case Sf_notification::UPLOAD_SUCCESS:
      processing_dialog->Message = SfIntegration::Sf_data_object_types[type]
                                   + " uploaded successfully.";
      break;
   default:
      assert(0);
   }

   processing_dialog->Refresh();
};

