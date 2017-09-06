#pragma once

#pragma pack(push)
#pragma pack(1)


struct IntDamageByCover
{
	int		AccountKey;
	int		PolicyKey;
	int		SiteKey;
	int		StructureKey;
	short	CoverageID;
	int		AnlCfgKey;
	short	ModelRegionID;
	short	PerilID;
	int		GridID;
	short	RankIndex;
	int		DamageLen;
};


#pragma pack(pop)
