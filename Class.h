#include <d2d1.h>
#include <dwrite.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include <tlhelp32.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dwmapi")

using namespace D2D1;
using namespace std;

auto GameFile = "SoTGame.exe";
auto GameName = "Sea of Thieves";
auto ItemName = "Overlay";

DWORD_PTR GNames, UEngine, UViewport, UPlayer;
HANDLE Process;
RECT Frame;

ID2D1Factory* Factory;
ID2D1HwndRenderTarget* RenderTarget;
ID2D1SolidColorBrush* Brush;
IDWriteTextFormat* TextFormat;
IDWriteFactory* DWriteFactory;

auto Player = ColorF(ColorF::Firebrick, 1);
auto Ship = ColorF(ColorF::LimeGreen, 1);
auto Common = ColorF(ColorF::White, 1);
auto Rare = ColorF(ColorF::Blue, 1);
auto Legendary = ColorF(ColorF::Purple, 1);
auto Mythical = ColorF(ColorF::Orange, 1);
auto Cloud = ColorF(ColorF::SlateBlue, 1);
auto Crate = ColorF(ColorF::Silver, 1);
auto Enemy = ColorF(ColorF::Red, 1);

vector<DWORD> GNamesPattern = {0x48, 0x89, 0x1D, 0x00, 0x00, 0x00, 0x00, 0x8B, 0xC3};
vector<DWORD> UEnginePattern = {0x48, 0x8B, 0x35, 0x00, 0x00, 0x00, 0x00, 0x33, 0xDB};

DWORD_PTR FindPattern(DWORD_PTR Base, DWORD Size, vector<DWORD> Signature) {
	DWORD_PTR Module = Base + Size;
	size_t BytesRead;

	while (Base < Module) {
		BYTE Buffer[1024 * 64];
		ReadProcessMemory(Process, (LPVOID)Base, &Buffer, sizeof(Buffer), &BytesRead);

		if (!BytesRead)
			return 0;

		for (int i = 0; i < BytesRead; i++) {
			if (Buffer[i] != Signature[0])
				continue;

			bool Match = true;
			for (int j = 0; j < Signature.size(); j++) {
				if (!Signature[j])
					continue;

				if (Buffer[i + j] != Signature[j]) {
					Match = false;
					break;
				}
			}

			if (Match)
				return Base + i;
		}

		Base += BytesRead;
	}

	return 0;
}

template <class T, class U>
T Read(U Address) {
	T Buffer;
	ReadProcessMemory(Process, (LPVOID)Address, &Buffer, sizeof(Buffer), 0);
	return Buffer;
}

int Setup() {
	DWORD_PTR Base = 0, Address;
	DWORD PID = 0, Size = 0, Offset;

	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry);
	HANDLE pSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	while (Process32Next(pSnapshot, &ProcEntry)) {
		if (!lstrcmpi(ProcEntry.szExeFile, GameFile)) {
			PID = ProcEntry.th32ProcessID;
			break;
		}
	}

	CloseHandle(pSnapshot);

	if (!PID)
		return 1;

	MODULEENTRY32 ModEntry;
	ModEntry.dwSize = sizeof(ModEntry);
	HANDLE mSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	while (Module32Next(mSnapshot, &ModEntry)) {
		if (!lstrcmpi(ModEntry.szModule, GameFile)) {
			Base = (DWORD_PTR)ModEntry.modBaseAddr;
			Size = ModEntry.modBaseSize;
			break;
		}
	}

	CloseHandle(mSnapshot);
	Process = OpenProcess(MAXIMUM_ALLOWED, 0, PID);

	Address = FindPattern(Base, Size, GNamesPattern);
	Offset = Read<DWORD>(Address + 0x3);
	GNames = Read<DWORD_PTR>(Address + Offset + 0x7);

	Address = FindPattern(Base, Size, UEnginePattern);
	Offset = Read<DWORD>(Address + 0x3);
	UEngine = Read<DWORD_PTR>(Address + Offset + 0x7);

	return 0;
}

struct TArray {
	DWORD_PTR Data;
	int Count;
	int Max;
};

struct FNameEntry {
	int Index;
	FNameEntry* HashNext;
	char AnsiName[1024];
};

struct FString {
	wchar_t WideName[1024];
};

struct FVector {
	float X;
	float Y;
	float Z;

	FVector operator-(FVector V) {
		return FVector{X - V.X, Y - V.Y, Z - V.Z};
	}

	float Dot(FVector V) {
		return X * V.X + Y * V.Y + Z * V.Z;
	}
};

struct FRotator {
	float Pitch;
	float Yaw;
	float Roll;
};

struct FQuat {
	float X;
	float Y;
	float Z;
	float W;
};

struct FTransform {
	FQuat Rotation;
	FVector Translation;
	FVector Scale3D;
};

struct FMinimalViewInfo {
	FVector Location;
	FRotator Rotation;
	char Unknown[0x10];
	float FOV;
};

FVector WorldToScreen(FVector WorldLocation, FMinimalViewInfo Camera, float SizeX, float SizeY) {
	float PI = 3.14159265359f;

	auto CP = cos(Camera.Rotation.Pitch * PI / 180);
	auto SP = sin(Camera.Rotation.Pitch * PI / 180);
	auto CR = cos(Camera.Rotation.Roll * PI / 180);
	auto SR = sin(Camera.Rotation.Roll * PI / 180);
	auto CY = cos(Camera.Rotation.Yaw * PI / 180);
	auto SY = sin(Camera.Rotation.Yaw * PI / 180);

	auto X = FVector{CP * CY, CP * SY, SP};
	auto Y = FVector{SR * SP * CY - CR * SY, SR * SP * SY + CR * CY, -SR * CP};
	auto Z = FVector{-(CR * SP * CY + SR * SY), CY * SR - CR * SP * SY, CR * CP};

	auto Transform = FVector{WorldLocation.Dot(Y), WorldLocation.Dot(Z), WorldLocation.Dot(X)};
	if (Transform.Z < 1)
		Transform.Z = 1;

	FVector Location;
	Location.X = SizeX + Transform.X * (SizeX / tan(Camera.FOV * PI / 360)) / Transform.Z;
	Location.Y = SizeY - Transform.Y * (SizeX / tan(Camera.FOV * PI / 360)) / Transform.Z;

	return Location;
}