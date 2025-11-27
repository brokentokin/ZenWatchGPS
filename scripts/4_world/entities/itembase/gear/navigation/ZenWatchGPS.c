class ZenWatchGPS extends Clothing
{
	protected ChemlightLight m_ZenLight;

	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTurnOnWhileInHands);
		AddAction(ActionTurnOffWhileInHands);
	}

	ChemlightLight GetZenLight()
	{
		return m_ZenLight;
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();

		RegisterZenGpsDeviceToPlayer(true);
		SetObjectTexture(0, "ZenWatchGPS/data/watch/textures/gps_on_co.paa");
	}

	override void OnWork (float consumed_energy)
	{
		super.OnWork(consumed_energy);

		#ifndef SERVER
		if (!m_ZenLight)
			CreateZenLight();
		#endif
	}
	
	override void OnWorkStop()
	{
		super.OnWorkStop();

		RegisterZenGpsDeviceToPlayer(false);
		SetObjectTexture(0, "ZenWatchGPS/data/watch/textures/gps_off_co.paa");

		if (m_ZenLight)
		{
			m_ZenLight.FadeOut();
		}
	}

	bool IsZenWatchTurnedOn()
	{
		return GetCompEM() && GetCompEM().IsWorking();
	}

	bool CanZenWatchShowTime()
	{
		return true;
	}

	override bool DescriptionOverride(out string output)
	{
		if (!CanZenWatchShowTime() || !IsZenWatchTurnedOn())
		{
			return false;
		}

		string cfgPath = "CfgVehicles " + GetType();
		GetGame().ConfigGetText(cfgPath + " descriptionShort", output);

		if (GetHealthLevel() >= GameConstants.STATE_DAMAGED)
		{
			output = output + " #STR_ZenWatch_Damaged";
			return true;
		}

		bool isNight = GetGame().GetWorld().IsNight();
		string daytime = "#STR_ZenWatch_Day";
		if (isNight)
			daytime = "#STR_ZenWatch_Night";

		int year, month, day, hour, minute;
        GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		string hourPreStr = "";
		if (hour <= 9)
			hourPreStr = "0";

		string minPreStr = "";
		if (minute <= 9)
			minPreStr = "0";

		output = output + " #STR_ZenWatchTime " + hourPreStr + hour + ":" + minPreStr + minute + " " + daytime + ".";
		return true;
	}

	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);

		string slot_name = InventorySlots.GetSlotName(slot_id);
		if (slot_name != "Armband")
			return;
		
		PlayerBase player = PlayerBase.Cast(parent);
		if (parent.GetHierarchyRoot())
		{
			player = PlayerBase.Cast(parent.GetHierarchyRoot());			
		}

		if (player)
		{
			if (IsZenWatchTurnedOn())
			{
				player.MapNavigationItemInPossession(this);
			}
		}
	}
	
	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);

		string slot_name = InventorySlots.GetSlotName(slot_id);
		if (slot_name != "Armband")
			return;

		PlayerBase player = PlayerBase.Cast(parent);
		if (parent.GetHierarchyRoot())
		{
			player = PlayerBase.Cast(parent.GetHierarchyRoot());			
		}

		if (player)
		{
			player.MapNavigationItemNotInPossession(this);
		}
	}

	void RegisterZenGpsDeviceToPlayer(bool isTurnedOn)
	{
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());

		if (player)
		{
			if (isTurnedOn)
			{
				player.MapNavigationItemInPossession(this);
			}
			else 
			{
				player.MapNavigationItemNotInPossession(this);
			}
		}
	}

	void CreateZenLight()
	{
		// todo: why the fuck does this light appear under player's feet and not on the proxy object when worn??
		
		/*
		if (!GetGame().IsDedicatedServer() || !GetGame().IsMultiplayer())
		{
			m_ZenLight = ChemlightLight.Cast(ScriptedLightBase.CreateLight(ChemlightLight, "0 0 0"));
			m_ZenLight.AttachOnMemoryPoint(this, "light");
			m_ZenLight.SetColorToGreen();
			m_ZenLight.SetIntensity(0.1, GetCompEM().GetUpdateInterval());
		}
		*/
	}
}

class ZenWatchGPS_NoTime extends ZenWatchGPS
{
	override bool CanZenWatchShowTime()
	{
		return false;
	}
}