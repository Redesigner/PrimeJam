// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/DropTables/DropTableAsset.h"

TArray<TSubclassOf<APickup>> UDropTableAsset::MakeRandomDrops() const
{
	TArray<TSubclassOf<APickup>> Result;
	
	for (const auto& [Pickup, DropChance] : Entries)
	{
		if (FMath::FRand() < DropChance)
		{
			Result.Add(Pickup);
		}
	}
	
	return Result;
}
