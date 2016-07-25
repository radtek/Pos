//---------------------------------------------------------------------------

#ifndef PalmUserSettingsH
#define PalmUserSettingsH
//---------------------------------------------------------------------------
struct TPalmUserSettings
{
   enum TBitwiseUserSettings
   {
	  usAdvancedUser = 0x0001, usPriceAdjust = 0x0002, usAutoDetails = 0x0004, usIsLargeFont = 0x0008, usScreenLock = 0x0010,
	  usClassicMode = 0x0020, usSoundLevelBit1 = 0x0040, usSoundLevelBit2 = 0x0080, usGetAllTables = 0x0100
   };

   enum TPalmSoundLevel
   {
	  slNone, slLow, slMedium, slHigh
   };

   TPalmUserSettings() : AdvanceUser(false), PriceAdjust(false), AutoDetails(false), LargeFont(true), ScreenLock(true), ClassicMode(false),
   GetAllTables(true), SoundLevel(slMedium)
   {
   }

   TPalmUserSettings(unsigned short Bitfield)
   {
	  this->operator = (Bitfield);
   }

   bool AdvanceUser;
   bool PriceAdjust;
   bool AutoDetails;
   bool LargeFont;
   bool ScreenLock;
   bool ClassicMode;
   TPalmSoundLevel SoundLevel;
   bool GetAllTables;

   TPalmUserSettings& operator = (unsigned short Bitfield)
   {
	  AdvanceUser = Bitfield & usAdvancedUser;
	  PriceAdjust = Bitfield & usPriceAdjust;
	  AutoDetails = Bitfield & usAutoDetails;
	  LargeFont = Bitfield & usIsLargeFont;
	  ScreenLock = Bitfield & usScreenLock;
	  ClassicMode = Bitfield & usClassicMode;
	  SoundLevel = static_cast <TPalmSoundLevel> ((Bitfield & usSoundLevelBit1 ? 1 : 0) + (Bitfield & usSoundLevelBit2 ? 2 : 0));
	  GetAllTables = Bitfield & usGetAllTables;

	  return *this;
   }operator unsigned short()const
   {
	  return(AdvanceUser ? usAdvancedUser : 0) | (PriceAdjust ? usPriceAdjust : 0) | (AutoDetails ? usAutoDetails : 0) |
		  (LargeFont ? usIsLargeFont : 0) | (ScreenLock ? usScreenLock : 0) | (ClassicMode ? usClassicMode : 0) |
		  ((SoundLevel & 1) ? usSoundLevelBit1 : 0) | ((SoundLevel & 2) ? usSoundLevelBit2 : 0) | (GetAllTables ? usGetAllTables : 0);
   }
};
#endif
