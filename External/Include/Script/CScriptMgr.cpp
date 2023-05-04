#include "pch.h"
#include "CScriptMgr.h"

#include "CBlankScript.h"
#include "CBombOrkScript.h"
#include "CBossHPScript.h"
#include "CBossMissileScript.h"
#include "CBossScript.h"
#include "CCameraMoveScript.h"
#include "CCursorUISCript.h"
#include "CDoorScript.h"
#include "CEffectScript.h"
#include "CEndingLevelScript.h"
#include "CEnterScript.h"
#include "CExplosionScript.h"
#include "CFireLightScript.h"
#include "CFragileScript.h"
#include "CGameCameraScript.h"
#include "CHPUIScript.h"
#include "CLight2DScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CMstGunScript.h"
#include "CMstMissileScript.h"
#include "CPetScript.h"
#include "CPlayButtonUIScript.h"
#include "CPlayerScript.h"
#include "CShadowScript.h"
#include "CSpawnScript.h"
#include "CStateScript.h"
#include "CTorchScript.h"
#include "CWalkEffectScript.h"
#include "CWaveScript.h"
#include "CWeaponScript.h"
#include "CWeaponUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBlankScript");
	_vec.push_back(L"CBombOrkScript");
	_vec.push_back(L"CBossHPScript");
	_vec.push_back(L"CBossMissileScript");
	_vec.push_back(L"CBossScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCursorUISCript");
	_vec.push_back(L"CDoorScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CEndingLevelScript");
	_vec.push_back(L"CEnterScript");
	_vec.push_back(L"CExplosionScript");
	_vec.push_back(L"CFireLightScript");
	_vec.push_back(L"CFragileScript");
	_vec.push_back(L"CGameCameraScript");
	_vec.push_back(L"CHPUIScript");
	_vec.push_back(L"CLight2DScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMstGunScript");
	_vec.push_back(L"CMstMissileScript");
	_vec.push_back(L"CPetScript");
	_vec.push_back(L"CPlayButtonUIScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CShadowScript");
	_vec.push_back(L"CSpawnScript");
	_vec.push_back(L"CStateScript");
	_vec.push_back(L"CTorchScript");
	_vec.push_back(L"CWalkEffectScript");
	_vec.push_back(L"CWaveScript");
	_vec.push_back(L"CWeaponScript");
	_vec.push_back(L"CWeaponUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBlankScript" == _strScriptName)
		return new CBlankScript;
	if (L"CBombOrkScript" == _strScriptName)
		return new CBombOrkScript;
	if (L"CBossHPScript" == _strScriptName)
		return new CBossHPScript;
	if (L"CBossMissileScript" == _strScriptName)
		return new CBossMissileScript;
	if (L"CBossScript" == _strScriptName)
		return new CBossScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCursorUISCript" == _strScriptName)
		return new CCursorUISCript;
	if (L"CDoorScript" == _strScriptName)
		return new CDoorScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CEndingLevelScript" == _strScriptName)
		return new CEndingLevelScript;
	if (L"CEnterScript" == _strScriptName)
		return new CEnterScript;
	if (L"CExplosionScript" == _strScriptName)
		return new CExplosionScript;
	if (L"CFireLightScript" == _strScriptName)
		return new CFireLightScript;
	if (L"CFragileScript" == _strScriptName)
		return new CFragileScript;
	if (L"CGameCameraScript" == _strScriptName)
		return new CGameCameraScript;
	if (L"CHPUIScript" == _strScriptName)
		return new CHPUIScript;
	if (L"CLight2DScript" == _strScriptName)
		return new CLight2DScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMstGunScript" == _strScriptName)
		return new CMstGunScript;
	if (L"CMstMissileScript" == _strScriptName)
		return new CMstMissileScript;
	if (L"CPetScript" == _strScriptName)
		return new CPetScript;
	if (L"CPlayButtonUIScript" == _strScriptName)
		return new CPlayButtonUIScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CShadowScript" == _strScriptName)
		return new CShadowScript;
	if (L"CSpawnScript" == _strScriptName)
		return new CSpawnScript;
	if (L"CStateScript" == _strScriptName)
		return new CStateScript;
	if (L"CTorchScript" == _strScriptName)
		return new CTorchScript;
	if (L"CWalkEffectScript" == _strScriptName)
		return new CWalkEffectScript;
	if (L"CWaveScript" == _strScriptName)
		return new CWaveScript;
	if (L"CWeaponScript" == _strScriptName)
		return new CWeaponScript;
	if (L"CWeaponUIScript" == _strScriptName)
		return new CWeaponUIScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BLANKSCRIPT:
		return new CBlankScript;
		break;
	case (UINT)SCRIPT_TYPE::BOMBORKSCRIPT:
		return new CBombOrkScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSHPSCRIPT:
		return new CBossHPScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSMISSILESCRIPT:
		return new CBossMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSCRIPT:
		return new CBossScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSORUISCRIPT:
		return new CCursorUISCript;
		break;
	case (UINT)SCRIPT_TYPE::DOORSCRIPT:
		return new CDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ENDINGLEVELSCRIPT:
		return new CEndingLevelScript;
		break;
	case (UINT)SCRIPT_TYPE::ENTERSCRIPT:
		return new CEnterScript;
		break;
	case (UINT)SCRIPT_TYPE::EXPLOSIONSCRIPT:
		return new CExplosionScript;
		break;
	case (UINT)SCRIPT_TYPE::FIRELIGHTSCRIPT:
		return new CFireLightScript;
		break;
	case (UINT)SCRIPT_TYPE::FRAGILESCRIPT:
		return new CFragileScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMECAMERASCRIPT:
		return new CGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::HPUISCRIPT:
		return new CHPUIScript;
		break;
	case (UINT)SCRIPT_TYPE::LIGHT2DSCRIPT:
		return new CLight2DScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MSTGUNSCRIPT:
		return new CMstGunScript;
		break;
	case (UINT)SCRIPT_TYPE::MSTMISSILESCRIPT:
		return new CMstMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PETSCRIPT:
		return new CPetScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYBUTTONUISCRIPT:
		return new CPlayButtonUIScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SHADOWSCRIPT:
		return new CShadowScript;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNSCRIPT:
		return new CSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::STATESCRIPT:
		return new CStateScript;
		break;
	case (UINT)SCRIPT_TYPE::TORCHSCRIPT:
		return new CTorchScript;
		break;
	case (UINT)SCRIPT_TYPE::WALKEFFECTSCRIPT:
		return new CWalkEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::WAVESCRIPT:
		return new CWaveScript;
		break;
	case (UINT)SCRIPT_TYPE::WEAPONSCRIPT:
		return new CWeaponScript;
		break;
	case (UINT)SCRIPT_TYPE::WEAPONUISCRIPT:
		return new CWeaponUIScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BLANKSCRIPT:
		return L"CBlankScript";
		break;

	case SCRIPT_TYPE::BOMBORKSCRIPT:
		return L"CBombOrkScript";
		break;

	case SCRIPT_TYPE::BOSSHPSCRIPT:
		return L"CBossHPScript";
		break;

	case SCRIPT_TYPE::BOSSMISSILESCRIPT:
		return L"CBossMissileScript";
		break;

	case SCRIPT_TYPE::BOSSSCRIPT:
		return L"CBossScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CURSORUISCRIPT:
		return L"CCursorUISCript";
		break;

	case SCRIPT_TYPE::DOORSCRIPT:
		return L"CDoorScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::ENDINGLEVELSCRIPT:
		return L"CEndingLevelScript";
		break;

	case SCRIPT_TYPE::ENTERSCRIPT:
		return L"CEnterScript";
		break;

	case SCRIPT_TYPE::EXPLOSIONSCRIPT:
		return L"CExplosionScript";
		break;

	case SCRIPT_TYPE::FIRELIGHTSCRIPT:
		return L"CFireLightScript";
		break;

	case SCRIPT_TYPE::FRAGILESCRIPT:
		return L"CFragileScript";
		break;

	case SCRIPT_TYPE::GAMECAMERASCRIPT:
		return L"CGameCameraScript";
		break;

	case SCRIPT_TYPE::HPUISCRIPT:
		return L"CHPUIScript";
		break;

	case SCRIPT_TYPE::LIGHT2DSCRIPT:
		return L"CLight2DScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MSTGUNSCRIPT:
		return L"CMstGunScript";
		break;

	case SCRIPT_TYPE::MSTMISSILESCRIPT:
		return L"CMstMissileScript";
		break;

	case SCRIPT_TYPE::PETSCRIPT:
		return L"CPetScript";
		break;

	case SCRIPT_TYPE::PLAYBUTTONUISCRIPT:
		return L"CPlayButtonUIScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SHADOWSCRIPT:
		return L"CShadowScript";
		break;

	case SCRIPT_TYPE::SPAWNSCRIPT:
		return L"CSpawnScript";
		break;

	case SCRIPT_TYPE::STATESCRIPT:
		return L"CStateScript";
		break;

	case SCRIPT_TYPE::TORCHSCRIPT:
		return L"CTorchScript";
		break;

	case SCRIPT_TYPE::WALKEFFECTSCRIPT:
		return L"CWalkEffectScript";
		break;

	case SCRIPT_TYPE::WAVESCRIPT:
		return L"CWaveScript";
		break;

	case SCRIPT_TYPE::WEAPONSCRIPT:
		return L"CWeaponScript";
		break;

	case SCRIPT_TYPE::WEAPONUISCRIPT:
		return L"CWeaponUIScript";
		break;

	}
	return nullptr;
}