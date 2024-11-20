#include "stdafx.h"
#include "Train_Water.h"
#include "GameInstance.h"
#include "Effect_Explo.h"
#include "Train_Head.h"
#include "ToolUI.h"
#include "SoundMgr.h"
CTrain_Water::CTrain_Water(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTrain(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_TRAIN_WATER;
}

CTrain_Water::CTrain_Water(const CTrain_Water & rhs)
	: CTrain(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CTrain_Water::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrain_Water::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_TrainComponents(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTrain_Water::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	if (!m_bMove) {
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Train"));

		if (nullptr != pLayer) {
			list<CGameObject*> ObjectList = pLayer->Get_ObjectList();
			for (auto& Object : ObjectList) {
				if (Object->Get_Type() == OBJECT_TYPE::OBJECT_TRAIN_HEAD) {
					m_bMove = dynamic_cast<CTrain_Head*>(Object)->bMove();
				}
			}
		}
	}

	if (m_bMove) {
		Follow_Rail(fTimeDelta);
	}

	time += fTimeDelta;



	//기차가 배치되고 대락 3초뒤.
	if (time > 30.f) {


		if (time > 35.f) {
			if (FAILED(SetUp_VIBuffer(TEXT("Train_Water2.dat"))))
				return;
		}
		if (time > 60.f) {
			if (FAILED(SetUp_VIBuffer(TEXT("Train_Water3.dat"))))
				return;
		}

		if (time > 90.f)
		{
			if (FAILED(SetUp_VIBuffer(TEXT("Train_Water4.dat"))))
				return;


			//TOOLUI
			{
				_float3 pColPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				
				ToolUI::TOOLDES tooldesc;
				
				tooldesc.tradesc.m_WorldMatrix.m[3][0] = pColPosition.x;
				tooldesc.tradesc.m_WorldMatrix.m[3][1] = pColPosition.y + 2;
				tooldesc.tradesc.m_WorldMatrix.m[3][2] = pColPosition.z;

				tooldesc.tradesc.m_WorldMatrix.m[0][0] = 0.5f;
				tooldesc.tradesc.m_WorldMatrix.m[1][1] = 0.5f;
				tooldesc.tradesc.m_WorldMatrix.m[2][2] = 0.5f;

				tooldesc.tk = ToolUI::TOOLKIND::WARNING;

				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_MaterialTrainUI"), TEXT("Prototype_GameObject_ToolUI"), &tooldesc);

			}

			CEffect_Explo::EXPLODESC	ExploDesc;
			ExploDesc.iTextureNumber = 43;
			ExploDesc.TransDesc.m_WorldMatrix.m[3][0] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).x;
			ExploDesc.TransDesc.m_WorldMatrix.m[3][1] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).y;
			ExploDesc.TransDesc.m_WorldMatrix.m[3][2] = m_pTransformCom->Get_State(CTransform::STATE_POSITION).z;
			for (_uint i = 0; i < 30; i++)
			{
				pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Effect_Explo"), TEXT("Prototype_GameObject_Effect_Explo"), &ExploDesc.TransDesc);
			}
			
			CSoundMgr::Get_Instance()->PlaySound(TEXT("Burning.mp3"), CSoundMgr::CHANNELID::TRAINFIRE, SOUND_DEFAULT);

		}
		time = 0.f;

	}
}

void CTrain_Water::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CTrain_Water::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTrain_Water::FireGone(bool isFire)
{
	if (isFire) {
		if (FAILED(SetUp_VIBuffer(TEXT("Train_Water.dat"))))
			return;
	}
}

HRESULT CTrain_Water::SetUp_TrainComponents(void * pArg)
{
	if (FAILED(__super::SetUp_TrainComponents(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_VIBuffer(TEXT("Train_Water.dat"))))
		return E_FAIL;

	return S_OK;
}

CTrain_Water * CTrain_Water::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTrain_Water*	pInstance = new CTrain_Water(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Train_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrain_Water::Clone(void * pArg)
{
	CTrain_Water*	pInstance = new CTrain_Water(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Train_Water"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrain_Water::Free()
{
	__super::Free();
}
