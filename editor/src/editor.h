#pragma once

#include "prime/prime.h"

namespace prime {

	class Editor : public Game
	{
	public:
		void Init() override;
		void Shutdown() override;

		void Update() override;

		Config GetConfig() const override
		{
			Config config;

			return config;
		}
	};
}