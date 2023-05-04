#include "pch.h"
#include "RigidbodyUI.h"


RigidbodyUI::RigidbodyUI()
	: ComponentUI("##Rigidbody", COMPONENT_TYPE::RIGIDBODY)
{
	SetName("Rigidbody");
}

RigidbodyUI::~RigidbodyUI()
{
}

int RigidbodyUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	return TRUE;
}