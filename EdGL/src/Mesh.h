#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include "Util.h"

namespace edgl {

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Material;

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Ref<Material> material);
	~Mesh();
	inline const VertexArray& GetVAO() const { return *m_Vao; }
	inline const VertexBuffer& GetVBO() const { return *m_Vbo; }
	inline const IndexBuffer& GetIBO() const { return *m_Ibo; }
	inline unsigned int GetVerticesNumber() const { return m_VerticesNumber; }
	inline unsigned int GetIndicesNumber() const { return m_IndicesNumber; }

	inline bool CanApplyCullFace() const { return m_ApplyCullFace; }

	void Draw(Shader& shader) const;
	inline void SetMaterial(Ref<Material> material) { m_Material = material; }
	inline void EnableCullFace(bool value) { m_ApplyCullFace = value; }

private:
	unsigned int m_VerticesNumber;
	unsigned int m_IndicesNumber;

	Ref<Material> m_Material;

	bool m_ApplyCullFace;

	VertexArray* m_Vao;
	VertexBuffer* m_Vbo;
	IndexBuffer* m_Ibo;

};

}
