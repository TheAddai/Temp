
#include "pch.h"
#include "guid.h"

#include <random>

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<ui64> s_uniformDistribution;

namespace prime {

	Guid::Guid() : m_guid(s_uniformDistribution(s_engine)) {}
	Guid::Guid(ui64 guid) : m_guid(guid) {}
}
