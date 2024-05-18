#include "Object3D.h"


class Object3D
{
public:
	Object3D();
	~Object3D();
	
	bool loadOBJ(
		const char* path,
		std::vector < glm::vec3 >& out_vertices,
		std::vector < glm::vec2 >& out_uvs,
		std::vector < glm::vec3 >& out_normals
	);

private:



};

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
}


