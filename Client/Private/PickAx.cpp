#include "stdafx.h"
#include "PickAx.h"
#include "GameInstance.h"
#include"ToolUI.h"
CPickAx::CPickAx(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_PICKAX;
}

CPickAx::CPickAx(const CPickAx & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CPickAx::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))	
		return E_FAIL;

	return S_OK;
}

HRESULT CPickAx::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPickAx::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	if (deltaTime >= 3) {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		_float3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		ToolUI::TOOLDES tooldesc;


		tooldesc.tradesc.m_WorldMatrix.m[3][0] = vPos.x;
		tooldesc.tradesc.m_WorldMatrix.m[3][1] = vPos.y + 1;
		tooldesc.tradesc.m_WorldMatrix.m[3][2] = vPos.z;

		tooldesc.tradesc.m_WorldMatrix.m[0][0] = 0.7f;
		tooldesc.tradesc.m_WorldMatrix.m[1][1] = 1.f;
		tooldesc.tradesc.m_WorldMatrix.m[2][2] = 1.f;



		tooldesc.tk = ToolUI::TOOLKIND::PICKAXE;

		pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_PickAx_UI"), TEXT("Prototype_GameObject_ToolUI"), &tooldesc);

		deltaTime = 0.f;
	}deltaTime += fTimeDelta;

	__super::Tick(fTimeDelta);

}

void CPickAx::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPickAx::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
}

HRESULT CPickAx::SetUp_Components(void* pArg)
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransDesc;

	if (nullptr == pArg)
		return E_FAIL;
	else {
		TransDesc = *((CTransform::TRANSFORMDESC*)pArg);
	}
	_float3 vPos{};
	memcpy(&vPos, TransDesc.m_WorldMatrix.m[3], sizeof(_float3));
	vPos.y = 0.7f;

	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	D3DXMatrixRotationZ(&TransDesc.m_WorldMatrix, D3DXToRadian(90.f));

	// 배율
	_float fScale = 30.f;

	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;

	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;

	TransDesc.m_WorldMatrix._31 /= 25.f;
	TransDesc.m_WorldMatrix._32 /= 25.f;
	TransDesc.m_WorldMatrix._33 /= 25.f;

	memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	_float4x4 RotateY;
	D3DXMatrixRotationY(&RotateY, D3DXToRadian(90));

	TransDesc.m_WorldMatrix = RotateY * TransDesc.m_WorldMatrix;

	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("PickAxe.dat"))))
		return E_FAIL;

	// Tree의 스케일 조정
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(Component_Texture, LEVEL_STATIC, TEXT("Prototype_Component_Texture_HyunWoo"), (CComponent**)&m_pTextureCom, nullptr)))
		return E_FAIL;

	CCollider::ColliDesc colDesc;
	colDesc.pTransform = m_pTransformCom;

	/* For.Com_Collider */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Collider"), LEVEL_STATIC, TEXT("Prototype_Component_Collider"), (CComponent**)&m_pColliderCom, &colDesc)))
		return E_FAIL;

	return S_OK;
}

CPickAx * CPickAx::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPickAx*	pInstance = new CPickAx(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPickAx::Clone(void * pArg)
{
	CPickAx*	pInstance = new CPickAx(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickAx::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
