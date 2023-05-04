#include "pch.h"
#include "CScript.h"


CScript::CScript(UINT _iScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptType(_iScriptType)
	, m_vecParam{}
{
}

CScript::~CScript()
{
}

// ScriptUI에 노출되고 싶은 Param 추가
void CScript::AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc)
{
	m_vecParam.push_back(tScriptParam{ eParam, _pData, _Desc });
}