#include "mfGameObject.h"
#include "mfTransform.h"


mfGameObject::mfGameObject()
{
}


mfGameObject::~mfGameObject()
{
}

void mfGameObject::Init(mfLoadModelID _RawData, mfBufferDesc _ModelDesc, vector<mfBaseTextureDesc> _TexturesDesc)
{
  // Initialize Vertex Buffer & Index Buffer
  m_Mesh.Init(_RawData);
  // Initialize Model Const Buffer
  m_ModelBuffer.Init(_ModelDesc);
  // Allocate Index Size of Mesh
  m_IndexSize = _RawData.IndexSize;
  // Initialize Material
  m_Material.Init(_TexturesDesc);
}

void mfGameObject::Update(mf_PRIMITIVE_TOPOLOGY _Topology, mfTransform & _Transform, float _Time)
{
  m_Model.mWorld = _Transform.getMatrix();
  m_Model.vMeshColor = XMFLOAT4(1, 1, 1, 1.0f);
  // Update Model Data
  m_ModelBuffer.Update(&m_Model);
  
}

void mfGameObject::Render()
{
  // Set Mesh
  m_Mesh.Render();
  // Set Topology
  mfGraphic_API::getSingleton().IASetPrimitiveTopology(mf_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  // Set Model Buffer
  m_ModelBuffer.Render(1, 1, true);
  mfGraphic_API::getSingleton().DrawIndexed(m_IndexSize, 0, 0);
}

void mfGameObject::Destroy()
{
  m_Material.Destroy();
  m_Mesh.Destroy();
  m_ModelBuffer.Destroy();
}

void mfGameObject::setTexture()
{
  m_Material.setTexture();
}

void mfGameObject::setTexture(vector<mfTexture> _Textures)
{
  m_Material.setTexture(_Textures);
}

void mfGameObject::setTexture(vector<mfRenderTarget>& _RenderTargets)
{
  m_Material.setTexture(_RenderTargets);
}
