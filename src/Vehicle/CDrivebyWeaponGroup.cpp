#include "Vehicle/CDrivebyWeaponGroup.hpp"

#include "Vehicle/DrivebyCommons.hpp"
#include "Weapons/weapons.hpp"

bool CDrivebyWeaponGroup::allowsWeapon(const Stand::Weapon& w) const
{
	Stand::DrivebyCommons::backupDrivebyWeaponGroups();
	for (const auto& wg : Stand::DrivebyCommons::getDrivebyWeaponGroupBackup(m_Name).m_WeaponGroupNames)
	{
		if (wg == w.group)
		{
			return true;
		}
	}
	for (const auto& wh : m_WeaponTypeNames)
	{
		if (wh == w.hash)
		{
			return true;
		}
	}
	return false;
}
