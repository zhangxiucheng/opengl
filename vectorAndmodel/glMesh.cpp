#include "glMesh.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#endif
glMesh::glMesh(const glMesh& _tg)
{
	vertices = _tg.vertices;
	indices = _tg.indices;
	textures = _tg.textures;

	vao = _tg.vao;
	vbo = _tg.vbo;
	veo = _tg.veo;
}

glMesh::glMesh(const std::vector<mVertex>& _vertices, const std::vector<uint32_t>& _indices, const std::vector<mTexture>& _textures)
{
	vertices = _vertices;
	indices = _indices;
	textures = _textures;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glGenBuffers(1, &veo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(mVertex), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mVertex), (void*)offsetof(mVertex, norm));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mVertex), (void*)offsetof(mVertex, txpos));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

glMesh& glMesh::operator=(const glMesh& _tg)
{
	vertices = _tg.vertices;
	indices = _tg.indices;
	textures = _tg.textures;
	vao = _tg.vao;
	vbo = _tg.vbo;
	veo = _tg.veo;

	return *this;
}

void glMesh::Draw(shaderProgram* sp_ptr)
{
	sp_ptr->use();
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
		// 获取纹理序号（diffuse_textureN 中的 N）
		std::string number;
		std::string name = textures[i].txname;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		name = (/*"material." +*/ name + number);
		sp_ptr->setUniform(glUniformType::INT, (char*)&i, sizeof(int), name);
		glBindTexture(GL_TEXTURE_2D, textures[i].tx_id);
	}
	glActiveTexture(GL_TEXTURE0);

	// 绘制网格
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void glMesh::FreeBuffer()
{
	if (vao)
		glDeleteVertexArrays(1, &vao);
	if (vbo)
		glDeleteBuffers(1, &vbo);
	if (veo)
		glDeleteBuffers(1, &veo);
}


//glMesh::~glMesh()
//{
//	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &veo);
//	glDeleteVertexArrays(1, &vao);
//}

void glModel::processNode(aiNode* node, const aiScene* scene, std::vector<BB>& bbs)
{
	//preocess mesh for this node
	for (auto i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene, bbs));
	}

	//process child node
	for (auto i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene, bbs);
}

