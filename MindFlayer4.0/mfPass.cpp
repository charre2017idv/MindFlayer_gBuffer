#include "mfPass.h"
#include "mfLoadModel.h"
#include "mfDepthStencilView.h"
#include "mfTransform.h"
mfPass::mfPass()
{
}


mfPass::~mfPass()
{
}

void mfPass::Init(mfBasePassDesc _Desc)
{
  m_descriptor = _Desc;
 
  // Initialize Render targets
  if (m_descriptor.PassID == GBUFFER_PASS)
  {
    for (int i = 0; i < _Desc.RenderTargetsDesc.size(); i++)
    {
      mfRenderTarget tmpRenderTargets;
      m_gBuffer.RenderTargetsVec.push_back(tmpRenderTargets);
      m_gBuffer.RenderTargetsVec[i].Init(_Desc.RenderTargetsDesc[i]);
      // Create Shader Resources from Render Targets
      
    }
  }
  if (m_descriptor.PassID == LIGHT_PASS)
  {
    for (int i = 0; i < _Desc.RenderTargetsDesc.size(); i++)
    {
      mfRenderTarget tmpRenderTargets;
      m_gBuffer.RenderTargetsVec.push_back(tmpRenderTargets);
      m_gBuffer.RenderTargetsVec[i].Init(_Desc.RenderTargetsDesc[i]);
    }    
  }
  if (m_descriptor.PassID == BACKBUFFER_PASS)
  {
    mfRenderTarget tmpRenderTargets;
    m_gBuffer.RenderTargetsVec.push_back(tmpRenderTargets);
    // Set Back buffer
    ID3D11Texture2D* tmpTex;
    _Desc.Swapchain.getInterface().ID->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_gBuffer.RenderTargetsVec[0].getInterface().ID);
    mfGraphic_API::getSingleton().GetDevice().CreateRenderTargetViewBF(m_gBuffer.RenderTargetsVec[0]);
  }
 
  if (m_descriptor.PassID == LIGHT_PASS)
  {
    m_LightBuffer.Init(_Desc.LightBufferDesc);
  }
  

#ifdef mfDIRECTX
  _Desc.VertexShaderDesc.BlobOut = &m_VertexShader.getInterface().VSBlob;
#endif // mfDIRECTX
  // Initialize Vertex Shader
  m_VertexShader.Init(_Desc.VertexShaderDesc);

  // Initialize Input Layout
  m_InputLayout.Init(_Desc.InputLayoutDesc, m_VertexShader);

#ifdef mfDIRECTX
  m_VertexShader.getInterface().VSBlob->Release();
#endif // mfDIRECTX

#ifdef mfDIRECTX
  _Desc.PixelShaderDesc.BlobOut = &m_PixelShader.getInterface().PSBlob;
#endif // mfDIRECTX

  // Initialize Pixel Shader
  m_PixelShader.Init(_Desc.PixelShaderDesc);

  // Initialize GameObjects
  for (int i = 0; i < _Desc.RawData.size(); i++)
  {
    mfGameObject tmpGameObject;
    m_GameObject.push_back(tmpGameObject);
    for (int j = 0; j < m_GameObject.size(); j++)
    {
      m_GameObject[j].Init(_Desc.RawData[i], _Desc.ModelBufferDesc, _Desc.ModelTexturesDesc);
    }
  }

  // Initialize Sampler state
  m_SamplerState.Init(_Desc.SamplerDesc);

  // Initialize Rasterizer 
  m_Rasterizer.Init(_Desc.RasterizerDesc);

}

void mfPass::Update(mfDepthStencilView & _DepthStencilView, mfTransform & _Transform, const void * _Resource,  float _Time)
{   
  // Set GameObject
  if (m_descriptor.PassID == GBUFFER_PASS)
  {
    for (int i = 0; i < m_GameObject.size(); i++)
    {
      m_GameObject[i].Update(m_descriptor.Topology, _Transform, _Time);
    }
  }

  if (m_descriptor.PassID == LIGHT_PASS)
  {
    m_LightBuffer.Update(_Resource);
  }
}

