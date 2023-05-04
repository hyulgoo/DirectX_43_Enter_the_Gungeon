#include "pch.h"
#include "SoundUI.h"
#include <Engine/CSound.h>

SoundUI::SoundUI()
    : ResUI(RES_TYPE::SOUND)
{
    SetName("Sound");
}

SoundUI::~SoundUI()
{
}

int SoundUI::render_update()
{
    ResUI::render_update();

    if (ImGui::Button("PLAY", ImVec2(60.f, 20.f)))
    {
        Ptr<CSound> pSound = (CSound*)GetTargetRes().Get();
        pSound->Play(1);
    }
    if (ImGui::Button("STOP", ImVec2(60.f, 20.f)))
    {
        Ptr<CSound> pSound = (CSound*)GetTargetRes().Get();
        pSound->Stop();
    }

    return 0;
}
