/**
 * @LC	   : 14/10/2019
 * @file   : mfInputLayout.h
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : 14/10/2019
 * @brief  : Class in charge of setting the input layout.
 * @bug	   : No known bugs.
 */

/**
 * Headers 
 */
#include "mfDefines.h"
#include "mfGraphic_API.h"
#include "mfVertexShader.h"
#pragma once
/**
 * @brief : 
 */
struct mfInputLayoutID
{
  /**
   * @brief : 
   */
  ID3D11InputLayout* ID;
  /**
   * @brief : Define the input layout
   */
//   D3D11_INPUT_ELEMENT_DESC Desc[4] =
//   {     
//       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//       { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//       { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//   };
  //UINT numElements = ARRAYSIZE(Desc);
};

struct mfInputLayoutDesc
{
  D3D11_INPUT_ELEMENT_DESC Desc[4];
  D3D11_INPUT_ELEMENT_DESC LightDesc[2];
};
/**
 * @brief : Class in charge of setting the input layout.
 */
class mfInputLayout
{
public:
  mfInputLayout();
  ~mfInputLayout();
  /**
   * Variables 
   */
private:
  /**
   * @brief : Structure that provides an ID of the class.
   */
  mfInputLayoutID m_inputLayout;
  /**
   * @brief : 
   */
  mfInputLayoutDesc m_descriptor;
  /**
   * Functions 
   */
public:
  /**
    * @brief  : Function that creates the input layout.
    * @param  : mfVertexShader _VertexShader : Vertex shader object.
    * @bug    : No known bugs.
    */
  HRESULT 
  Init(mfInputLayoutDesc & _Desc, mfVertexShader _VertexShader);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update();
  /**
    * @brief  : Set the input layout pointer ID.
    * @bug    : No known bugs.
    */
  void 
  Render();
  /**
    * @brief  : Release the current pointer ID.
    * @bug    : No known bugs.
    */
  void 
  Destroy();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  mfInputLayoutID & getInterface();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  mfInputLayoutDesc & getDescriptor();
};

