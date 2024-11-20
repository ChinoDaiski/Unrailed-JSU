#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Object;
END

BEGIN(Tool)

class CRock final : public CGameObject
{
public:
	typedef struct _tagRockDesc {
		CTransform::TRANSFORMDESC TransDesc;
		int iRockNumber;
	}ROCKDESC;

private:
	explicit CRock(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRock(const CRock& rhs);
	virtual ~CRock() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	/* �����δ�.(���¸� ǥ���Ѵ�.) */
	CTransform*			m_pTransformCom = nullptr;
	
	/* �������� �׸�����?! (��) */
	CVIBuffer_Object*	m_pVIBufferCom = nullptr;	

	/* �� ������ � �ؽ��� �ø�����?! */
	CTexture*			m_pTextureCom = nullptr;

	/* �׸���!!! */
	CRenderer*			m_pRendererCom = nullptr;

private:
	HRESULT SetUp_Components(void* pArg);

private:
	int			m_iLife;

public:
	// �������� 1 �پ���, VIBuffer�� ��ü�ȴ�.
	void Get_Damage(void);

public:
	static CRock* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END