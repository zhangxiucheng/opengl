#ifndef __GL_MESH__
#define __GL_MESH__
#include"glFrameWork.h"
#include"shader.h"
#include"glPhysics.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 txpos;
};



using mVertex = struct Vertex;


struct Texture
{
	glID tx_id;
	std::string txname;
	aiString path;
};
using mTexture = struct Texture;

class glMesh
{
private:
	std::vector<mVertex>  vertices;
	std::vector<mTexture> textures;
	std::vector<uint32_t> indices;
	glID vao, vbo, veo;
public:
	glMesh() = delete;
	glMesh(const glMesh&);
	glMesh(const std::vector<mVertex>& _vertices, const std::vector<uint32_t>& _indices, const std::vector<mTexture>& _texture);

	glMesh& operator=(const glMesh&);

	void Draw(shaderProgram* sp_ptr);
	void FreeBuffer();
//	~glMesh();
};

class glModel
{
private:
	std::vector<glMesh> meshes;

	std::string directory;
	void processNode(aiNode* node, const aiScene* scene, std::vector<BB>& bbs);
	glMesh processMesh(aiMesh* mesh, const aiScene* scene, std::vector<BB>& bbs);
	std::vector<mTexture> loadTexture(aiMaterial* mat, aiTextureType tt, const char* txname);


public:
	glModel() = delete;
	glModel(const std::string& path);
	glModel(const glModel& _tg);
	glModel& operator=(const glModel& _tg);

	void Draw(shaderProgram* ptr_sp);
	//expose binding box
	glPhysics mp;
	~glModel();
};

glID TextureFromFile(std::string path, std::string dir);
glID CreateCubeMap(std::vector<std::string> paths);
void alwaysOnGround(const BB& bb, glm::mat4& trans);

#endif
