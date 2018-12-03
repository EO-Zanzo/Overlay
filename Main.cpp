#include "Functions.h"

ID2D1Factory *pFactory;
ID2D1HwndRenderTarget *pRenderTarget;
ID2D1SolidColorBrush* pBrush;
IDWriteTextFormat* pTextFormat;
IDWriteFactory* pDWriteFactory;
Process Read;

int Render() {
	pRenderTarget->BeginDraw();
	pRenderTarget->Clear();

	tWnd = FindWindow(0, tName);
	if (tWnd) {
		GetWindowRect(tWnd, &tWindow);
		MoveWindow(hWnd, tWindow.left, tWindow.top, tWindow.right - tWindow.left, tWindow.bottom - tWindow.top, true);
		pRenderTarget->Resize(D2D1::SizeU(tWindow.right, tWindow.bottom));

		if (!Init) {
			ReadData();
			Init = true;
		}

		auto Names = Read.Memory<DWORD_PTR>(GNames);
		auto World = Read.Memory<DWORD_PTR>(UWorld);
		auto OwningGameInstance = Read.Memory<DWORD_PTR>(World + Offsets::OwningGameInstance);
		auto LocalPlayers = Read.Memory<DWORD_PTR>(OwningGameInstance + Offsets::LocalPlayers);
		auto ULocalPlayer = Read.Memory<DWORD_PTR>(LocalPlayers);
		auto PlayerController = Read.Memory<DWORD_PTR>(ULocalPlayer + Offsets::PlayerController);
		auto Pawn = Read.Memory<DWORD_PTR>(PlayerController + Offsets::Pawn);
		auto CameraManager = Read.Memory<DWORD_PTR>(PlayerController + Offsets::PlayerCameraManager);
		auto RootComponent = Read.Memory<DWORD_PTR>(Pawn + Offsets::RootComponent);
		auto ULevel = Read.Memory<DWORD_PTR>(World + Offsets::PersistentLevel);
		auto ActorCount = Read.Memory<DWORD>(ULevel + Offsets::ActorsTArrayCount);

		for (DWORD i = 0; i < ActorCount; i++) {
			auto ActorList = Read.Memory<DWORD_PTR>(ULevel + Offsets::ActorsTArray);
			auto Actor = Read.Memory<DWORD_PTR>(ActorList + (i * 0x8));

			if (!Actor)
				continue;

			auto ActorID = Read.Memory<DWORD>(Actor + Offsets::Id);
			auto ActorNamePTR = Read.Memory<DWORD_PTR>(Names + (ActorID / 0x4000) * 0x8);
			auto ActorName = Read.Memory<DWORD_PTR>(ActorNamePTR + 0x8 * (ActorID % 0x4000));
			auto ActorRootComponent = Read.Memory<DWORD_PTR>(Actor + Offsets::RootComponent);
			auto ActorRelativeLocation = Read.Memory<Vector3>(ActorRootComponent + Offsets::RelativeLocation);
			Name = Read.Memory<Text>(ActorName + 16).word;

			if (Name.find("BP_") == -1)
				continue;

			Term = Name;
			Type = Common;

			// Debug
			if (Debug) {
			// Player
			} else if (Name.find("BP_PlayerPirate_C") != -1) {
				auto PlayerState = Read.Memory<DWORD_PTR>(Actor + Offsets::PlayerState);
				auto PlayerNamePTR = Read.Memory<DWORD_PTR>(PlayerState + Offsets::PlayerName);
				auto PlayerName = Read.Memory<TextX>(PlayerNamePTR);
				wstring PlayerNameWide = PlayerName.word;
				string PlayerString(PlayerNameWide.begin(), PlayerNameWide.end());

				Term = PlayerString;
				Type = Player;
			// Ships
			} else if (Name.find("BP_SmallShipNetProxy") != -1 || Name.find("BP_MediumShipNetProxy") != -1 || Name.find("BP_LargeShipNetProxy") != -1) {
				Term = "Ship";
				Type = Ship;
			} else if (Name.find("BP_SmallShipTemplate") != -1 || Name.find("BP_MediumShipTemplate") != -1 || Name.find("BP_LargeShipTemplate") != -1) {
				Term = "Ship";
				Type = Ship;
			// Shipwrecks
			} else if (Name.find("BP_Shipwreck_01_a_NetProxy") != -1) {
				Term = "Shipwreck";
				Type = Crate;
			// Chests
			} else if (Name.find("TreasureChest") != -1 && Name.find("ItemInfo") == -1) {
				if (Name.find("Common") != -1) {
					Term = "Castaways Chest";
					Type = Common;
					if (Name.find("DVR") != -1)
						Term = "Ashen Castaways Chest";
				} else if (Name.find("Rare") != -1) {
					Term = "Seafarers Chest";
					Type = Rare;
					if (Name.find("DVR") != -1)
						Term = "Ashen Seafarers Chest";
				} else if (Name.find("Legendary") != -1) {
					Term = "Marauders Chest";
					Type = Legendary;
					if (Name.find("DVR") != -1)
						Term = "Ashen Marauders Chest";
				} else if (Name.find("Mythical") != -1) {
					Term = "Captains Chest";
					Type = Mythical;
					if (Name.find("DVR") != -1)
						Term = "Ashen Captains Chest";
				} else if (Name.find("PirateLegend") != -1) {
					Term = "Chest of Legends";
					Type = Mythical;
					if (Name.find("DVR") != -1)
						Term = "Ashen Chest of Legends";
				} else if (Name.find("Drunken") != -1) {
					Term = "Chest of Thousand Grogs";
					Type = Mythical;
				} else if (Name.find("Weeping") != -1) {
					Term = "Chest of Sorrow";
					Type = Mythical;
				} else if (Name.find("Fort") != -1) {
					Term = "Stronghold Chest";
					Type = Mythical;
				}
			// Skulls
			} else if (Name.find("BountyRewardSkull") != -1 && Name.find("ItemInfo") == -1) {
				if (Name.find("Common") != -1) {
					Name = "Foul Skull";
					Type = Common;
					if (Name.find("DVR") != -1)
						Name = "Ashen Foul Skull";
				} else if (Name.find("Rare") != -1) {
					Name = "Disgraced Skull";
					Type = Rare;
					if (Name.find("DVR") != -1)
						Name = "Ashen Disgraced Skull";
				} else if (Name.find("Legendary") != -1) {
					Name = "Hateful Skull";
					Type = Legendary;
					if (Name.find("DVR") != -1)
						Name = "Ashen Hateful Skull";
				} else if (Name.find("Mythical") != -1) {
					Name = "Villainous Skull";
					Type = Mythical;
					if (Name.find("DVR") != -1)
						Name = "Ashen Villainous Skull";
				} else if (Name.find("Fort") != -1) {
					Name = "Stronghold Skull";
					Type = Mythical;
				}
			// Skeleton
			} else if (Name.find("Skeleton") != -1 && Name.find("Pawn") != -1) {
				Term = "Skeleton";
				Type = Enemy;
			// Skeleton Fort
			} else if (Name.find("SkellyFort") != -1) {
				Type = Cloud;
				Term = "Skeleton Fort";
			// Chickens
			} else if (Misc && Name.find("BP_Chicken") != -1) {
				if (Name.find("Common") != -1) {
					Term = "White Chicken";
					Type = Common;
				} else if (Name.find("Rare") != -1) {
					Term = "Red Speckled Chicken";
					Type = Rare;
				} else if (Name.find("Legendary") != -1) {
					Term = "Black Plumed Chicken";
					Type = Legendary;
				} else if (Name.find("Mythical") != -1) {
					Term = "Golden Chicken";
					Type = Mythical;
				}
			// Pigs
			} else if (Misc && Name.find("BP_Pig") != -1) {
				if (Name.find("Common") != -1) {
					Term = "Pink Pig";
					Type = Common;
				} else if (Name.find("Rare") != -1) {
					Term = "Black Spotted Pig";
					Type = Rare;
				} else if (Name.find("Legendary") != -1) {
					Term = "Black Coated Pig";
					Type = Legendary;
				} else if (Name.find("Mythical") != -1) {
					Term = "Gold Striped Pig";
					Type = Mythical;
				}
			// Snakes
			} else if (Misc && Name.find("BP_Snake") != -1) {
				if (Name.find("Common") != -1) {
					Term = "Red Striped Snake";
					Type = Common;
				} else if (Name.find("Rare") != -1) {
					Term = "Blue Dappled Snake";
					Type = Rare;
				} else if (Name.find("Legendary") != -1) {
					Term = "Black Scaled Snake";
					Type = Legendary;
				} else if (Name.find("Mythical") != -1) {
					Term = "Golden Snake";
					Type = Mythical;
				}
			// Journal
			} else if (Name.find("BP_LoreBook") != -1) {
				Term = "Journal";
				Type = Common;
			// Rowboat
			} else if (Name.find("BP_Rowboat_C") != -1) {
				Term = "Rowboat";
				Type = Common;
			// Mermaid
			} else if (Name.find("BP_Mermaid_C") != -1) {
				Term = "Mermaid";
				Type = Common;
			// Shark
			} else if (Name.find("BP_Shark_C") != -1) {
				Term = "Shark";
				Type = Enemy;
			// Megalodon
			} else if (Name.find("BP_TinyShark_C") != -1) {
				Term = "Megalodon";
				Type = Enemy;
			} else {
				continue;
			}

			auto RelativeLocation = Read.Memory<Vector3>(RootComponent + Offsets::RelativeLocation);
			CameraRotation = Read.Memory<Vector3>(CameraManager + Offsets::CameraRotation);
			CameraLocation = Read.Memory<Vector3>(CameraManager + Offsets::CameraLocation);
			FOV = Read.Memory<float>(CameraManager + Offsets::FOV);

			float Distance = Vector2(RelativeLocation.x, RelativeLocation.y).DistTo(Vector2(ActorRelativeLocation.x, ActorRelativeLocation.y)) / 100;
			Term += " [" + std::to_string((int)Distance) + "m]";
			wstring NameWide(Term.begin(), Term.end());

			if (WorldToScreen(ActorRelativeLocation, &ScreenPoint, (float)hWindow.right, (float)hWindow.bottom)) {
				pRenderTarget->CreateSolidColorBrush(Type, &pBrush);
				pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(ScreenPoint.x, ScreenPoint.y));
				pRenderTarget->DrawText(NameWide.c_str(), (DWORD)NameWide.size(), pTextFormat, D2D1::RectF((float)hWindow.right, (float)hWindow.bottom, 0, 0), pBrush);
			}
		}
	}

	pRenderTarget->EndDraw();

	// F5 Misc
	if (GetAsyncKeyState(VK_F5) & 1) {
		if (Misc)
			Misc = false;
		else
			Misc = true;
	// F8 Debug
	} else if (GetAsyncKeyState(VK_F8) & 1) {
		if (Debug)
			Debug = false;
		else
			Debug = true;
	// F12 Exit
	} else if (GetAsyncKeyState(VK_F12) & 1) {
		exit(1);
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_CREATE:
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
			GetClientRect(Window, &hWindow);
			pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(Window, D2D1::SizeU(hWindow.right, hWindow.bottom)), &pRenderTarget);
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), reinterpret_cast<IUnknown **>(&pDWriteFactory));
			pDWriteFactory->CreateTextFormat(L"Arial", 0, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12, L"", &pTextFormat);
			return 0;

		case WM_PAINT:
			Render();
			return 0;

		case WM_DESTROY:
			exit(1);
	}

	return DefWindowProc(Window, Message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT) {
	MSG Message;
	WNDCLASS wclass = {CS_HREDRAW | CS_VREDRAW, WinProc, 0, 0, hInstance, 0, LoadCursor(0, IDC_ARROW), 0, 0, hName};
	RegisterClass(&wclass);

	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, hName, hName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, 0);
	SetLayeredWindowAttributes(hWnd, 0, 0, LWA_COLORKEY);
	ShowWindow(hWnd, SW_SHOW);

	while (GetMessage(&Message, hWnd, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return 0;
}