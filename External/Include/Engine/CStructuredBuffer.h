#pragma once
#include "CEntity.h"
class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    ComPtr<ID3D11Buffer>                m_SB_CPU_Read;
    ComPtr<ID3D11Buffer>                m_SB_CPU_Write;

    D3D11_BUFFER_DESC                   m_tDesc;

    UINT                                m_iElementSize;
    UINT                                m_iElementCount;

    SB_TYPE                             m_Type;
    bool                                m_bSysAccess;

    UINT                                m_iRecentRegisterNum;


public:
    void Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _Type, bool _bSysAccess, void* _pSysMem = nullptr);
    void SetData(void* _pDst, UINT _iSize = 0);
    void GetData(void* _pDst);

    // PIPELINE_STAGE를 따로 지정해주어야 함
    void UpdateData(UINT _iRegisterNum, UINT _iPipeLineStage);
    void UpdateData_CS(UINT _iRegisterNum, bool _IsShaderRes);

    void Clear();
    void Clear_CS(bool _IsShaderRes);

    UINT GetElementSize() { return m_iElementSize; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }



    CLONE_DISABLE(CStructuredBuffer);
public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

