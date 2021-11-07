// Fill out your copyright notice in the Description page of Project Settings.


#include "Platforms.h"

bool UPlatforms::IsAndroid()
{
#if PLATFORM_ANDROID
	return true;
#endif
	return false;
}
