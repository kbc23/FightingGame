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

    const Vector3 ControllerLStick();

    const Vector2 ControllerRStick();


private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // 他のクラスから持ってきたインスタンス
    //////////////////////////////
    GamePad* m_gamePad = nullptr; // ゲームパッド
    StPlayerStatus* m_getStPlayerStatus = nullptr;
};