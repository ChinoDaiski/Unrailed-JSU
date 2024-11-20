#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
class CCollider;
END

BEGIN(Client)

class CCube_Water final : public CGameObject
{
private:
	explicit CCube_Water(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCube_Water(const CCube_Water& rhs);
	virtual ~CCube_Water() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	/* 움직인다.(상태를 표현한다.) */
	CTransform*			m_pTransformCom = nullptr;
	
	/* 어떤모양으로 그릴건지?! (모델) */
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;

	// 충돌체 설정
	CCollider*			m_pColliderCom = nullptr;

private:
	HRESULT SetUp_Components(void * pArg);

public:
	static CCube_Water* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END