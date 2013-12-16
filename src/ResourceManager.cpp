#include "ResourceManager.h"
#include "TGA.h"

using namespace raytracer;

ResourceManager::~ResourceManager()
{
	clearAll();
}

Image* ResourceManager::createImage(const std::string imageID,
	const std::string& imageFilename)
{
	ImageTable::const_iterator it = images.find(imageID);
	if (it != images.end())
		removeImage(imageID);
	Image* image = tga::readTGAFile(imageFilename);
	images.insert( std::pair<std::string, Image*>(imageID, image) );
	return image;
}

Mesh* ResourceManager::createMesh(const std::string& meshID,
	const std::vector<Vertex>& vertices,
	const Material& material)
{
	// Check if mesh already exists. If so, remove the mesh
	// to make way for the new one
	MeshTable::const_iterator it = meshes.find(meshID);
	if (it != meshes.end())
		removeMesh(meshID);
	// Create mesh
	Mesh* mesh = new Mesh(vertices, material);
	// Insert mesh into table so it can be retrieved later
	meshes.insert( std::pair<std::string, Mesh*>(meshID, mesh) );
	return mesh;
}

Texture* ResourceManager::createTexture(const std::string textureID,
	const std::string& imageID)
{
	// If specified image has not been loaded, return null
	Image* texImage = getImage(imageID);
	if (!texImage)
		return NULL;

	TextureTable::const_iterator texIt = textures.find(imageID);
	if (texIt != textures.end())
		removeTexture(textureID);
	Texture* texture = new Texture(texImage);
	textures.insert( std::pair<std::string, Texture*>(textureID, texture) );
	return texture;
}
	
bool ResourceManager::removeImage(const std::string& imageID)
{
	ImageTable::iterator it = images.find(imageID);
	if (it != images.end())
	{
		images.erase(it);
		return true;
	}
	else
	{	
		return false;
	}
}

bool ResourceManager::removeMesh(const std::string& meshID)
{
	MeshTable::iterator it = meshes.find(meshID);
	if (it != meshes.end())
	{
		meshes.erase(it);
		return true;
	}
	else
	{	
		return false;
	}
}

bool ResourceManager::removeTexture(const std::string& textureID)
{
	TextureTable::iterator it = textures.find(textureID);
	if (it != textures.end())
	{
		textures.erase(it);
		return true;
	}
	else
	{	
		return false;
	}
}

Image* ResourceManager::getImage(const std::string& imageID) const
{
	ImageTable::const_iterator it = images.find(imageID);
	if (it != images.end())
		return it->second;
	else
		return NULL;
}

Mesh* ResourceManager::getMesh(const std::string& meshID) const
{
	MeshTable::const_iterator it = meshes.find(meshID);
	if (it != meshes.end())
		return it->second;
	else
		return NULL;
}

Texture* ResourceManager::getTexture(const std::string& textureID) const
{
	TextureTable::const_iterator it = textures.find(textureID);
	if (it != textures.end())
		return it->second;
	else
		return NULL;
}

void ResourceManager::clearImages()
{
	for (ImageTable::iterator it = images.begin(); (it != images.end()); it++)
		delete it->second;
	images.clear();
}

void ResourceManager::clearMeshes()
{
	for (MeshTable::iterator it = meshes.begin(); (it != meshes.end()); it++)
		delete it->second;
	meshes.clear();
}

void ResourceManager::clearTextures()
{
	for (TextureTable::iterator it = textures.begin(); (it != textures.end()); it++)
		delete it->second;
	textures.clear();
}

void ResourceManager::clearAll()
{
	clearImages();
	clearMeshes();
	clearTextures();
}
