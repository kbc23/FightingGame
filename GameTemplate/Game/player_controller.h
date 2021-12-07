#pragma once

struct StPlayerStatus;

class PlayerController : public IGameObject
{
public:
    PlayerController();
    ~PlayerController();
    bool Start() override final;
    void Update() override final;

    void Init(GamePad& gamePad, StPlayerStatus& playerStatus);

    void ControllerButton();

    const Vector3& ControllerLStick();

    const Vector2& ControllerRStick();


private: // data member
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // ���̃N���X���玝���Ă����C���X�^���X
    //////////////////////////////
    GamePad* m_gamePad = nullptr; // �Q�[���p�b�h
    StPlayerStatus* m_getStPlayerStatus = nullptr;
};