#include "Model.h"



Model::Model()
{
}


Model::~Model()
{
}

void Model::LoadModel(const std::string & filename, std::string subfolder)
{
	Assimp::Importer importer;
	// importing scene to use
	const aiScene * scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	_subfolder = "/"+subfolder+"/";
	if (!scene)
	{
		printf("Model (%s failed to load: %s", filename, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}


// most important function
void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		//
		if (materialIndex < textureList.size() && textureList[materialIndex] != nullptr)
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	// usually would check as list of ptrs if it is getting used or not and delete if not, otherwise block dfelete???
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
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
	std::vector<unsigned int> indices;

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
		//doing in reverse bcz of shader, check C model importing 43ish min
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,  -mesh->mNormals[i].y ,  -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh * newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	// where is material in material list
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial * material = scene->mMaterials[i];

		textureList[i] = nullptr;
		//at least 1 material
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int index = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(index + 1);

				std::string texPath = std::string("Textures/")+ _subfolder + filename;

				textureList[i] = new Texture(texPath.c_str());

				// if texture load failed
				if (!textureList[i]->LoadTexture())
				{
					printf("failed to load texture at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureAlpha();
		}
	}
}
