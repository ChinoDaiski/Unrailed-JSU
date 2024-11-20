#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END
BEGIN(Client)
class MenuBtn final: public CGameObject
{
public:
	explicit MenuBtn(LPDIRECT3DDEVICE9 pGraphic_Device);
	MenuBtn(const MenuBtn& rhs);
	virtual ~MenuBtn()= default;

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
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	/* �� ������ � �ؽ��� �ø�����?! */
	CTexture*			m_pTextureCom = nullptr;

	/* �׸���!!! */
	CRenderer*			m_pRendererCom = nullptr;

private:
	_float				timecontrol = 0.f;	//Move��

	HRESULT SetUp_Components();

	_float4x4			m_ProjMatrix; //���� ����
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
public:

	HRESULT Ready_Layer_BackGround(const _tchar * pLayerTag);
public:
	static MenuBtn* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END