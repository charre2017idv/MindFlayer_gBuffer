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
#include "mfRenderTarget.h"
#include "mfVertexShader.h"
#include "mfInputLayout.h"
#include "mfPixelShader.h"
#include "mfMesh.h"
#include "mfSampler.h"
#include "mfGameObject.h"
#include "mfRasterizer.h"
#include "mfConstBuffer.h"
#include "mfCamera.h"
#pragma once
class mfLoadModel;
class mfDepthStencilView;
/**
 * @brief :
 */
struct mfBasePassDesc
{
  mfLoadModelID RawData;
  mfBufferDesc ViewBufferDesc;
  mfBufferDesc ProjBufferDesc;
  mfBufferDesc ModelBufferDesc;
  mfSamplerDesc SamplerDesc;
  mfCameraDesc CameraDesc;
  WCHAR * VertexShaderFileName;
  WCHAR * PixelShaderFileName;
  mfRasterizerDesc RasterizerDesc;
  mfInputLayoutDesc InputLayoutDesc;
};
/**
 * @brief :
 */
class mfBasePass
{
public:
  mfBasePass();
  ~mfBasePass();
  /**
   * Members
   */
protected:
  /**
   * @brief :
   */
  mfBasePassDesc m_descriptor;
private:
  /**
   * @brief :
   */
  mfGameObject m_GameObject;
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
    Update(float _Time);
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

};

