#pragma once

#pragma pack(push)
#pragma pack(1)


struct IntDamageByEvent
{
	int		AccountKey;
	int		AnlCfgKey;
	short	ModelRegionID;
	short	PerilID;
	int		EventID;
	short	CountryKey;
	short	ChunkID;
	int		DamageLen;
};


#pragma pack(pop)
