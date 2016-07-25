//---------------------------------------------------------------------------

#ifndef VersionH
#define VersionH
//---------------------------------------------------------------------------
class TFileVersion
{
public:
	TFileVersion();
	TFileVersion(int major, int minor, int release, int build);
	TFileVersion(AnsiString FileName);

	AnsiString Version();

	int Major;
	int Minor;
	int Release;
	int Build;

	bool operator > (const TFileVersion &FileVersion)
	{
		return
			(Major > FileVersion.Major) ||
			(Major == FileVersion.Major && Minor > FileVersion.Minor) ||
			(Major == FileVersion.Major && Minor == FileVersion.Minor && Release > FileVersion.Release) ||
			(Major == FileVersion.Major && Minor == FileVersion.Minor && Release == FileVersion.Release && Build > FileVersion.Build);
	}
	bool operator < (const TFileVersion &FileVersion)
	{
		return !(operator >(FileVersion) || operator ==(FileVersion));
	}
	bool operator == (const TFileVersion &FileVersion)
	{
		return
			(Major == FileVersion.Major) &&
			(Minor == FileVersion.Minor) &&
			(Release == FileVersion.Release) &&
			(Build == FileVersion.Build);
	}
	bool operator >= (const TFileVersion &FileVersion)
	{
		bool LessThan = operator <(FileVersion);
		return !LessThan;
//		return !(operator <(FileVersion));
	}
	bool operator <= (const TFileVersion &FileVersion)
	{
		bool GreaterThan = operator >(FileVersion);
		return !GreaterThan;  
//		return !(operator >(FileVersion));
	}

private:
	void GetFileVersion(AnsiString FileName);
};
//---------------------------------------------------------------------------
//AnsiString GetFileVersion();
//AnsiString GetFileVersion(AnsiString FileName);
//---------------------------------------------------------------------------
#endif
