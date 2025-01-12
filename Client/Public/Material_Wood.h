#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
class CCollider;
END

BEGIN(Client)

class CMaterial_Wood final : public CGameObject {
private:
	explicit CMaterial_Wood(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMaterial_Wood(const CMaterial_Wood& rhs);
	virtual ~CMaterial_Wood() = default;

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
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;	

	/* 저 모델위에 어떤 텍스쳐 올릴건지?! */
	CTexture*			m_pTextureCom = nullptr;

	/* 그린다!!! */
	CRenderer*			m_pRendererCom = nullptr;
	
	//콜라이더
	CCollider*			m_pColliderCom = nullptr;
	

public:
	void Stack();
private:
	bool m_isCol = false;
	HRESULT SetUp_Components(void* pArg);
	_float deltaTime = 0.f;
public:
	static CMaterial_Wood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END