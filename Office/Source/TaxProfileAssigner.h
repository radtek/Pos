//---------------------------------------------------------------------------

#ifndef TaxProfileAssignerH
#define TaxProfileAssignerH
//---------------------------------------------------------------------------

#include "TaxProfile.h"

#include <vector>
#include <set>

typedef std::set<__int32> TaxProfileKeyList;
typedef std::set<__int32>::iterator TaxProfileKeyIterator;

/*
typedef std::set<TaxProfile> TaxProfileList;
typedef std::set<TaxProfile>::iterator TaxProfileIterator;
typedef std::set<TaxProfile>::const_iterator TaxProfileConstIterator;
*/

typedef std::set<TaxProfile*> TaxProfileList;
typedef std::set<TaxProfile*>::iterator TaxProfileIterator;
typedef std::set<TaxProfile*>::const_iterator TaxProfileConstIterator;


// Lower level class to communicate only to the TaxProfileAssigner and nothing else.
// Receives TaxProfile vectors, and individual TaxProfiles to be transferred between vectors.
// Responsible for storing the TaxProfiles data and transferring the TaxProfiles.
class TaxProfileWrapper
{
   public:
      // Allocates memory
      TaxProfileWrapper();

      // Destroyes memory
      ~TaxProfileWrapper();

      // Determines what TaxProfiles are available for the ItemSize.
      void InitialiseTaxProfile(TaxProfileList inItemSizeTPs, TaxProfileList inTotalTPs);

      // Transfers a TaxProfile between 2 TaxProfile vectors
      void Transfer(
              const TaxProfile* const inTp,
              TaxProfileList *t,
              TaxProfileList *s);

      TaxProfileList* GetAvailableTPs() { return availableTPs; }
      TaxProfileList* GetItemSizeTPs() { return itemSizeTPs; }
      void Clear();

   private:
      TaxProfileList *availableTPs;
      TaxProfileList *itemSizeTPs;

      bool findTaxProfile(
              TaxProfile*    inTP,
              TaxProfileList inTPList);

      // NOT USED!!!
      void vectorFromSet(
              const TaxProfileList*          const inSet,
                    std::vector<TaxProfile>* const outVector );
      // NOT USED!!!
      void setFromVector(
              const std::vector<TaxProfile>* const inVector,
                    TaxProfileList*          const outSet );
};


typedef void __fastcall (__closure *TAssignedTaxProfilesEvent)( TObject* sender, TStrings* const taxProfileContainer );

// Higher level class to interact with external classes.
// Receives vectors of total TaxProfiles and for each ItemSize.
// Responsible for deciding which TaxProfile gets displayed in what ListBox
// and for passing the TaxProfiles down to a lower level wrapper class.
class TTaxProfileAssigner : public TObject
{
   private:
      TAssignedTaxProfilesEvent FOnAssignedTPChanged;

   // Public Interface
   public:
      // Initialize private members, such as: Total TaxProfiles in the Menu, and 2 pointers to the ListBox GUIs
      __fastcall TTaxProfileAssigner(const TaxProfileList intotalTPs, TStrings *inunassignedTPs, TStrings *inassignedTPs);
      virtual __fastcall ~TTaxProfileAssigner(){}

      // Initilalize the TaxProfiles for the ItemSize
      void ItemSizeTaxProfiles(TaxProfileList initemSizeTPs);

      // public method to be called when TaxProfiles are transfered between ListBoxes
      void Transfer(TCustomListBox *inListBox);

      __property TAssignedTaxProfilesEvent OnAssignedTaxProfilesChanged = {read=FOnAssignedTPChanged, write=FOnAssignedTPChanged};

   private:
      void Display();
      void Display(TaxProfileList *t, TStrings *s);
      void Clear();

      //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
      // SORTING

      // Sort tax profile. Criteria: Priority
      void sortTaxProfiles( TStrings* const s );

      // The quicksort recursive function
      void quicksort( TStrings* const s, int p, int r );

      // The partition function
      int partition( TStrings* const s, int p, int r );

      // Sort tax profile. Criteria: Value
      void TTaxProfileAssigner::sortTaxProfileList( TaxProfileList &inTPs );

      // The quicksort recursive function
      void TTaxProfileAssigner::quicksortProfileList( TaxProfileList &inTPs, int p, int r );

      // The partition function
      int TTaxProfileAssigner::partition( TaxProfileList &inTPs, int p, int r );

      //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


      TaxProfileWrapper taxProfiles;
      const TaxProfileList totalTPs;
      TStrings *unassignedTPs;
      TStrings *assignedTPs;
};

#endif
