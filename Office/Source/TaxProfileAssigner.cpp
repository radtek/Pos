//---------------------------------------------------------------------------


#pragma hdrstop

#include "TaxProfileAssigner.h"
#include <algorithm>

//---------------------------------------------------------------------------

#pragma package(smart_init)


// Allocates memory to the available TaxProfiles and ItemSize TaxProfiles
TaxProfileWrapper::TaxProfileWrapper()
{
   availableTPs = new TaxProfileList();
   itemSizeTPs  = new TaxProfileList();
}

// Destroyes the memory of the available TaxProfiles and ItemSize TaxProfiles
TaxProfileWrapper::~TaxProfileWrapper()
{
   delete availableTPs;
   delete itemSizeTPs;
}

// Removes the TaxProfile from one TaxProfile vector and stores it into another.
void TaxProfileWrapper::Transfer(
                           const TaxProfile* const inTp,
                           TaxProfileList *t,
                           TaxProfileList *s)
{
   try
   {
      TaxProfileIterator it = std::find( t->begin(), t->end(), inTp);
      s->insert(*it);
      t->erase(it);
   }
   catch(Exception &ex)
   {
      Application->ShowException(&ex);
   }
}

void TaxProfileWrapper::Clear()
{
   if(availableTPs->size())
      availableTPs->clear();
}

// Called when a ItemSize is selected. It needs to determine what TaxProfiles are available based on
// the total TaxProfiles and the TaxProfiles for the ItemSize.
void TaxProfileWrapper::InitialiseTaxProfile(TaxProfileList inItemSizeTPs, TaxProfileList inTotalTPs)
{
   availableTPs->clear();
    itemSizeTPs->clear();

   TaxProfileIterator tpIT;

   for( tpIT = inTotalTPs.begin(); tpIT != inTotalTPs.end(); tpIT++ )
   {
      TaxProfile *tp = *tpIT;

      if( findTaxProfile( tp, inItemSizeTPs ) )
      {
         itemSizeTPs->insert( tp );
      }
      else
      {
         availableTPs->insert( tp );
      }
   }
}
//---------------------------------------------------------------------------
bool TaxProfileWrapper::findTaxProfile( TaxProfile* inTP, TaxProfileList inTPList)
{
   TaxProfileIterator tpIT;

   for( tpIT = inTPList.begin(); tpIT != inTPList.end(); tpIT++ )
   {
      if( ( *tpIT )->GetTaxProfileDBKey() == inTP->GetTaxProfileDBKey() )
      {
         break;
      }
   }

   return tpIT != inTPList.end();
}
//---------------------------------------------------------------------------
// NOT USED!!!
void TaxProfileWrapper::vectorFromSet(
                             const TaxProfileList*          const inSet,
                                   std::vector<TaxProfile>* const outVector )
{
   outVector->clear();

   TaxProfileIterator setIT;

   for( setIT = inSet->begin(); setIT != inSet->end(); setIT++ )
   {
        outVector->push_back( *( *setIT ) );
   }
}
//---------------------------------------------------------------------------
// NOT USED!!!
void TaxProfileWrapper::setFromVector(
                             const std::vector<TaxProfile>* const inVector,
                                   TaxProfileList*          const outSet )
{
   outSet->clear();

   std::vector<TaxProfile>::const_iterator vectorIT;

   for( vectorIT = inVector->begin(); vectorIT != inVector->end(); vectorIT++ )
   {
      TaxProfile *tp = new TaxProfile( vectorIT->GetTaxProfileDBKey() );

      tp->taxProfileName = vectorIT->taxProfileName;
      tp->taxPercentage  = vectorIT->taxPercentage;
      tp->taxProfileType = vectorIT->taxProfileType;
      tp->taxPriority    = vectorIT->taxPriority;
      tp->taxSelectable  = true;

      outSet->insert( tp );
   }
}
//---------------------------------------------------------------------------



// Initialize private members, such as: Total TaxProfiles in the Menu, and 2 pointers to the ListBox GUIs
__fastcall TTaxProfileAssigner::TTaxProfileAssigner(const TaxProfileList intotalTPs, TStrings *inunassignedTPs, TStrings *inassignedTPs):
                                                         totalTPs(intotalTPs),
                                                         unassignedTPs((TStrings *)inunassignedTPs),
                                                         assignedTPs((TStrings *)inassignedTPs)
{
}

