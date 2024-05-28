
#include "editor.h"
#include "prime/entry.h"

prime::Game* prime::CreateGame()
{
	return new Editor();
}