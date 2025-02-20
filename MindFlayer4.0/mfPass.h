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
#include "mfTexture.h"
#include "mfRenderTarget.h"
#include "mfVertexShader.h"
#include "mfInputLayout.h"
#include "mfPixelShader.h"
#include "mfMesh.h"
#include "mfSampler.h"
#include "mfGameObject.h"
#include "mfRasterizer.h"
#include "mfConstBuffer.h"
#include "mfTexture.h"
#include "mfSwapchain.h"
#pragma once
/**
 * Forward Class Declarations 
 */
class mfLoadModel;
class mfDepthStencilView;
class mfTransform;
/**
 * @brief : Pass Type ID
 */
enum mfPassTypeID
{

  Default,
  GBUFFER_PASS,
  LIGHT_PASS,
  BACKBUFFER_PASS
};
/**
 * @brief :
 */
struct mfBasePassDesc
{
  mfPassTypeID PassID;
  vector<mfLoadModelID> RawData;
  mfBufferDesc ModelBufferDesc;
  mfSamplerDesc SamplerDesc;
  mfBaseShaderDesc VertexShaderDesc;
  mfBaseShaderDesc PixelShaderDesc;
  mfRasterizerDesc RasterizerDesc;
  mfInputLayoutDesc InputLayoutDesc;
  vector<mfBaseTextureDesc> RenderTargetsDesc;
  vector<mfBaseTextureDesc> ModelTexturesDesc;
  mf_PRIMITIVE_TOPOLOGY Topology;
  mfSwapchain Swapchain;
  mfBufferDesc LightBufferDesc;
  vector<mfRenderTarget> tmpRenderTargets;
};
/**
 * @brief : 
 */
struct mfgBufferPassID
{
  /**
   * @brief : 
   */
  vector<mfRenderTarget> RenderTargetsVec;
};

  /**
   * @brief :
   */
class mfPass
{
public:
  mfPass();
  ~mfPass();
  /**
   * Members
   */
private:
  /**
   * @brief : 
   */
  mfgBufferPassID m_gBuffer;
  
  /**
   * @brief : 
   */
  float ClearColor0[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha   
  /**
   * @brief : 
   */
  float ClearColor1[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha  
  /**
   * @brief : 
   */
  float ClearColor2[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // red, green, blue, alpha   
  /**
   * @brief : 
   */
  float ClearColor3[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // red, green, blue, alpha 
  /**
   * @brief : 
   */
  float whiteClearcolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // red, green, blue, alpha 
  float BlackClearcolor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha 

  /**
   * @brief :
   */
  mfBasePassDesc m_descriptor;
private:
  /**
   * @brief :
   */
  vector<mfGameObject> m_GameObject;
  /**
   * @brief :
   */
  mfVertexShader m_VertexShader;
  /**
   * @brief :
   */
  mfInputLayout m_InputLayout;
  /**
   * @brief :
   */
  mfPixelShader m_PixelShader;
  /**
   * @brief :
   */
  mfSampler m_SamplerState;
  /**
   * @brief :
   */
  mfRasterizer m_Rasterizer;
  /**
   * @brief : 
   */
  mfConstBuffer m_LightBuffer;

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
    Init(mfBasePassDesc _Desc);
  /**
    * @brief  :
    * @param  :
    * @bug    : No known bugs.
    */
  void
    Update(mfDepthStencilView & _DepthStencilView, mfTransform & _Transform, const void * _Resource, float _Time);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void Render(mfDepthStencilView & _DepthStencilView);
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
  mfgBufferPassID & getInterface();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  mfBasePassDesc & getDescriptor();
};

