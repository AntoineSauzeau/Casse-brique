#pragma once

#define INTERFACE_WIDTH 900
#define INTERFACE_HEIGHT 575

void CreateInterface();
void DestroyInterface();
void SetInterfacePage(int index);

void StartEventLoop();
void StartUpdateLoop();