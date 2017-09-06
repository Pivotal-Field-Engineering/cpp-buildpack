#pragma once

#pragma pack(push)
#pragma pack(1)


struct PdamageA
{
	int			ExposureKey;
	int			AccountKey;
	int			PolicyKey;
	int			SiteKey;
	int			StructureKey;
	short		CoverageID;
	int			AnlCfgKey;
	short		PerilID;
	short		ModelRegionID;
	int			CountryKey;
	long long	Fips;
	long long	Postcode;
	int			RegionKey;
	int			DamageLen;
};


#pragma pack(pop)
