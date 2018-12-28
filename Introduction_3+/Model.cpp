#include "Model.h"



Model::Model()
{
}


Model::~Model()
{
}

void Model::LoadModel(const std::string & filename)
{
	Assimp::Importer importer;
	// importing scene to use
	const aiScene * scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s failed to load: %s", filename, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

void Model::RenderModel()
{
}

void Model::ClearModel()
{
}

void Model::LoadNode(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indicies;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		// inserting vertices at end of an array
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,  mesh->mVertices[i].y ,  mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			// adding (loading) textures
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,  mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
	}
}

void Model::LoadMaterials(const aiScene * scene)
{
}
