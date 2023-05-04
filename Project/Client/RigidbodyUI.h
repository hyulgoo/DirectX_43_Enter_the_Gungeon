#pragma once
#include "ComponentUI.h"

class RigidbodyUI :
	public ComponentUI	
{
private:

public:
	virtual int render_update() override;

public:
	RigidbodyUI();
	~RigidbodyUI();


};
