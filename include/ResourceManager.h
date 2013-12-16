#ifndef DW_RAYTRACER_RESOURCEMANAGER_H
#define DW_RAYTRACER_RESOURCEMANAGER_H

#include <map>
#include "Image.h"
#include "Mesh.h"
#include "Texture.h"

namespace raytracer {

class ResourceManager
{

public:
	virtual~ ResourceManager();
	
	Image* createImage(const std::string imageID,
		const std::string& imageFilename);
	Mesh* createMesh(const std::string& meshID,
		const std::vector<Vertex>& vertices,
		const Material& material);
	Texture* createTexture(const std::string textureID,
		const std::string& imageID);
		
	bool removeImage(const std::string& imageID);
	bool removeMesh(const std::string& meshID);
	bool removeTexture(const std::string& TextureID);
	
	Image* getImage(const std::string& imageID) const;
	Mesh* getMesh(const std::string& meshID) const;
	Texture* getTexture(const std::string& textureID) const;
	
	void clearImages();
	void clearMeshes();
	void clearTextures();
	void clearAll();
	
private:
	typedef std::map<std::string, Image*> ImageTable;
	typedef std::map<std::string, Mesh*> MeshTable;
	typedef std::map<std::string, Texture*> TextureTable;

	ImageTable images;
	MeshTable meshes;
	TextureTable textures;
	
};

}

#endif
