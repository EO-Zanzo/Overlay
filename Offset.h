// UObject:FName
auto ComparisonIndex = 0x0018;

/* ---------------------------------*/

// UEngine:UGameViewportClient*
auto ViewportClient = 0x0640;

// UGameViewportClient:UWorld*
auto World = 0x0080;

// UWorld:ULevel*
auto PersistentLevel = 0x0030;

// ULevel:TArray<AActor*>
auto LevelArray = 0x00A0;

/* ---------------------------------*/

// UGameViewportClient:UGameInstance*
auto GameInstance = 0x0088;

// UGameInstance:TArray<ULocalPlayer*>
auto LocalPlayers = 0x0038;

// UPlayer:AController*
auto PlayerController = 0x0030;

// AController:APawn*
auto Pawn = 0x0498;

/* ---------------------------------*/

// APawn:APlayerState*
auto PlayerState = 0x04B0;

// APlayerState:FString
auto PlayerName = 0x0498;

/* ---------------------------------*/

// APawn:USceneComponent*
auto RootComponent = 0x0170;

// USceneComponent:FTransform
auto ComponentToWorld = 0x0130;

/* ---------------------------------*/

// APlayerController:APlayerCameraManager*
auto PlayerCameraManager = 0x0520;

// APlayerCameraManager:FCameraCacheEntry
auto CameraCache = 0x0500;

// FCameraCacheEntry:FMinimalViewInfo
auto POV = 0x0010;