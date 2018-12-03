class Offsets {
public:
	// :APlayerController:PlayerCameraManager:SoT_Engine_classes.hpp
	static constexpr int PlayerCameraManager = 0x0520;

	// :APlayerCameraManager:CameraCache:SoT_Engine_classes.hpp
	static constexpr int CameraLocation = 0x0510;
	static constexpr int CameraRotation = 0x051C;

	// :APlayerCameraManager:POV.FOV:SoT_Engine_classes.hpp
	static constexpr int FOV = 0x0538;

	// :UPlayer:PlayerController:SoT_Engine_classes.hpp
	static constexpr int PlayerController = 0x0030;

	// :AActor:RootComponent:SoT_Engine_classes.hpp
	static constexpr int RootComponent = 0x0170;

	// :APawn:PlayerState:SoT_Engine_classes.hpp
	static constexpr int PlayerState = 0x04B0;

	// :AController:Pawn:SoT_Engine_classes.hpp
	static constexpr int Pawn = 0x0498;

	// :APlayerState:PlayerName:SoT_Engine_classes.hpp
	static constexpr int PlayerName = 0x0498;

	/* -------------------------------------------------------------------- */

	// UObject
	static constexpr int Id = 0x0018;

	// :UWorld:PersistentLevel:SoT_Engine_classes.hpp
	static constexpr int PersistentLevel = 0x0030;
	static constexpr int ActorsTArray = 0x00A0;
	static constexpr int ActorsTArrayCount = 0x00A8;

	// :UWorld:OwningGameInstance:SoT_Engine_classes.hpp
	static constexpr int OwningGameInstance = 0x01C0;

	// :UGameInstance:LocalPlayers:SoT_Engine_classes.hpp
	static constexpr int LocalPlayers = 0x0038;

	// :USceneComponent:RelativeLocation:SoT_Engine_classes.hpp
	static constexpr int RelativeLocation = 0x01C0;
};