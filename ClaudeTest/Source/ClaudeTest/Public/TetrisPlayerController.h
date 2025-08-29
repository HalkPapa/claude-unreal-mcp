#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TetrisPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ATetrisGameMode;

UCLASS(BlueprintType, Blueprintable)
class CLAUDETEST_API ATetrisPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATetrisPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Enhanced Input関連
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* HardDropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RestartAction;

	// GameModeへの参照
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	ATetrisGameMode* TetrisGameMode;

	// 入力設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	float RepeatDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Settings")
	float RepeatRate;

	// リピート入力管理
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bIsMovingLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bIsMovingRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
	bool bIsMovingDown;

	// タイマー
	float LeftMoveTimer;
	float RightMoveTimer;
	float DownMoveTimer;

public:
	// 入力処理関数
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveLeft(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveRight(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveDown(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnRotate(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnHardDrop(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPause(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnRestart(const FInputActionValue& Value);

	// 入力開始/終了
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveLeftStarted(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveLeftCompleted(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveRightStarted(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveRightCompleted(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveDownStarted(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnMoveDownCompleted(const FInputActionValue& Value);

	// 設定関数
	UFUNCTION(BlueprintCallable, Category = "Input Settings")
	void SetRepeatDelay(float NewDelay) { RepeatDelay = NewDelay; }

	UFUNCTION(BlueprintCallable, Category = "Input Settings")
	void SetRepeatRate(float NewRate) { RepeatRate = NewRate; }

	UFUNCTION(BlueprintCallable, Category = "Input Settings")
	void EnableInput() { bInputEnabled = true; }

	UFUNCTION(BlueprintCallable, Category = "Input Settings")
	void DisableInput() { bInputEnabled = false; }

	virtual void Tick(float DeltaTime) override;

private:
	// 内部状態
	bool bInputEnabled;

	// リピート入力処理
	void HandleRepeatInput(float DeltaTime);
	void ProcessLeftMovement(float DeltaTime);
	void ProcessRightMovement(float DeltaTime);
	void ProcessDownMovement(float DeltaTime);

	// ゲームモード取得
	void CacheGameModeReference();
};

// プレイヤーコントローラー関連のデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputStateChanged, bool, bInputEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGamePaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameRestarted);