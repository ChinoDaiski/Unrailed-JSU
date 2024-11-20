#include "stdafx.h"
#include "Material_Iron.h"
#include "GameInstance.h"

CMaterial_Iron::CMaterial_Iron(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	m_eType = OBJECT_TYPE::OBJECT_MATERIAL_IRON;
}

CMaterial_Iron::CMaterial_Iron(const CMaterial_Iron & rhs)
	: CGameObject(rhs)
{
	m_eType = rhs.m_eType;
}

HRESULT CMaterial_Iron::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMaterial_Iron::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMaterial_Iron::Tick(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	//// �浹�� �� ������Ʈ�� �����´�.
	CGameObject* pColObject = m_pColliderCom->Get_CollisionObject();
	CTransform*		PlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	_float3 PlayerPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);

	_float3 resevZ = m_pTransformCom->Get_State(CTransform::STATE_LOOK);


	if (m_isCol == FALSE) {

		// ���� �浹�� ������Ʈ�� �����Ѵٸ�
		if (nullptr != pColObject&& GetAsyncKeyState(VK_SPACE) & 0x8000) {


			//��ġ�� �÷��̾ �浹�� ���¿��� space Ű�� ������ ������� �÷��̾� �������� �ٽ� ���õȴ�. �����ϵ� �������־���.


			m_pTransformCom->Set_State(CTransform::STATE_POSITION, PlayerTransform->Get_State(CTransform::STATE_POSITION));
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, PlayerTransform->Get_State(CTransform::STATE_LOOK));
			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, PlayerTransform->Get_State(CTransform::STATE_RIGHT));
			m_pTransformCom->Set_State(CTransform::STATE_UP, PlayerTransform->Get_State(CTransform::STATE_UP));



			m_isCol = TRUE;
		}
	}
	else if (TRUE == m_isCol) {
		//�� ���� ��Ʈ������ �÷��̾� ���� ��Ʈ������ ���Ѵ�.
		//�װŸ� �ٽ� ������ �����Ѵ�.

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, PlayerTransform->Get_State(CTransform::STATE_POSITION));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, PlayerTransform->Get_State(CTransform::STATE_LOOK));
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, PlayerTransform->Get_State(CTransform::STATE_RIGHT));
		m_pTransformCom->Set_State(CTransform::STATE_UP, PlayerTransform->Get_State(CTransform::STATE_UP));


		_float4x4 playermatrix = PlayerTransform->Get_WorldMatrix();
		_float4x4 myMatrix = m_pTransformCom->Get_WorldMatrix();

		m_pTransformCom->Set_WorldMatrix(myMatrix*playermatrix);

		//������ �����ΰ�?
		m_pTransformCom->Scaled(_float3{ 0.03f, 0.03f, 0.03f });


		//��� �ִ� ���¿��� space ������ �浹 ó�� Ǯ���� ���ڸ��� ���̰� �ȴ�.
		if (GetAsyncKeyState(VK_SPACE))
		{
			m_isCol = false;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, PlayerTransform->Get_State(CTransform::STATE_POSITION));
			m_pTransformCom->Set_State(CTransform::STATE_LOOK, resevZ);
			m_pTransformCom->Set_State(CTransform::STATE_RIGHT, PlayerTransform->Get_State(CTransform::STATE_RIGHT));
			m_pTransformCom->Set_State(CTransform::STATE_UP, PlayerTransform->Get_State(CTransform::STATE_UP));


			//������ �����ΰ�?
			m_pTransformCom->Scaled(_float3{ 0.03f, 0.03f, 0.03f });

		}
	}

	__super::Tick(fTimeDelta);
}

void CMaterial_Iron::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CMaterial_Iron::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDevice()))
		return E_FAIL;

	// �������� �ִ� 3, 1�̵ǰ� ���� Ÿ�ֿ̹� �����ȴ�.
	auto& ObjectList = m_pVIBufferCom->Get_lstVIBuffer();

	for (auto& Object : ObjectList) {

		if (FAILED(m_pTextureCom->Bind_OnGraphicDevice(Object.second)))
			return E_FAIL;

		Object.first->Render();
	}

	return S_OK;
}

HRESULT CMaterial_Iron::SetUp_Components(void* pArg)
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


	TransDesc.fSpeedPerSec = 0.3f;
	TransDesc.fRotationPerSec = D3DXToRadian(10.0f);
	TransDesc.fScalePerSec = 1.f;

	// ����
	_float fScale = 40.f;

	// Material_Iron�� ������ ����
	TransDesc.m_WorldMatrix._11 /= fScale;
	TransDesc.m_WorldMatrix._12 /= fScale;
	TransDesc.m_WorldMatrix._13 /= fScale;
	TransDesc.m_WorldMatrix._21 /= fScale;
	TransDesc.m_WorldMatrix._22 /= fScale;
	TransDesc.m_WorldMatrix._23 /= fScale;
	TransDesc.m_WorldMatrix._31 /= fScale;
	TransDesc.m_WorldMatrix._32 /= fScale;
	TransDesc.m_WorldMatrix._33 /= fScale;

	memcpy(TransDesc.m_WorldMatrix.m[3], &vPos, sizeof(_float3));

	/* For.Com_Transform */
	if (FAILED(__super::SetUp_Components(Component_Transform, LEVEL_STATIC, Prototype_Compo_Transform, (CComponent**)&m_pTransformCom, &TransDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(Component_Renderer, LEVEL_STATIC, Prototype_Compo_Renderer, (CComponent**)&m_pRendererCom, nullptr)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(Component_VIBuffer, LEVEL_STATIC, Prototype_Compo_VIB_Object, (CComponent**)&m_pVIBufferCom, TEXT("Material_Iron.dat"))))
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

CMaterial_Iron * CMaterial_Iron::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMaterial_Iron*	pInstance = new CMaterial_Iron(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created Material_Iron"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMaterial_Iron::Clone(void * pArg)
{
	CMaterial_Iron*	pInstance = new CMaterial_Iron(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone Material_Iron"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMaterial_Iron::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}