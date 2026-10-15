#ifdef EXPANSIONMODNAVIGATION
modded class PlayerBase
{
	void PlayerBase()
	{
		// Expansion tracks inventory and power changes for registered item types.
		Expansion_RegisterInventoryItemType(ZenWatchGPS);
	}

	override bool HasItemGPS()
	{
		return super.HasItemGPS() || Expansion_GetInventoryCount(ZenWatchGPS, true) > 0;
	}

	override bool HasItemCompass()
	{
		return super.HasItemCompass() || Expansion_GetInventoryCount(ZenWatchGPS, true) > 0;
	}
}
#endif
