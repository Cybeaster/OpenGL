#include "RendererInputHandler.hpp"

#include "Logging/Log.hpp"
#include "Renderer.hpp"

namespace RenderAPI
{

void ORendererInputHandler::OnWKeyToggled(bool Pressed)
{
	RAPI_LOG(Log, "On W KeyToggled called (Is pressed: {}), (Camera position is: {})", TO_STRING(Pressed), TO_STRING(Owner->GetRenderer()->GetCameraPosition()));

	if (Pressed)
	{
		Owner->MoveCamera(OVec3(0, 0, -1));
	}
}

void ORendererInputHandler::OnSKeyToggled(bool Pressed)
{
	RAPI_LOG(Log, "On S KeyToggled called (Is pressed: {}), (Camera position is: {})", TO_STRING(Pressed), TO_STRING(Owner->GetRenderer()->GetCameraPosition()));
	if (Pressed)
	{
		Owner->MoveCamera(OVec3(0, 0, 1));
	}
}

void ORendererInputHandler::OnDKeyToggled(bool Pressed)
{
	RAPI_LOG(Log, "On D KeyToggled called (Is pressed: {}), (Camera position is: {})", TO_STRING(Pressed), TO_STRING(Owner->GetRenderer()->GetCameraPosition()));
	if (Pressed)
	{
		Owner->MoveCamera(OVec3(1, 0, 0));
	}
}

void ORendererInputHandler::OnAKeyToggled(bool Pressed)
{
	RAPI_LOG(Log, "On A KeyToggled called (Is pressed: {}), (Camera position is: {})", TO_STRING(Pressed), TO_STRING(Owner->GetRenderer()->GetCameraPosition()));
	if (Pressed)
	{
		Owner->MoveCamera(OVec3(-1, 0, 0));
	}
}

void ORendererInputHandler::OnEKeyToggled(bool Pressed)
{
	RAPI_LOG(Log, "On E KeyToggled called (Is pressed: {}), (Camera position is: {})", TO_STRING(Pressed), TO_STRING(Owner->GetRenderer()->GetCameraPosition()));
	if (Pressed)
	{
		Owner->MoveCamera(OVec3(0, 1, 0));
	}
}

void ORendererInputHandler::OnQKeyToggled(bool Pressed)
{
	RAPI_LOG(Log, "On Q KeyToggled called (Is pressed: {}), (Camera position is: {})", TO_STRING(Pressed), TO_STRING(Owner->GetRenderer()->GetCameraPosition()));
	if (Pressed)
	{
		Owner->MoveCamera(OVec3(0, -1, 0));
	}
}

void ORendererInputHandler::BindKeys()
{
	Owner->AddRawListener(this, &ORendererInputHandler::OnWKeyToggled, EKeys::KEY_W);
	Owner->AddRawListener(this, &ORendererInputHandler::OnWKeyToggled, EKeys::KEY_W);
	Owner->AddRawListener(this, &ORendererInputHandler::OnAKeyToggled, EKeys::KEY_A);
	Owner->AddRawListener(this, &ORendererInputHandler::OnDKeyToggled, EKeys::KEY_D);
	Owner->AddRawListener(this, &ORendererInputHandler::OnSKeyToggled, EKeys::KEY_S);
	Owner->AddRawListener(this, &ORendererInputHandler::OnEKeyToggled, EKeys::KEY_E);
	Owner->AddRawListener(this, &ORendererInputHandler::OnQKeyToggled, EKeys::KEY_Q);
}

} // namespace RenderAPI