// Initilalize the TaxProfiles for the ItemSize
void TTaxProfileAssigner::ItemSizeTaxProfiles(TaxProfileList initemSizeTPs)
{
   taxProfiles.Clear();

   taxProfiles.InitialiseTaxProfile(initemSizeTPs, totalTPs);

   Clear();
   Display();
}

// Clears the GUI ListBox display
void TTaxProfileAssigner::Clear()
{
   unassignedTPs->Clear();
   assignedTPs->Clear();
}

// Displays the data in the ListBoxes
void TTaxProfileAssigner::Display()
{
   Display(taxProfiles.GetAvailableTPs(), unassignedTPs);
   Display(taxProfiles.GetItemSizeTPs(),  assignedTPs);
}

// Adds the TaxProfiles to the ListBox TStrings pointer
void TTaxProfileAssigner::Display(TaxProfileList *t, TStrings *s)
{
   //for(TaxProfileConstIterator it = t->begin();
   for(TaxProfileIterator it = t->begin();
       it != t->end();
       std::advance(it,1))
   {
      s->AddObject( ( *it )->taxProfileName, ( TObject* )( *it ) );
   }

   sortTaxProfiles( s );
}

// public method to be called when TaxProfiles are transfered between ListBoxes
void TTaxProfileAssigner::Transfer(TCustomListBox *inListBox)
{
   if(inListBox->ItemIndex != -1)
   {
      TaxProfileList *ar[2] = {
                                taxProfiles.GetAvailableTPs(),
                                taxProfiles.GetItemSizeTPs()
                              };

      int sel = !(int)(inListBox->Items == assignedTPs);

      TaxProfile* t= ( TaxProfile* )inListBox->Items->Objects[inListBox->ItemIndex];
      taxProfiles.Transfer( t, ar[!sel], ar[sel] );

      Clear();
      Display();

      if( FOnAssignedTPChanged != NULL )
      {
         FOnAssignedTPChanged( this, assignedTPs );
      }
   }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// SORTING
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// Sort tax profile. Criteria: Priority
void TTaxProfileAssigner::sortTaxProfiles( TStrings* const s )
{
   if( s->Count > 0 )
   {
      quicksort( s, 0, s->Count-1 );
   }
}
//---------------------------------------------------------------------------
// The quicksort recursive function
void TTaxProfileAssigner::quicksort( TStrings *input, int p, int r )
{
    if ( p < r )
    {
        int j = partition( input, p, r );

        quicksort( input, p,   j-1);
        quicksort( input, j+1, r  );
    }
}
//---------------------------------------------------------------------------
// The partition function
int TTaxProfileAssigner::partition( TStrings *input, int p, int r )
{
    TaxProfile *pivot = ( TaxProfile* )input->Objects[r];

    while ( p < r )
    {
        TaxProfile *pTP = ( TaxProfile* )input->Objects[p];
        while ( pTP->taxPriority < pivot->taxPriority )
        {
            p++;
            pTP = ( TaxProfile* )input->Objects[p];
        }

        TaxProfile *rTP = ( TaxProfile* )input->Objects[r];
        while ( rTP->taxPriority > pivot->taxPriority )
        {
            r--;
            rTP = ( TaxProfile* )input->Objects[r];
        }

        pTP = ( TaxProfile* )input->Objects[p];
        rTP = ( TaxProfile* )input->Objects[r];

        if ( pTP->taxPriority == rTP->taxPriority )
            p++;
        else if ( p < r )
        {
            AnsiString  tmpName = input->Strings[p];
            TaxProfile *tmpTP   = ( TaxProfile* )input->Objects[p];

            input->Strings[p] = input->Strings[r];
            input->Objects[p] = input->Objects[r];

            input->Strings[r] = tmpName;
            input->Objects[r] = ( TObject* )tmpTP;
        }
    }

    return r;

/*
// The original partition
    int pivot = input[r];

    while ( p < r )
    {
        while ( input[p] < pivot )
            p++;

        while ( input[r] > pivot )
            r--;

        if ( input[p] == input[r] )
            p++;
        else if ( p < r )
        {
            int tmp = input[p];
            input[p] = input[r];
            input[r] = tmp;
        }
    }

    return r;
*/
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::

