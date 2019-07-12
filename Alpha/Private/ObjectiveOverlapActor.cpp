#include "ObjectiveOverlapActor.h"
#include "Logger.h"

AObjectiveOverlapActor::AObjectiveOverlapActor() {
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
}

void AObjectiveOverlapActor::BeginPlay(){
	Super::BeginPlay();
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveOverlapActor::OnOverlap);
}

void AObjectiveOverlapActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherComp != nullptr) && (OtherActor != this))
	{
		ULogger::ScreenMessage(FColor::Red, "Objective overlapped");
	}
}