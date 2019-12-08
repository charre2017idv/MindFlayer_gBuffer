/**
 * @LC	   : 21/10/2019
 * @file   : mfMaterial.h
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : 17/10/2019
 * @brief  : Class in charge of creating a new material.
 * @bug	   : No known bugs.
 */

/**
 * Headers 
 */
#include "mfDefines.h"
#include "mfTexture.h"
#include "mfGraphic_API.h"
#pragma once
class mfRenderTarget;
/**
 * @brief : Class in charge of creating a new material.
 */
struct mfMaterialID
{
  /**
 * @brief : Vector in charge of allocating textures.
 */
  vector<mfTexture> Textures;
};

/**
 * @brief : Class in charge of creating a new material.
 */
class mfMaterial
{
public:
  mfMaterial();
  ~mfMaterial();
/**
 * Members 
 */
private:
  /**
   * @brief : Structure that provides an ID of the class.
   */
  mfMaterialID m_Material;
  /**
   * Methods
   */
public:
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Init(vector<mfBaseTextureDesc> _TexturesDesc);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Render();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Destroy();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  mfMaterialID & getInterface();
  /**
    * @brief  : Function that renders the texture that is relative to the initial 
    loaded texture.
    * @bug    : No know bugs.
    */
  void setTexture();
  /**
    * @brief  : Function that Renders textures from an external vector of textures.
    * @param  : vector<mfTexture> _Textures : Vector of textures.
    * @bug    : No know bugs.
    */
  void setTexture(vector<mfTexture> _Textures);
  /**
    * @brief  : Function that Renders render Targets form an external vector of
                render targets.
    * @param  : vector<mfRenderTarget>& _RenderTargets : Vector of render targets.
    * @bug    : No know bugs.
    */
  void setTexture(vector<mfRenderTarget>& _RenderTargets);
};

