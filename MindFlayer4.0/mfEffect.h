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
#include "mfTechnique.h"
#include "mfCamera.h"
#include "mfRenderTarget.h"
#pragma once
/**
 * Forward Class Declarations 
 */
class mfDepthStencilView;
class mfPass;
class mfTransform;
/**
 * @brief : 
 */
class mfEffect
{
public:
  mfEffect();
  ~mfEffect();
  /**
   * Members 
   */
private:
  /**
   * @brief : 
   */
  vector<mfTechnique> m_Techniques;
  /**
   * @brief : 
   */
  mfCamera m_Camera;
public:
  vector<mfRenderTarget> m_gBufferRT;
  vector<mfRenderTarget> m_posRT;
  vector<mfRenderTarget> m_norRT;
  vector<mfRenderTarget> m_albRT;
  vector<mfRenderTarget> m_speRT;
  vector<mfRenderTarget> m_LightRT;

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
  Init(mfBasePassDesc _PassDesc, int _Index);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update(mfDepthStencilView & _DepthStencilView, mfCamera & _CameraRef, mfTransform & _Transform, const void * _Resource, float _Time);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Render(mfDepthStencilView & _DepthStencilView);
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
  void CreateTechniques(int _NumTechniques);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void getTechnique(mfPassTypeID _PassType);
  void getRenderTargets(mfPassTypeID _PassType);
};


