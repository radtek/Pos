#ifndef __h_magic_memories_sf_progress_monitor__
#define __h_magic_memories_sf_progress_monitor__

#include "MagicMemoriesSfService.h"

using SfIntegration::Sf_data_object_type;
using SfIntegration::Sf_notification;
using SfIntegration::Sf_notification_receiver;

class SalesforceProgressMonitor : public Sf_notification_receiver {
public:
   SalesforceProgressMonitor(TfrmProcessing *processing_dialog)
     : processing_dialog(processing_dialog)
   {
   }

   ~SalesforceProgressMonitor() { };

   void push_notification(Sf_notification notification,
                          Sf_data_object_type type);

private:
   SalesforceProgressMonitor(const SalesforceProgressMonitor &initializer);
   SalesforceProgressMonitor &operator=(
     const SalesforceProgressMonitor &prototype);

   TfrmProcessing *processing_dialog;
};

#endif

