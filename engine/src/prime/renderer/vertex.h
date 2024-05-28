#pragma once

#include "prime/core/defines.h"

#include <vector>

enum class VertexType
{
	position
};

static ui32 GetVertexTypeSize(VertexType vertexType)
{
	switch (vertexType)
	{
	case VertexType::position:
		return 12;
		break;
	}
	return 0;
}

static ui32 GetVertexTypeCount(VertexType vertexType)
{
	switch (vertexType)
	{
	case VertexType::position:
		return 3;
		break;
	}
	return 0;
}

class Vertex
{
public:
	Vertex() = default;
	Vertex(VertexType vertexType, const char* name)
		: type(vertexType), name(name), size(GetVertexTypeSize(vertexType)) {}

	ui32 GetCount() const { return GetVertexTypeCount(type); }

public:
	const char* name = nullptr;
	VertexType type = VertexType::position;
	ui32 size = 0;
	ui64 offset = 0;
};

class VertexLayout
{
public:
	VertexLayout() = default;

	void AddVertex(Vertex& vertex) { m_vertices.push_back(vertex); }

	ui32 GetStride() const { return m_stride; }
	const std::vector<Vertex>& GetVertices() const { return m_vertices; }

	std::vector<Vertex>::iterator begin() { return m_vertices.begin(); }
	std::vector<Vertex>::iterator end() { return m_vertices.end(); }
	std::vector<Vertex>::const_iterator begin() const { return m_vertices.begin(); }
	std::vector<Vertex>::const_iterator end() const { return m_vertices.end(); }

	void ProcessVertices()
	{
		ui64 offset = 0;
		m_stride = 0;
		for (Vertex& vertex : m_vertices)
		{
			vertex.offset = offset;
			offset += vertex.size;
			m_stride += vertex.size;
		}
	}

private:
	ui32 m_stride = 0;
	std::vector<Vertex> m_vertices;
};
