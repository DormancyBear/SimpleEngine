////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(WCHAR const *);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	// A shader-resource view is required when binding a resource to a shader stage
	// Examples of shader resources include a constant buffer, a texture buffer, and a texture.
	ID3D11ShaderResourceView* m_texture;
};

#endif