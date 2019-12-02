#include "mfPass.h"
#include "mfLoadModel.h"
#include "mfDepthStencilView.h"

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
  if (_Desc.RenderTargetsDesc.size() == 1 )
  {
    // Set Back buffer
    _Desc.Swapchain.getInterface().ID->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_gBuffer.RenderTargetsVec[0].getInterface().ID);
    mfGraphic_API::getSingleton().GetDevice().CreateRenderTargetViewBF(m_gBuffer.RenderTargetsVec[0]);
    m_gBuffer.RenderTargetsVec[1].getInterfaceRT().ID = NULL;
    m_gBuffer.RenderTargetsVec[2].getInterfaceRT().ID = NULL;
    m_gBuffer.RenderTargetsVec[3].getInterfaceRT().ID = NULL;
  }
  else if (_Desc.RenderTargetsDesc.size() >= 2)
  {
    for (int i = 0; i < _Desc.RenderTargetsDesc.size(); i++)
    {
      mfRenderTarget tmpRenderTargets;
      m_gBuffer.RenderTargetsVec.push_back(tmpRenderTargets);
      m_gBuffer.RenderTargetsVec[i].Init(_Desc.RenderTargetsDesc[i]);
    }
  }

  // Initialize textures
  for (int i = 0; i < _Desc.ModelTexturesDesc.size(); i++)
  {
    mfTexture tmpTexture;
    m_gBuffer.TexturesVec.push_back(tmpTexture);
    m_gBuffer.TexturesVec[i].Init(_Desc.ModelTexturesDesc[i]);
  }

  for (int i = 0; i < _Desc.ConstBufferDesc.size(); i++)
  {
    mfConstBuffer tmpConstBuffer;
    m_ConstantBuffers.push_back(tmpConstBuffer);
    for (int i = 0; i < m_ConstantBuffers.size(); i++)
    {
      m_ConstantBuffers[i].Init(_Desc.ConstBufferDesc[i]);
    }
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
      m_GameObject[i].Init(_Desc.RawData[i], _Desc.ModelBufferDesc);
    }
  }

  // Initialize Sampler state
  m_SamplerState.Init(_Desc.SamplerDesc);

  // Initialize Rasterizer 
  m_Rasterizer.Init(_Desc.RasterizerDesc);
}

void mfPass::Update(mfDepthStencilView & _DepthStencilView, const void * _Resource,  float _Time)
{
  // Set Render Targets
  mfGraphic_API::getSingleton().OMSetRenderTargets(m_gBuffer.RenderTargetsVec, _DepthStencilView);
  // Clear the render target buffers
  mfGraphic_API::getSingleton().GetDeviceContext().ClearRenderTargetView(whiteClearcolor);
  // Clear Depth Stencil View
  mfGraphic_API::getSingleton().ClearDepthStencilView(_DepthStencilView);
 // Set shader resources
  mfGraphic_API::getSingleton().GetDeviceContext().PSSetShaderResources(0, m_gBuffer.TexturesVec);
  // Set GameObject
  for (int i = 0; i < m_GameObject.size(); i++)
  {
    m_GameObject[i].Update(m_descriptor.Topology, _Time);
  }
  for (int i = 0; i < m_ConstantBuffers.size(); i++)
  {
    m_ConstantBuffers[i].Update(_Resource);
  }
  // Set Vertex Shader
  m_VertexShader.Update();
  // Set Pixel Shader
  m_PixelShader.Update();
  // Set Samplers
  m_SamplerState.Update();
  // Set Rasterizer
  m_Rasterizer.Update();
}

void mfPass::Render()
{
  m_InputLayout.Render();
  // Draw Game Object
  for (int i = 0; i < m_GameObject.size(); i++)
  {
    m_GameObject[i].Render();
  }
  for (int i = 0; i < m_ConstantBuffers.size(); i++)
  {
    m_ConstantBuffers[i].Render(0, 1, false);
  }
}

void mfPass::Destroy()
{
  // Destroy Render Targets
  for (int i = 0; i < m_gBuffer.RenderTargetsVec.size(); i++)
  {
    m_gBuffer.RenderTargetsVec[i].Destroy();
  }
  // Destroy Textures
  for (int i = 0; i < m_gBuffer.TexturesVec.size(); i++)
  {
    m_gBuffer.TexturesVec[i].Destroy();
  }
  // Destroy Game Objects
  for (int i = 0; i < m_GameObject.size(); i++)
  {
    m_GameObject[i].Destroy();
  }
  for (int i = 0; i < m_ConstantBuffers.size(); i++)
  {
    m_ConstantBuffers[i].Destroy();
  }
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
