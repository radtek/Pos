#ifndef MenuManagerItemSizeSelectionH
#define MenuManagerItemSizeSelectionH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>

#include <map>
#include <key_types.hh>

namespace item_management {

class i_item_definition;
class i_item_manager;

class TSizeProperties {
public:
   TSizeProperties();
   TSizeProperties(const UnicodeString &sizeName,
                   size_key_type sizeID,
                   double price,
                   double maxRetailPrice,
                   double specialPrice,
                   double availableQuantity,
                   double defaultQuantity,
                   double warningQuantity,
                   bool enabledState);
   TSizeProperties(const TSizeProperties &initializer);
   ~TSizeProperties();

   TSizeProperties &operator=(const TSizeProperties &rhs);

   size_key_type GetSizeID() const;

   double GetPrice() const;
   double GetMaxRetailPrice() const;
   double GetSpecialPrice() const;
   double GetAvailableQuantity() const;
   double GetDefaultQuantity() const;
   double GetWarningQuantity() const;

   bool GetEnabledState() const;

   bool HavePropertiesChanged() const;

   double SetPrice(double newPrice);
   double SetMaxRetailPrice(double newPrice);
   double SetSpecialPrice(double newSpecialPrice);
   double SetAvailableQuantity(double newAvailableQuantity);
   double SetDefaultQuantity(double newDefaultQuantity);
   double SetWarningQuantity(double newWarningQuantity);
   bool SetEnabledState(bool newEnabledState);

   void RevertToOriginalState();

private:
   double newAvailableQuantity_;
   double newDefaultQuantity_;
   bool   newEnabledState_;
   double newPrice_;
   double newMaxRetailPrice_;
   double newSpecialPrice_;
   double newWarningQuantity_;

   double originalAvailableQuantity_;
   double originalDefaultQuantity_;
   bool   originalEnabledState_;
   double originalPrice_;
   double originalMaxRetailPrice_;
   double originalSpecialPrice_;
   double originalWarningQuantity_;

   unsigned int numChangedProperties_;

   size_key_type sizeID_;
   UnicodeString sizeName_;
};

typedef std::map<TGridButton *, TSizeProperties> TSizePropertiesMap;

class TfrmItemSizeSelection : public TForm
{
__published:
   TGroupBox *gbSizeSelection;
   TTouchGrid *tgridAvailableSizes;
    TTouchBtn *tbtnOk;
    TTouchBtn *tbtnCancel;
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *pnlItemName;
    TCheckBox *cbItemAvailable;
    TGroupBox *gbItemSettings;
   void __fastcall saveChangesClicked(TObject *Sender);
   void __fastcall cancelClicked(TObject *Sender);
   void __fastcall sizeSelectionGridClicked(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton);

private:
   TfrmItemSizeSelection(const TfrmItemSizeSelection &initializer);
   TfrmItemSizeSelection &operator=(const TfrmItemSizeSelection &rhs);

   void initializeForm();
   void initializeSizeSelectionGrid(unsigned int n_rows);
   void populateSizeSelectionData();
   void saveItemAvailability();

   const item_key_type itemID_;
   const UnicodeString itemName_;

   TSizePropertiesMap properties_;

   i_item_manager &itemManager_;
   i_item_definition &itemDefinition_;

public:
   __fastcall TfrmItemSizeSelection(TComponent *owner, const UnicodeString &itemName, item_key_type itemID, bool itemAvailable);
};

} /* item_management */

#endif /* ! MenuManagerItemSizeSelectionH */

