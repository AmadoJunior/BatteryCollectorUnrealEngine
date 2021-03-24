#pragma once

//Core
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

//Main Components
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//Widget
#include "Blueprint/UserWidget.h"

//Generated
#include "MyCharacter.generated.h"

UCLASS()
class VSMAN_API AMyCharacter : public ACharacter{
	GENERATED_BODY()

public:
	//Constructor
	AMyCharacter();

	//Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;	

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	bool  bDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Power;

	UPROPERTY(EditAnywhere)
		float PowerThreshold;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
							class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Player_Power_Widget_Class;
		UUserWidget* Player_Power_Widget;

	void RestartGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
