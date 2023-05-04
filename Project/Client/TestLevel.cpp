#include "pch.h"
#include "TestLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CAstarMgr.h>

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Script/CLight2DScript.h>
#include <Script/CPlayerScript.h>
#include <Script/CStateScript.h>
#include <Script/CMissileScript.h>
#include <Script/CWeaponScript.h>
#include <Script/CGameCameraScript.h>
#include <Script/CPetScript.h>
#include <Script/CMonsterScript.h>
#include <Script/CEffectScript.h>
#include <Script/CShadowScript.h>
#include <Script/CWalkEffectScript.h>b
#include <Script/MstStates.h>
#include <Script/CMstGunScript.h>
#include <Script/CMstMissileScript.h>
#include <Script/CBossScript.h>
#include <Script/CBossMissileScript.h>
#include <Script/CFragileScript.h>
#include <Script/CFireLightScript.h>
#include <Script/CWaveScript.h>
#include <Script/CSpawnScript.h>
#include <Script/CDoorScript.h>
#include <Script/CEnterScript.h>
#include <Script/CCursorUISCript.h>
#include <Script/CScriptMgr.h>
#include <Script/CUIMgr.h>
#include <Script/CPlayButtonUIScript.h>
#include <Script/CEndingLevelScript.h>
#include <Script/CBlankScript.h>
void CreateTestLevel()
{

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::PLAY);

	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"Wall");

	// Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	//pMainCam->AddComponent(new CGameCameraScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC); // 투영 설정
	pMainCam->Camera()->SetCameraIndex(0);  // MainCamera로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);
	pMainCam->Camera()->SetLayerMask(31, false);

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), L"Default");
	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);
	
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex(1);		// MainCamera 로 설정
	pUICam->Camera()->SetLayerMask(31, true);	// 모든 레이어 체크
	
	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);
	
	CGameObject* pMainLevelUI = new CGameObject;
	pMainLevelUI->AddComponent(new CTransform);
	pMainLevelUI->AddComponent(new CPlayButtonUIScript);
	
	SpawnGameObject(pMainLevelUI, Vec3(0.f, 0.f, 0.f), (int)LAYER::UI);

	CGameObject* pCursor = new CGameObject;
	pCursor->SetName(L"Cursor");
	pCursor->AddComponent(new CTransform);
	pCursor->AddComponent(new CMeshRender);
	pCursor->AddComponent(new CCursorUISCript);

	pCursor->Transform()->SetRelativeScale(40.f, 40.f, 1.f);
	pCursor->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pCursor->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"CursorMtrl"));
	pCursor->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"AIM"));

	SpawnGameObject(pCursor, Vec3(300.f, 200.f, 1.f), (int)LAYER::UI);

	////왜곡처리 포스트 프로세싱 용
	//CGameObject* pDistortion = new CGameObject;
	//pDistortion->SetName(L"Distortion");
	//pDistortion->AddComponent(new CTransform);
	//pDistortion->AddComponent(new CMeshRender);
	//pDistortion->AddComponent(new CBlankScript);
	//pDistortion->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));
	//pDistortion->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pDistortion->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"));
	//
	//SpawnGameObject(pDistortion, Vec3(0.f, 0.f, 1.f), (int)LAYER::DEFAULT);

	return;
	// 흑백처리 포스트 프로세싱 용 
	//CGameObject* pGray = new CGameObject;
	//pGray->SetName(L"GrayScale");
	//pGray->AddComponent(new CTransform);
	//pGray->AddComponent(new CMeshRender);
	//pGray->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pGray->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl"));
	//
	//SpawnGameObject(pGray, Vec3(0.f, 0.f, 0.f), 0);
	 
	//CGameObject* pBossIntro = new CGameObject;
	//pBossIntro->SetName(L"BossIntro");
	//pBossIntro->AddComponent(new CTransform);
	//pBossIntro->AddComponent(new CMeshRender);
	//
	//pBossIntro->Transform()->SetRelativeScale(1280.f, 768.f, 1.f);
	//pBossIntro->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pBossIntro->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pBossIntro->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BOSS_INTRO"));
	//
	//SpawnGameObject(pBossIntro, Vec3(0.f, 0.f, 3.f), (int)LAYER::UI);
	// 
	//CGameObject* pCrossBox = new CGameObject;
	//pCrossBox->SetName(L"Crossbow");
	//pCrossBox->AddComponent(new CTransform);
	//pCrossBox->AddComponent(new CMeshRender);
	//pCrossBox->AddComponent(new CCollider2D);
	//pCrossBox->AddComponent(new CMissileScript);
	//
	//pCrossBox->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	//pCrossBox->Transform()->SetAbsolute(true);
	//pCrossBox->Transform()->SetRelativeScale(60.f, 20.f, 1.f);
	//pCrossBox->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pCrossBox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"CrossbowMtrl"));
	//pCrossBox->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BULLET_CROSSBOW"));
	//
	//SpawnGameObject(pCrossBox, Vec3(0.f, 0.f, 5.f), (int)LAYER::UI);
	


	//CGameObject* pMain = new CGameObject;
	//pMain->SetName(L"Ending");
	//pMain->AddComponent(new CTransform);
	//pMain->AddComponent(new CMeshRender);
	//pMain->AddComponent(new CAnimator2D);
	//
	//pMain->Transform()->SetRelativeScale(1280.f, 768.f, 1.f);
	//pMain->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pMain->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));
	//pMain->Animator2D()->LoadAnimFromFile(pMain->GetName());
	//pMain->Animator2D()->Play(L"Ending", false);
	//
	//SpawnGameObject(pMain, Vec3(0.f, 0.f, 1.f), (int)LAYER::UI);
	//
	// 
	//CResMgr::GetInst()->FindRes<CSound>(L"MenuBGM")->Play(0, 1.f);
	//
	//
	//CGameObject* pPlayButton = new CGameObject;
	//pPlayButton->SetName(L"PlayButton");
	//pPlayButton->AddComponent(new CTransform);
	//pPlayButton->AddComponent(new CMeshRender);
	//pPlayButton->AddComponent(new CAnimator2D);
	//
	//pPlayButton->Transform()->SetRelativeScale(200.f, 100.f, 1.f);
	//pPlayButton->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pPlayButton->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	//pPlayButton->Animator2D()->LoadAnimFromFile(pPlayButton->GetName());
	//pPlayButton->Animator2D()->Play(L"PlayButton", true);
	//
	//SpawnGameObject(pPlayButton, Vec3(-530.f, -300.f, 1.f), (int)LAYER::UI);
	//
	//CGameObject* pLogo = new CGameObject;
	//pLogo->SetName(L"Logo");
	//pLogo->AddComponent(new CTransform);
	//pLogo->AddComponent(new CMeshRender);
	//
	//pLogo->Transform()->SetRelativeScale(700.f, 250.f, 1.f);
	//pLogo->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pLogo->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	//pLogo->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TITLE"));
	//pLogo->MeshRender()->GetDynamicMaterial();
	//
	//SpawnGameObject(pLogo, Vec3(0.f, 100.f, 1.f), (int)LAYER::UI);

	// Light Object 생성
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight2D);
	pLightObj->AddComponent(new CLight2DScript);

	pLightObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pLightObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 2.f));

	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetAngle(XM_PI / 2.f);
	pLightObj->Light2D()->SetRadius(500.f);

	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), L"Default");

	// Player Object 생성
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CStateScript);
	pParent->AddComponent(new CPlayerScript);

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	Ptr<CMaterial> Std2DMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl");

	// 이제는 화면을 기준으로 출력되므로 MDC좌표를 기준으로 설정했던 크기 및 좌표를 화면기준으로 바꿔줌
	pParent->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	pParent->MeshRender()->SetMesh(pMesh);
	pParent->MeshRender()->SetMaterial(Std2DMtrl);
	
	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(40.f, 60.f));
	
	SpawnGameObject(pParent, Vec3(1700.f, -1500.f, 500.f), L"Player");
	
	// 플레이어 발자국 효과
	CGameObject* pWalkParticle = new CGameObject;
	pWalkParticle->SetName(L"WalkEffect");
	pWalkParticle->AddComponent(new CTransform);
	pWalkParticle->AddComponent(new CParticleSystem(PARTICLE::PLAYER_WALK));
	pWalkParticle->AddComponent(new CWalkEffectScript);
	
	pWalkParticle->Transform()->SetRelativePos(0.f, -0.4f, 0.f);
	pWalkParticle->Transform()->SetRelativeScale(50.f, 50.f, 1.f);
	
	SpawnGameObject(pWalkParticle, Vec3(0.f, 0.f, 1.f), (int)LAYER::DEFAULT);
	
	// 플레이어 그림자
	CGameObject* pShadowParticle = new CGameObject;
	pShadowParticle->SetName(L"ShadowParticle");
	pShadowParticle->AddComponent(new CTransform);
	pShadowParticle->AddComponent(new CParticleSystem(PARTICLE::SHADOW));
	pShadowParticle->AddComponent(new CShadowScript);
	
	pShadowParticle->Transform()->SetRelativeScale(60.f, 60.f, 1.f);

	pParent->AddChild(pShadowParticle);
	

	// 플레이어 조명
	CGameObject* pPlyLight = new CGameObject;
	pPlyLight->SetName(L"LightEffect");
	pPlyLight->AddComponent(new CTransform);
	pPlyLight->AddComponent(new CLight2D);
	pPlyLight->AddComponent(new CFireLightScript);
	
	pPlyLight->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	
	pPlyLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pPlyLight->Light2D()->SetLightDiffuse(Vec3(5.f, 5.f, 5.f));
	pPlyLight->Light2D()->SetAngle(XM_PI / 2.f);
	pPlyLight->Light2D()->SetRadius(50.f);

	pParent->AddChild(pPlyLight);


	// 자식
	CGameObject* pWeapon = new CGameObject;
	
	pWeapon->SetName(L"Weapon");
	pWeapon->AddComponent(new CTransform);
	pWeapon->AddComponent(new CMeshRender);
	pWeapon->AddComponent(new CAnimator2D);
	pWeapon->AddComponent(new CStateScript);
	pWeapon->AddComponent(new CWeaponScript);
	pWeapon->Transform()->SetAbsolute(false);
	pWeapon->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	pWeapon->Transform()->SetRelativePos(1.f, 0.f, 0.f);
	
	pWeapon->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pWeapon->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	pParent->AddChild(pWeapon);

	// Shooting Effect
	CGameObject* pFireEffect = new CGameObject;
	
	pFireEffect->SetName(L"Effect");
	pFireEffect->AddComponent(new CTransform);
	pFireEffect->AddComponent(new CMeshRender);
	pFireEffect->AddComponent(new CAnimator2D);
	pFireEffect->AddComponent(new CEffectScript);
	pFireEffect->Transform()->SetRelativePos(1.f, 1.f, 1.f);
	pFireEffect->Transform()->SetRelativeScale(30.f, 30.f, 30.f);

	pFireEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pFireEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DLightMtrl"));

	pParent->AddChild(pFireEffect);
	

	CGameObject* pBoss = new CGameObject;
	
	pBoss->SetName(L"Gatling_Gull");
	pBoss->AddComponent(new CTransform);
	pBoss->AddComponent(new CMeshRender);
	pBoss->AddComponent(new CCollider2D);
	pBoss->AddComponent(new CAnimator2D);
	pBoss->AddComponent(new CStateScript);
	pBoss->AddComponent(new CBossScript);
	
	pBoss->Transform()->SetRelativeScale(400.f, 400.f, 1.f);
	
	pBoss->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pBoss->Collider2D()->SetOffsetScale(Vec2(0.4f, 0.4f));
	
	pBoss->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBoss->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DLightMtrl"));
	
	SpawnGameObject(pBoss, Vec3(300.f, 300.f, 100.f), (int)LAYER::MONSTER);
	


	//CGameObject* pBossHPUI = new CGameObject;
	//pBossHPUI->SetName(L"BossHPUI");
	//pBossHPUI->AddComponent(new CTransform);
	//pBossHPUI->AddComponent(new CMeshRender);
	//
	//pBossHPUI->Transform()->SetAbsolute(true);
	//pBossHPUI->Transform()->SetRelativeScale(600.f, 40.f, 1.f);
	//pBossHPUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pBossHPUI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	//pBossHPUI->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BOSSHP_GUIDE"));	
	//pBossHPUI->MeshRender()->GetDynamicMaterial();
	//SpawnGameObject(pBossHPUI, Vec3(0.f, -340.f, 5.f), (int)LAYER::UI);
	//
	//CGameObject* pBossHPbar = new CGameObject;
	//pBossHPbar->SetName(L"BossHPbar");
	//pBossHPbar->AddComponent(new CTransform);
	//pBossHPbar->AddComponent(new CMeshRender);
	//
	//pBossHPbar->Transform()->SetAbsolute(true);
	//pBossHPbar->Transform()->SetRelativeScale(495.f, 20.f, 1.f);
	//pBossHPbar->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pBossHPbar->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	//pBossHPbar->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BOSSHP_BAR"));
	//SpawnGameObject(pBossHPbar, Vec3(0.f, -340.f, 1.f), (int)LAYER::UI);
	// 
	//CGameObject* pHP = new CGameObject;
	//pHP->SetName(L"HP");
	//pHP->AddComponent(new CTransform);
	//pHP->AddComponent(new CMeshRender);
	//pHP->AddComponent(new CAnimator2D);
	//
	//pHP->Transform()->SetRelativeScale(80.f, 80.f, 1.f);
	//pHP->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pHP->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	//pHP->Animator2D()->LoadAnimFromFile(pHP->GetName());
	//pHP->Animator2D()->Play(L"HP", true);
	//
	//SpawnGameObject(pHP, Vec3(100.f, 200.f, 50.f), (int)LAYER::UI);
	//
	//CGameObject* pBlank = new CGameObject;
	//pBlank->SetName(L"Blank");
	//pBlank->AddComponent(new CTransform);
	//pBlank->AddComponent(new CMeshRender);
	//pBlank->AddComponent(new CAnimator2D);
	//
	//pBlank->Transform()->SetRelativeScale(60.f, 60.f, 1.f);
	//pBlank->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pBlank->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	//pBlank->Animator2D()->LoadAnimFromFile(pBlank->GetName());
	//pBlank->Animator2D()->Play(L"Blank", false);

	//SpawnGameObject(pBlank, Vec3(200.f, 200.f, 50.f), (int)LAYER::UI);


	//CGameObject* pUIMgr = new CGameObject;
	//pUIMgr->SetName(L"BossHP");
	//pUIMgr->AddComponent(new CTransform);
	//pUIMgr->AddComponent(new CBossHPScript);
	//
	//SpawnGameObject(pUIMgr, Vec3(0.f, 0.f, 1.f), (int)LAYER::DEFAULT);
	
	//CGameObject* ppUIMgr = new CGameObject;
	//ppUIMgr->SetName(L"WeaponUI");
	//ppUIMgr->AddComponent(new CTransform);
	//ppUIMgr->AddComponent(new CWeaponUIScript);
	//
	//SpawnGameObject(ppUIMgr, Vec3(0.f, 0.f, 1.f), (int)LAYER::DEFAULT);
	

	//CGameObject* pBullet_Hit = new CGameObject;
	//pBullet_Hit->SetName(L"Bullet_Die");
	//pBullet_Hit->AddComponent(new CTransform);
	//pBullet_Hit->AddComponent(new CMeshRender);
	//pBullet_Hit->AddComponent(new CAnimator2D);
	//
	//pBullet_Hit->Transform()->SetRelativeScale(30.f, 30.f, 1.f);
	//
	//pBullet_Hit->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pBullet_Hit->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DLightMtrl"));
	//pBullet_Hit->Animator2D()->LoadAnimFromFile(pBullet_Hit->GetName());
	//pBullet_Hit->Animator2D()->Play(L"Bullet_Die", false);
	//
	//SpawnGameObject(pBullet_Hit, Vec3(100.f, 100.f, 300.f), (int)LAYER::ITEM);

	//CGameObject* pFall = new CGameObject;
	//pFall->AddComponent(new CTransform);
	//pFall->AddComponent(new CCollider2D);
	//
	//pFall->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
	//
	//pFall->Collider2D()->SetOffsetScale(Vec2(2.f, 2.f));
	//SpawnGameObject(pFall, Vec3(0.f, -300.f, 200.f), (int)LAYER::FALLAREA);

		  // TileMap Object
	CGameObject* pTileMap = new CGameObject;
	pTileMap->SetName(L"Tile");	
	pTileMap->AddComponent(new CTransform);
	pTileMap->AddComponent(new CTileMap);	
	pTileMap->Transform()->SetRelativeScale(Vec3(4800.f, 4800.f, 1.f));
	  // 타일은 전용 Mesh 및 Mtrl을 사용하기 때문에 생성자에서 설정함.
	pTileMap->TileMap()->UseFrame(1.f);
	pTileMap->TileMap()->Load(L"Tile\\Tile.tile");
	SpawnGameObject(pTileMap, Vec3(0.f, 0.f, 600.f), L"Tile");

	CGameObject* pWallTile = new CGameObject;
	pWallTile->SetName(L"Wall");
	pWallTile->AddComponent(new CTransform);
	pWallTile->AddComponent(new CTileMap);
	pWallTile->Transform()->SetRelativeScale(Vec3(4800.f, 4800.f, 1.f));
	pWallTile->TileMap()->SetSliceSize(Vec2(0.1f, 0.1f));
	pWallTile->TileMap()->UseFrame(1.f);
	pWallTile->TileMap()->Load(L"Tile\\Wall.tile");

	SpawnGameObject(pWallTile, Vec3(0.f, 0.f, 200.f), L"Tile");
	//=================================================
	// Particle
	//=================================================
	
	CGameObject* pPet = new CGameObject;
	pPet->SetName(L"Pet");
	pPet->AddComponent(new CTransform);
	pPet->AddComponent(new CMeshRender);
	pPet->AddComponent(new CAnimator2D);
	pPet->AddComponent(new CStateScript);
	pPet->AddComponent(new CPetScript);

	// 이제는 화면을 기준으로 출력되므로 MDC좌표를 기준으로 설정했던 크기 및 좌표를 화면기준으로 바꿔줌
	pPet->Transform()->SetRelativeScale(Vec3(80.f, 80.f, 1.f));
	pPet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPet->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	SpawnGameObject(pPet, Vec3(400.f, 200.f, 500.f), 0);



	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::MONSTER);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::WALL);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTER, (int)LAYER::WALL);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTERPROJECTILE, (int)LAYER::WALL);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTERPROJECTILE, (int)LAYER::WALL);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYERPROJECTILE, (int)LAYER::MONSTER);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTERPROJECTILE, (int)LAYER::PLAYER);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYERPROJECTILE, (int)LAYER::ITEM);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTERPROJECTILE, (int)LAYER::ITEM);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTER, (int)LAYER::ITEM);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::ITEM);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::FALLAREA);
	CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::SPAWNBOX);

}
