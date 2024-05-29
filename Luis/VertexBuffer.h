#pragma once
#include "../Mesh.h"

class VertexBuffer {
private:
	unsigned int m_BufferID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();

	void LateInit(const void* data, unsigned int size);

	void MeshDataInit(std::vector<Mesh> data);

	void Bind() const;
	void Unbind() const;
};