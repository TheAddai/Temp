
#include "editor.h"

namespace prime {

	void Editor::Init()
	{

	}

	void Editor::Shutdown()
	{

	}

	void Editor::Update()
	{
		Renderer::BeginDrawing();
		Renderer::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f } );
		Renderer::EndDrawing();
	}
}