#include "stdafx.h"
#include "Command.h"
#include "Object.h"
#include "Player.h"

void CForwardCommand::execute(CGameObject& gameObject)
{
	CPlayer* player = dynamic_cast<CPlayer*>(&gameObject);
	XMFLOAT3 shift = Vector3::Add(XMFLOAT3(0.0f, 0.0f, 0.0f), player->GetLook(), 10.25f);
	player->Move(shift, false);
}

void CBackwardCommand::execute(CGameObject& gameObject)
{
	CPlayer* player = dynamic_cast<CPlayer*>(&gameObject);
	XMFLOAT3 shift = Vector3::Add(XMFLOAT3(0.0f, 0.0f, 0.0f), player->GetLook(), -10.25f);
	player->Move(shift, false);
}

void CRightCommand::execute(CGameObject& gameObject)
{
	CPlayer* player = dynamic_cast<CPlayer*>(&gameObject);
	XMFLOAT3 shift = Vector3::Add(XMFLOAT3(0.0f, 0.0f, 0.0f), player->GetRight(), 10.25f);
	player->Move(shift, false);
}

void CLeftCommand::execute(CGameObject& gameObject)
{
	CPlayer* player = dynamic_cast<CPlayer*>(&gameObject);
	XMFLOAT3 shift = Vector3::Add(XMFLOAT3(0.0f, 0.0f, 0.0f), player->GetRight(), -10.25f);
	player->Move(shift, false);
}

void CUpCommand::execute(CGameObject& gameObject)
{
	CPlayer* player = dynamic_cast<CPlayer*>(&gameObject);
	XMFLOAT3 shift = Vector3::Add(XMFLOAT3(0.0f, 0.0f, 0.0f), player->GetUp(), 10.25f);
	player->Move(shift, false);
}

void CDownCommand::execute(CGameObject& gameObject)
{
	CPlayer* player = dynamic_cast<CPlayer*>(&gameObject);
	XMFLOAT3 shift = Vector3::Add(XMFLOAT3(0.0f, 0.0f, 0.0f), player->GetUp(), -10.25f);
	player->Move(shift, false);
}

void CCameraRotateYCommand::execute(CGameObject& gameObject)
{
	// 윈도우 창이 하나라면 필요없다.
	//if (GetCapture() != CBaseInputHandler::GetHWND())
	//	return;

	float deltaX, deltaY;

	POINT ptCursorPos;
	SetCursor(nullptr);
	GetCursorPos(&ptCursorPos);
	deltaX = (float)(ptCursorPos.x - CBaseInputHandler::m_sptOldCursorPos.x) / 3.0f;
	deltaY = (float)(ptCursorPos.y - CBaseInputHandler::m_sptOldCursorPos.y) / 3.0f;
	SetCursorPos(CBaseInputHandler::m_sptOldCursorPos.x, CBaseInputHandler::m_sptOldCursorPos.y);

	CPlayer* player = dynamic_cast<CPlayer*>(&gameObject);
	player->Rotate(deltaY, deltaX, 0.0f);
}

void CFireCommand::execute(CGameObject& gameObject)
{
	CHelicopterPlayer* player = dynamic_cast<CHelicopterPlayer*>(&gameObject);
	if (player->GetCoolTime() > 0.0f)
		return;

	player->SetCoolTime(5.0f);
	player->Fire();

	char buf[256];
	
	char err[] = "KeyDown Fire";
	sprintf_s(buf, sizeof(buf), "Debug: %s\n", err);
	OutputDebugStringA(buf);
	
}

UCHAR CBaseInputHandler::m_spKeysBuffer[] = {};
POINT CBaseInputHandler::m_sptOldCursorPos = { 0, 0 };
HWND CBaseInputHandler::m_shWnd;

bool CBaseInputHandler::IsPressed(ButtonType buttonType)
{
	if (CBaseInputHandler::m_spKeysBuffer[m_mapButtonToKey[buttonType]] & 0xF0)
	{
		return true;
	}
	return false;
}

void CPlayerInputHandler::HandleInput(CGameObject& gameObject)
{
	if (IsPressed(ButtonType::BUTTON_MOVE_FORWARD)) button_forward->execute(gameObject);
	if (IsPressed(ButtonType::BUTTON_MOVE_BACKWARD)) button_backward->execute(gameObject);
	if (IsPressed(ButtonType::BUTTON_MOVE_RIGHT)) button_right->execute(gameObject);
	if (IsPressed(ButtonType::BUTTON_MOVE_LEFT)) button_left->execute(gameObject);
	if (IsPressed(ButtonType::BUTTON_MOVE_UP)) button_up->execute(gameObject);
	if (IsPressed(ButtonType::BUTTON_MOVE_DOWN)) button_down->execute(gameObject);
	if (IsPressed(ButtonType::BUTTON_CAMERA_ROTATE_Y)) button_camera_rotate_y->execute(gameObject);
	if (IsPressed(ButtonType::BUTTON_FIRE)) button_fire->execute(gameObject);
}


