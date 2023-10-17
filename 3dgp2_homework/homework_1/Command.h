#pragma once

class CGameObject;

enum class ButtonType
{
	BUTTON_MOVE_FORWARD,
	BUTTON_MOVE_BACKWARD,
	BUTTON_MOVE_RIGHT,
	BUTTON_MOVE_LEFT,
	BUTTON_MOVE_DOWN,
	BUTTON_MOVE_UP,
	BUTTON_CAMERA_ROTATE_Y,
	BUTTON_FIRE
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

class CUpCommand : public CBaseCommand
{
public:
	virtual ~CUpCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};

class CDownCommand : public CBaseCommand
{
public:
	virtual ~CDownCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};

class CCameraRotateYCommand : public CBaseCommand
{
public:
	virtual ~CCameraRotateYCommand() {}
	virtual void execute(CGameObject& gameObject) override;
};

class CFireCommand : public CBaseCommand
{
public:
	virtual ~CFireCommand() {}
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
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_FORWARD] = 'W';
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_BACKWARD] = 'S';
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_RIGHT] = 'D';
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_LEFT] = 'A';
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_UP] = VK_SPACE;
		m_mapButtonToKey[ButtonType::BUTTON_MOVE_DOWN] = VK_LSHIFT;
		m_mapButtonToKey[ButtonType::BUTTON_CAMERA_ROTATE_Y] = VK_LBUTTON;
		m_mapButtonToKey[ButtonType::BUTTON_FIRE] = VK_LCONTROL;

		button_forward = make_unique<CForwardCommand>();
		button_backward = make_unique<CBackwardCommand>();
		button_right = make_unique<CRightCommand>();
		button_left = make_unique<CLeftCommand>();
		button_up = make_unique<CUpCommand>();
		button_down = make_unique<CDownCommand>();
		button_camera_rotate_y = make_unique<CCameraRotateYCommand>();
		button_fire = make_unique<CFireCommand>();
	}

	virtual ~CPlayerInputHandler() {};
	virtual void HandleInput(CGameObject& gameObject) override;

private:
	std::unique_ptr<CBaseCommand> button_forward;
	std::unique_ptr<CBaseCommand> button_backward;
	std::unique_ptr<CBaseCommand> button_right;
	std::unique_ptr<CBaseCommand> button_left;
	std::unique_ptr<CBaseCommand> button_up;
	std::unique_ptr<CBaseCommand> button_down;
	std::unique_ptr<CBaseCommand> button_camera_rotate_y;
	std::unique_ptr<CBaseCommand> button_fire;
};
