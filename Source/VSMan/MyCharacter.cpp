#include "MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCharacter::AMyCharacter(){
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//Character Rotation Disabled
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	//Initiating/Attaching CameraBoom to RootComponent (Capsule, This Component is a Child of Capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	//Setting Arm Length and Enabling Arm Rotation
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	//Initiating/Attaching FollowCamera to CameraBoom and Disabling Rotation
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bDead = false;
	Power = 100.0f;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay(){
	Super::BeginPlay();
	//Attach OnBeginOverlap Method
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnBeginOverlap);
	//Check if Widget Attached
	if (Player_Power_Widget_Class != nullptr) {
		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	Power -= DeltaTime * PowerThreshold;

	if (Power <= 0) {
		if (!bDead) {
			bDead = true;
			//Ragdoll
			GetMesh()->SetSimulatePhysics(true);
			//Retart Game
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
				UnusedHandle, this, &AMyCharacter::RestartGame, 3.0f, false
			);
		}
	}
}

//Called to Bind Functionality to Input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Player Input SetUp -> (Project Settings -> Input)
	//Uses "Turn" Input to Control Yaw Rotation
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//Uses "LookUp" Input to Control Pitch Rotation
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Binding Jump 
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Binding to my Methods
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
}

void AMyCharacter::MoveForward(float Axis){
	//If Not Dead
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Calculates Forward Direction of this Vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::MoveRight(float Axis){
	//If Not Dead
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//Calculates Forward Direction of this Vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, 
								  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
								  bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->ActorHasTag("Recharge")) {
		/*UE_LOG(LogTemp, Warning, TEXT("Collided With"));*/
		Power += 50.0f;
		if (Power > 100.0f) {
			Power = 100.0f;
		}
		OtherActor->Destroy();
	}
}

void AMyCharacter::RestartGame() {
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}