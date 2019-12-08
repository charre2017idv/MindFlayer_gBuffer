/**
 * @LC	   : 21/10/2019
 * @file   : mfMaterial.cpp
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : 17/10/2019
 * @brief  : Class in charge of creating a new material.
 * @bug	   : No known bugs.
 */
#include "mfMaterial.h"
#include "mfRenderTarget.h"

mfMaterial::mfMaterial()
{
}


mfMaterial::~mfMaterial()
{
}

void mfMaterial::Init(vector<mfBaseTextureDesc> _TexturesDesc)
{
  for (int i = 0; i < _TexturesDesc.size(); i++)
  {
    mfTexture tmpTexture;
    m_Material.Textures.push_back(tmpTexture);
    m_Material.Textures[i].Init(_TexturesDesc[i]);
  }
}


void mfMaterial::Destroy()
{
  for (int i = 0; i < m_Material.Textures.size(); i++)
  {
    m_Material.Textures[i].Destroy();
  }
}

mfMaterialID & mfMaterial::getInterface()
{
  return m_Material;
}

void mfMaterial::setTexture()
{
  mfGraphic_API::getSingleton().GetDeviceContext().PSSetShaderResources(0, m_Material.Textures);
}

void mfMaterial::setTexture(vector<mfTexture> _Textures)
{
  mfGraphic_API::getSingleton().GetDeviceContext().PSSetShaderResources(0, _Textures);
}

void mfMaterial::setTexture(vector<mfRenderTarget>& _RenderTargets)
{
  mfGraphic_API::getSingleton().GetDeviceContext().SetShaderResources(0, _RenderTargets);
}
