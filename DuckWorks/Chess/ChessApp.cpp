#include <Chess/ChessApp.h>

#include <Chess/ChessBoard.h>

#include <External/imgui/imgui.h>

#include <iostream>

using namespace DC;

void ChessApp::sRegisterRTTI()
{
	REGISTER_RTTI(ChessBoard);
}

ChessApp::ChessApp() 
{

}

void ChessApp::Update(float inDeltaTime) 
{

}
