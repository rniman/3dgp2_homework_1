#pragma once

class CGameObject;

enum class ButtonType
{
	BUTTON_MOVE_FORWARD,
	BUTTON_MOVE_BACKWARD,
	BUTTON_MOVE_RIGHT,
	BUTTON_MOVE_LEFT,
	BUTTON_CAMERA_ROTATE_Y
};

class CBaseCommand
{
public:
	virtual ~CBaseCommand() {}
	virtual void execute(CGameObject& gameObject) = 0;
};

class CForwardCommand : public CBaseCommand
{
public:
	virtual ~CForwardCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};

class CBackwardCommand : public CBaseCommand
{
public:
	virtual ~CBackwardCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};

class CRightCommand : public CBaseCommand
{
public:
	virtual ~CRightCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};

class CLeftCommand : public CBaseCommand
{
public:
	virtual ~CLeftCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};

class CCameraRotateYCommand : public CBaseCommand
{
public:
	virtual ~CCameraRotateYCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};


class CBaseInputHandler
{
public:
	CBaseInputHandler()
	{
	}

	virtual ~CBaseInputHandler()
	{
	}

	bool IsPressed(ButtonType buttonType);
	virtual void HandleInput(CGameObject& gameObject) = 0;

	void ChangeKey() {};

	// interface
	void SetHWND(HWND hWnd) { m_shWnd = hWnd; };
	static HWND GetHWND() { return m_shWnd; };

	// static member variable
	static UCHAR m_spKeysBuffer[256];
	static POINT m_sptOldCursorPos;
private:
	static HWND	 m_shWnd;

protected:
	std::unordered_map<ButtonType, WORD> m_mapButtonToKey;
};

class CPlayerInputHandler : public CBaseInputHandler
{
public:
	CPlayerInputHandler()
		: CBaseInputHandler()
	{
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_FORWARD] = VK_UP;
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_BACKWARD] = VK_DOWN;
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_RIGHT] = VK_RIGHT;
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_LEFT] = VK_LEFT;
		m_mapButtonToKey[ButtonType::BUTTON_CAMERA_ROTATE_Y] = VK_LBUTTON;

		button_forward = make_unique<CForwardCommand>();
		button_backward = make_unique<CBackwardCommand>();
		button_right = make_unique<CRightCommand>();
		button_left = make_unique<CLeftCommand>();
		button_camera_rotate_y = make_unique<CCameraRotateYCommand>();
	}

	virtual ~CPlayerInputHandler() {};
	virtual void HandleInput(CGameObject& gameObject) override;

private:
	std::unique_ptr<CBaseCommand> button_forward;
	std::unique_ptr<CBaseCommand> button_backward;
	std::unique_ptr<CBaseCommand> button_right;
	std::unique_ptr<CBaseCommand> button_left;
	std::unique_ptr<CBaseCommand> button_camera_rotate_y;
};