void mfPass::Render(mfDepthStencilView & _DepthStencilView)
{
  if (m_descriptor.PassID == GBUFFER_PASS || m_descriptor.PassID == LIGHT_PASS || m_descriptor.PassID == BACKBUFFER_PASS)
  {
    // Set Samplers
    m_SamplerState.Update();
    // Set Rasterizer
    m_Rasterizer.Update();
  }

 // Set shader resources
  if (m_descriptor.PassID == GBUFFER_PASS)
  {
    // Set Render Targets
    mfGraphic_API::getSingleton().OMSetRenderTargets(m_gBuffer.RenderTargetsVec, _DepthStencilView);
    // Clear the render target buffers
    mfGraphic_API::getSingleton().GetDeviceContext().ClearRenderTargetView(BlackClearcolor);
    // Clear Depth Stencil View
    mfGraphic_API::getSingleton().ClearDepthStencilView(_DepthStencilView);
    // Set Shader Resources
    for (int i = 0; i < m_GameObject.size(); i++)
    {
      m_GameObject[i].setTexture();
    }
  }
  if (m_descriptor.PassID == LIGHT_PASS)
  {
    // Set Render Targets
    mfGraphic_API::getSingleton().OMSetRenderTargets(m_gBuffer.RenderTargetsVec, _DepthStencilView);
    // Clear Render Targets
    mfGraphic_API::getSingleton().GetDeviceContext().ClearRenderTargetView(m_gBuffer.RenderTargetsVec[0], BlackClearcolor);
    // Set shader Resources
    for (int i = 0; i < m_GameObject.size(); i++)
    {
      m_GameObject[i].setTexture(m_descriptor.tmpRenderTargets);
    }
  }
  if (m_descriptor.PassID == BACKBUFFER_PASS)
  {
    // Set Render Targets
    mfGraphic_API::getSingleton().OMSetRenderTargets(m_gBuffer.RenderTargetsVec, _DepthStencilView);
    // Clear Render Targets
    mfGraphic_API::getSingleton().GetDeviceContext().ClearRenderTargetView(m_gBuffer.RenderTargetsVec[0], BlackClearcolor);
    // Clear Depth Stencil View
    mfGraphic_API::getSingleton().ClearDepthStencilView(_DepthStencilView);
    // Set shader Resources
    for (int i = 0; i < m_GameObject.size(); i++)
    {
      m_GameObject[i].setTexture(m_descriptor.tmpRenderTargets);
    }
  }
  // Clear Depth Stencil View
  if (m_descriptor.PassID == GBUFFER_PASS || m_descriptor.PassID == LIGHT_PASS || m_descriptor.PassID == BACKBUFFER_PASS)
  {
    // Set Vertex Shader
    m_VertexShader.Render();
    // Set Input Layout
    m_InputLayout.Render();
    // Set Pixel Shader
    m_PixelShader.Render();
  }
  // Draw Game Object
  for (int i = 0; i < m_GameObject.size(); i++)
  {
    m_GameObject[i].Render();
  }
  // Set Constant Buffer for the LightPass
  if (m_descriptor.PassID == LIGHT_PASS)
  {
    m_LightBuffer.Render(0, 1, true);
  }
}

void mfPass::Destroy()
{
  // Destroy Render Targets
  for (int i = 0; i < m_gBuffer.RenderTargetsVec.size(); i++)
  {
    m_gBuffer.RenderTargetsVec[i].Destroy();
  }
  // Destroy Game Objects
  for (int i = 0; i < m_GameObject.size(); i++)
  {
    m_GameObject[i].Destroy();
  }
  m_LightBuffer.Destroy();
  m_VertexShader.Destroy();
  m_PixelShader.Destroy();
  m_InputLayout.Destroy();
  m_SamplerState.Destroy();
  m_Rasterizer.Destroy();
}

mfgBufferPassID & mfPass::getInterface()
{
  return m_gBuffer;
}

mfBasePassDesc & mfPass::getDescriptor()
{
  return m_descriptor;
}
