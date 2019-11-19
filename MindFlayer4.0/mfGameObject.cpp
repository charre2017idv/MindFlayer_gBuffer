#include "mfGameObject.h"



mfGameObject::mfGameObject()
{
}


mfGameObject::~mfGameObject()
{
}

void mfGameObject::Init(mfLoadModelID _RawData, mfBufferDesc _ModelDesc)
{
  // Initialize Vertex Buffer & Index Buffer
  m_Mesh.Init(_RawData);
  // Initialize Model Const Buffer
  m_ModelBuffer.Init(_ModelDesc);
  // Allocate Index Size of Mesh
  m_IndexSize = _RawData.IndexSize;
}

void mfGameObject::Update(mf_PRIMITIVE_TOPOLOGY _Topology)
{
  // Update Model Data
  m_ModelBuffer.Update(&m_Model);
  // Set Mesh
  m_Mesh.Update();
  // Set Topology
  mfGraphic_API::getSingleton().IASetPrimitiveTopology(_Topology);
  // Set Model Buffer
  m_ModelBuffer.Render(2, 1, true);
}

void mfGameObject::Render()
{
  mfGraphic_API::getSingleton().DrawIndexed(m_IndexSize, 0, 0);
}

void mfGameObject::Destroy()
{
  m_Mesh.Destroy();
  m_ModelBuffer.Destroy();
}