glMesh glModel::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<BB>& bbs)
{
	std::vector<mVertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<mTexture> textures;
	BB bb;
	auto updateBB = [tg = &bb](glm::vec3& v) 
	{
		if (v.x > tg->max.x)
			tg->max.x = v.x;
		if (v.x < tg->min.x)
			tg->min.x = v.x;

		if (v.y > tg->max.y)
			tg->max.y = v.y;
		if (v.y < tg->min.y)
			tg->min.y = v.y;

		if (v.z > tg->max.z)
			tg->max.z = v.z;
		if (v.z < tg->min.z)
			tg->min.z = v.z;

	};
	bb.max.x = mesh->mVertices[0].x;
	bb.min.x = mesh->mVertices[0].x;
	bb.max.y = mesh->mVertices[0].y;
	bb.min.y = mesh->mVertices[0].y;
	bb.max.z = mesh->mVertices[0].z;
	bb.min.z = mesh->mVertices[0].z;

	//save vertexs
	mVertex vertex;
	
	for (auto i = 0; i < mesh->mNumVertices; i++)
	{
		//position
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		updateBB(vertex.pos);
		//norm
		vertex.norm.x = mesh->mNormals[i].x;
		vertex.norm.y = mesh->mNormals[i].y;
		vertex.norm.z = mesh->mNormals[i].z;
		//txpos
		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
		{
			vertex.txpos.x = mesh->mTextureCoords[0][i].x;
			vertex.txpos.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.txpos = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	//save indices
	for (auto i = 0; i < mesh->mNumFaces; i++)
	{
		auto face = mesh->mFaces[i];
		for (auto j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//diffusion maps
		std::vector<Texture> diffuseMaps = loadTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//specualr maps
		std::vector<Texture> specularMaps = loadTexture(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//normal maps
		std::vector<Texture> normalMaps = loadTexture(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//height maps
		std::vector<Texture> heightMaps = loadTexture(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		//none
		std::vector<Texture> noneMaps = loadTexture(material, aiTextureType_NONE, "texture_height");
		textures.insert(textures.end(), noneMaps.begin(), noneMaps.end());
		//unkown
		/*std::vector<Texture> unknownMaps = loadTexture(material, aiTextureType_UNKNOWN, "texture_height");
		textures.insert(textures.end(), noneMaps.begin(), unknownMaps.end());*/
	}
	bbs.push_back(bb);

	return glMesh(vertices, indices, textures);
}

std::vector<mTexture> glModel::loadTexture(aiMaterial* mat, aiTextureType tt, const char* txname)
{
	std::vector<mTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(tt); i++)
	{
		aiString str;
		mat->GetTexture(tt, i, &str);
		mTexture texture;
		texture.tx_id = TextureFromFile(str.C_Str(), directory);
		texture.txname = txname;
		texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}

glModel::glModel(const std::string& path)
{
	//load model from sys files
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	std::vector<BB> bbs;

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string s = "ERROR::ASSIMP::";
		s += importer.GetErrorString();
		g_log->newlog(s);
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene, bbs);
	if (!bbs.empty())
		mp.bb = bbs[0];
	else
		mp = glPhysics();
	for (auto& _bb : bbs)
	{
		if (_bb.max.x > mp.bb.max.x)
			mp.bb.max.x = _bb.max.x;
		if (_bb.max.y > mp.bb.max.y)
			mp.bb.max.y = _bb.max.y;
		if (_bb.max.z > mp.bb.max.z)
			mp.bb.max.z = _bb.max.z;
		if (_bb.min.x < mp.bb.min.x)
			mp.bb.min.x = _bb.min.x;
		if (_bb.min.y < mp.bb.min.y)
			mp.bb.min.y = _bb.min.y;
		if (_bb.min.z < mp.bb.min.z)
			mp.bb.min.z = _bb.min.z;
	}
	importer.FreeScene();
}

glModel::glModel(const glModel& _tg)
{
	meshes = _tg.meshes;
	mp = _tg.mp;
}

glModel& glModel::operator=(const glModel& _tg)
{
	meshes = _tg.meshes;
	mp = _tg.mp;
	return *this;
}

void glModel::Draw(shaderProgram* ptr_sp)
{
	for (auto mesh : meshes)
		mesh.Draw(ptr_sp);
}

glModel::~glModel()
{
	for (auto mesh : meshes)
		mesh.FreeBuffer();
}

glID TextureFromFile(std::string path, std::string dir)
{
	std::string fp;
	fp = dir + '/' + path;

	glID tx;
	glGenTextures(1, &tx);
	glBindTexture(GL_TEXTURE_2D, tx);
	//set texture wraping parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//set texture filtering parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int iw, ih, ich;
	//flip picture when loaded
	stbi_set_flip_vertically_on_load(true);
	//texture1
	unsigned char* dat = stbi_load(fp.c_str(), &iw, &ih, &ich, 4);
	if (dat)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iw, ih, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat);
		glGenerateMipmap(tx);
	}
	else
	{
		std::string c = "Failed to open: " + fp;
		g_log->newlog(c);
	}
	stbi_image_free(dat);

	return tx;
}

void alwaysOnGround(const BB& bb, glm::mat4& trans)
{
	glm::vec4 loc(0.0f, bb.min.y, 0.0f, 1.0f);
	loc = trans * loc;
	if (loc.y < 0)
	{
		trans[3][1] -= loc.y;
	}
	return;
}


glID CreateCubeMap(std::vector<std::string> paths)
{
	glID cubeTex;
	glGenTextures(1, &cubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	auto id = 0;
	for (auto& path : paths)
	{
		int wd, ht, c;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(path.c_str(), &wd, &ht, &c, 4);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + id++, 0, GL_RGBA, wd, ht, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return cubeTex;
}