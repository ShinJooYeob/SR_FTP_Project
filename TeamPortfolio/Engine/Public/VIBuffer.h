#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract :public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() =default;

public:
	virtual HRESULT Render();
protected:
	virtual HRESULT Initialize_Protoype(void * pArg);
	virtual HRESULT Initialize_Clone(void * pArg);


protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	//Ŀ���� ���ؽ� ������ ũ��
	_uint							m_iStride = 0; 
	//������ ����
	_uint							m_iNumVertices = 0;
	//���ؽ� ����
	_ulong							m_dwFVF = 0;

	//�׸��� ���� (�ﰢ�� 3���� ������ �׸���)
	_D3DPRIMITIVETYPE				m_eDrawPrimitive;
	//�ﰢ���� ����
	_uint							m_iNumPrimitive = 0;


	//�ﰢ���� �׸��� �ε��� ����ü�� ������
	_uint							m_iIndicesSize = 0;
	//�ε��� �ϳ��� byte
	D3DFORMAT						m_eIndexFormat;

	void*							m_pVertices = nullptr;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg)PURE;
	virtual void Free() override;
};

END	