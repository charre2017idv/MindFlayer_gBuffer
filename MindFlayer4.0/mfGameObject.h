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
#pragma once

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
  CBChangesEveryFrame m_Model;
  mfMesh m_Mesh;
  mfConstBuffer m_ModelBuffer;
  int m_IndexSize;
  XMMATRIX   m_Matrix = XMMatrixIdentity();
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
  Init(mfLoadModelID _RawData, mfBufferDesc _ModelDesc);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update(mf_PRIMITIVE_TOPOLOGY _Topology, float _Time);
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

