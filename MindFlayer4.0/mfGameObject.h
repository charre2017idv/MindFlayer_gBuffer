/**
 * @LC	   : //2019
 * @file   : 
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : //2019
 * @brief  : 
 * @bug	   : No known bugs.
 */

/**
 * Headers 
 */
#include "mfDefines.h"
#include "mfGraphic_API.h"
#include "mfConstBuffer.h"
#include "mfMesh.h"
#include "mfLoadModel.h"
#include "mfMaterial.h"
#pragma once
class mfTransform;
/**
 * @brief : 
 */
class mfGameObject
{
public:
  mfGameObject();
  ~mfGameObject();
  /**
   * Members 
   */
private:
  /**
   * @brief : 
   */
  mfMesh m_Mesh;
  CBChangesEveryFrame m_Model;
  /**
   * @brief : 
   */
  mfConstBuffer m_ModelBuffer;
  /**
   * @brief : 
   */
  int m_IndexSize;
  /**
   * @brief : 
   */
  XMMATRIX   m_Matrix = XMMatrixIdentity();
  /**
   * @brief : 
   */
  mfMaterial m_Material;
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
  Init(mfLoadModelID _RawData, mfBufferDesc _ModelDesc, vector<mfBaseTextureDesc> _TexturesDesc);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update(mf_PRIMITIVE_TOPOLOGY _Topology, mfTransform & _Transform, float _Time);
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
    * @brief  : Function that Renders a texture of render Targets form an 
                external vector of render targets.
    * @param  : vector<mfRenderTarget>& _RenderTargets : Vector of render targets.
    * @bug    : No know bugs.
    */
  void setTexture(vector<mfRenderTarget>& _RenderTargets);
};

