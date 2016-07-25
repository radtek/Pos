#include <vcl.h>
#pragma hdrstop

#include "MenuManagerItemSizeSelection.h"
#include "MenuManagerItemSettings.h"
#include "SelectDish.h"
#include <memory>
#include <cmath>

#include <i_item_definition.hh>
#include <i_item_manager.hh>
#include <i_size_definition.hh>
#include <i_size_definition_factory.hh>
#include <item_management_booter.hh>

#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"

namespace item_management {

typedef std::map<size_key_type, const i_size_definition *> TSizeDefinitionMap;

template<typename value_type>
value_type
CompareAndUpdate(
  value_type &newValue,
  value_type &updateValue,
  value_type &originalValue,
  unsigned int &propertiesChangedCount)
{
   if (newValue == updateValue)
      return updateValue;

   propertiesChangedCount +=
     (newValue = updateValue) == originalValue ? -1 : +1;

   return newValue;
}

TSizeProperties::TSizeProperties()
{
}

TSizeProperties::TSizeProperties(
  const UnicodeString &sizeName,
  size_key_type sizeID,
  double price,
  double maxRetailPrice,
  double specialPrice,
  double availableQuantity,
  double defaultQuantity,
  double warningQuantity,
  bool enabledState)
  : newAvailableQuantity_(availableQuantity),
    newDefaultQuantity_(defaultQuantity),
    newEnabledState_(enabledState),
    newPrice_(price),
    newMaxRetailPrice_(maxRetailPrice),
    newSpecialPrice_(specialPrice),
    newWarningQuantity_(warningQuantity),
    originalAvailableQuantity_(availableQuantity),
    originalDefaultQuantity_(defaultQuantity),
    originalEnabledState_(enabledState),
    originalPrice_(price),
    originalMaxRetailPrice_(maxRetailPrice),
    originalSpecialPrice_(specialPrice),
    originalWarningQuantity_(warningQuantity),
    numChangedProperties_(0),
    sizeID_(sizeID),
    sizeName_(sizeName)
{
}

TSizeProperties::TSizeProperties(const TSizeProperties &initializer)
  : newAvailableQuantity_(initializer.newAvailableQuantity_),
    newDefaultQuantity_(initializer.newDefaultQuantity_),
    newEnabledState_(initializer.newEnabledState_),
    newPrice_(initializer.newPrice_),
    newMaxRetailPrice_(initializer.newMaxRetailPrice_),
    newSpecialPrice_(initializer.newSpecialPrice_),
    newWarningQuantity_(initializer.newWarningQuantity_),
    originalAvailableQuantity_(initializer.originalAvailableQuantity_),
    originalDefaultQuantity_(initializer.originalDefaultQuantity_),
    originalEnabledState_(initializer.originalEnabledState_),
    originalPrice_(initializer.originalPrice_),
    originalMaxRetailPrice_(initializer.originalMaxRetailPrice_),
    originalSpecialPrice_(initializer.originalSpecialPrice_),
    originalWarningQuantity_(initializer.originalWarningQuantity_),
    numChangedProperties_(initializer.numChangedProperties_),
    sizeID_(initializer.sizeID_),
    sizeName_(initializer.sizeName_)
{
}

TSizeProperties::~TSizeProperties()
{
}

size_key_type
TSizeProperties::GetSizeID()
const
{
   return sizeID_;
}

double
TSizeProperties::GetPrice()
const
{
   return newPrice_;
}

double
TSizeProperties::GetMaxRetailPrice()
const
{
   return newMaxRetailPrice_;
}

double
TSizeProperties::GetSpecialPrice()
const
{
   return newSpecialPrice_;
}

double
TSizeProperties::GetAvailableQuantity()
const
{
   return newAvailableQuantity_;
}

double
TSizeProperties::GetDefaultQuantity()
const
{
   return newDefaultQuantity_;
}

double
TSizeProperties::GetWarningQuantity()
const
{
   return newWarningQuantity_;
}

bool
TSizeProperties::GetEnabledState()
const
{
   return newEnabledState_;
}

bool
TSizeProperties::HavePropertiesChanged()
const
{
   return numChangedProperties_ > 0;
}

double
TSizeProperties::SetPrice(double newPrice)
{
   return CompareAndUpdate<double>(
            newPrice_, newPrice, originalPrice_, numChangedProperties_);
}

double
TSizeProperties::SetMaxRetailPrice(double newMaxRetailPrice)
{
   return CompareAndUpdate<double>(
            newMaxRetailPrice_, newMaxRetailPrice, originalMaxRetailPrice_, numChangedProperties_);
}

double
TSizeProperties::SetSpecialPrice(double newSpecialPrice)
{
   return CompareAndUpdate<double>(newSpecialPrice_,
                                   newSpecialPrice,
                                   originalSpecialPrice_,
                                   numChangedProperties_);
}

double
TSizeProperties::SetAvailableQuantity(double newAvailableQuantity)
{
   return CompareAndUpdate<double>(newAvailableQuantity_,
                                   newAvailableQuantity,
                                   originalAvailableQuantity_,
                                   numChangedProperties_);
}

double
TSizeProperties::SetDefaultQuantity(double newDefaultQuantity)
{
   return CompareAndUpdate<double>(newDefaultQuantity_,
                                   newDefaultQuantity,
                                   originalDefaultQuantity_,
                                   numChangedProperties_);
}

double
TSizeProperties::SetWarningQuantity(double newWarningQuantity)
{
   return CompareAndUpdate<double>(newWarningQuantity_,
                                   newWarningQuantity,
                                   originalWarningQuantity_,
                                   numChangedProperties_);
}

bool
TSizeProperties::SetEnabledState(bool newEnabledState)
{
   return CompareAndUpdate<bool>(newEnabledState_,
                                 newEnabledState,
                                 originalEnabledState_,
                                 numChangedProperties_);
}

void
TSizeProperties::RevertToOriginalState()
{
   newAvailableQuantity_ = originalAvailableQuantity_;
   newEnabledState_ = originalEnabledState_;
   newDefaultQuantity_ = originalDefaultQuantity_;
   newPrice_ = originalPrice_;
   newMaxRetailPrice_ = originalMaxRetailPrice_;
   newSpecialPrice_ = originalSpecialPrice_;
   newWarningQuantity_ = originalWarningQuantity_;

   numChangedProperties_ = 0;
}

__fastcall
TfrmItemSizeSelection::TfrmItemSizeSelection(TComponent *owner, const UnicodeString &itemName, item_key_type itemID, bool itemAvailable)
   : TForm(Owner), itemID_(itemID), itemName_(itemName), properties_(),
     itemManager_(item_management::kickstarter::get_item_manager()),
     itemDefinition_(itemManager_.get_item_definition(itemID))
{
   initializeForm();
   cbItemAvailable->Checked = itemAvailable;
}

void __fastcall
TfrmItemSizeSelection::saveChangesClicked(TObject *sender)
{
    for (TSizePropertiesMap::iterator i = properties_.begin(); i != properties_.end(); ++i)
    {
        if (i->second.HavePropertiesChanged())
        {
            const TSizeProperties &properties = i->second;
            const i_size_definition &currentSizeDefinition = itemDefinition_.get_size_definition(properties.GetSizeID());
            const i_size_definition_factory &sizeDefinitionFactory = currentSizeDefinition.get_size_definition_factory();
            std::auto_ptr<i_size_definition> updatedSizeDefinition(sizeDefinitionFactory.create(currentSizeDefinition));

            updatedSizeDefinition->set_all(properties.GetAvailableQuantity(),
                                           properties.GetDefaultQuantity(),
                                           0,
                                           std::fabs(properties.GetAvailableQuantity()) != 0,
                                           currentSizeDefinition.get_key(),
                                           currentSizeDefinition.get_name(),
                                           properties.GetPrice(),
                                           properties.GetMaxRetailPrice(),
                                           0,
                                           properties.GetSpecialPrice(),
                                           properties.GetWarningQuantity());

            itemDefinition_.update_size_definition(*updatedSizeDefinition);
        }
    }

    itemDefinition_.set_enabled(cbItemAvailable->Checked);

    frmSelectDish->RedrawCourses();
    ModalResult = mrOk;
}


void __fastcall
TfrmItemSizeSelection::cancelClicked(TObject *sender)
{
    ModalResult = mrCancel;
}


void __fastcall
TfrmItemSizeSelection::sizeSelectionGridClicked(
  TObject *sender,
  TMouseButton button,
  TShiftState shift,
  TGridButton *gridButton)
{
    std::auto_ptr<TfrmItemSettings> itemSettingsDialog(new TfrmItemSettings(this, properties_[gridButton]));

    itemSettingsDialog->ShowModal();
}

void
TfrmItemSizeSelection::initializeForm()
{
    pnlItemName->Caption = itemName_;

    populateSizeSelectionData();
}

void
TfrmItemSizeSelection::initializeSizeSelectionGrid(unsigned int n_rows)
{
   tgridAvailableSizes->ColCount = 1;
   tgridAvailableSizes->RowCount = n_rows;
}

void
TfrmItemSizeSelection::populateSizeSelectionData()
{
   TSizeDefinitionMap availableSizes;
   unsigned int y = 0;

   itemDefinition_.get_available_sizes(availableSizes);

   initializeSizeSelectionGrid(availableSizes.size());

   for (TSizeDefinitionMap::const_iterator i = availableSizes.begin();
        i != availableSizes.end(); ++i) {
      const i_size_definition &size_definition = *i->second;

      tgridAvailableSizes->Buttons[y][0]->Caption = size_definition.get_name();

      properties_.insert(
        TSizePropertiesMap::value_type(
          tgridAvailableSizes->Buttons[y++][0],
          TSizeProperties(size_definition.get_name(),
                          i->first,
                          size_definition.get_price(),
                          size_definition.get_maxRetailPrice(),
                          size_definition.get_special_price(),
                          size_definition.get_available_quantity(),
                          size_definition.get_default_quantity(),
                          size_definition.get_warning_quantity(),
                          size_definition.get_enabled())));
   }
}

} /* item_management */

