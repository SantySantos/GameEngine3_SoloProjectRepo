// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TP_ThirdPersonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;



	/** Dash Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = "Dashing")
	float DashForce;
	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category = "Dashing")
	bool CanDash;
	
	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category = "Dashing")
	float DashLength;

	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category = "Dashing")
	float DashDelayLength;

	
	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category = "Dashing")
	float OriginalFallingLateralFriction;
	
	UPROPERTY(EditAnywhere, Category = "Dashing")
	FTimerHandle TimerHandle_DashLength;

	UPROPERTY(EditAnywhere, Category = "Dashing")
	FTimerHandle TimerHandle_ResetDash;

	//UPROPERTY(VisibleAnywhere, Category = "Dashing")
	//UNiagaraComponent* DashEffect;
	
	
public:
	
	/** Constructor */
	ATP_ThirdPersonCharacter();
	
	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Movement")
	bool bInTurret;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);


public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();


	//example of doxigen style comment / documentation formatting
	/**
	 * @brief 
	 * @param a = input integer for force 
	* @return 
	*/
	int MyFnc(int a);
	
	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Handles dash inputs from either controls or UI interfaces
	 * @note Dashing is a simple forward impulse in the direction the character is facing
	 */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoDash();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void ResetDash();
	
	UFUNCTION(BlueprintCallable, Category="Dashing")
	virtual void EnableDash();
	
public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

