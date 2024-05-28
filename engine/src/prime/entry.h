#pragma once

#include "core/engine.h"
#include "core/platform_detection.h"

#ifdef P_WINDOWS
int main(int argc, char** argv)
{
	prime::Engine::Run();
	return 0;
}
#endif // P_WINDOWS
