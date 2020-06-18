#include "MyMeshConsolidator.hpp"
using namespace glm;
using namespace std;

#include "cs488-framework/Exception.hpp"
#include "cs488-framework/ObjFileDecoder.hpp"

#include <iostream>
#include <glm/gtx/io.hpp>

//----------------------------------------------------------------------------------------
// Default constructor
MeshConsolidator::MeshConsolidator()
{
}

//----------------------------------------------------------------------------------------
// Destructor
MeshConsolidator::~MeshConsolidator()
{
}

//----------------------------------------------------------------------------------------
template <typename T>
static void appendVector(
    std::vector<T> &dest,
    const std::vector<T> &source)
{
  // Increase capacity to hold source.size() more elements
  dest.reserve(dest.size() + source.size());

  dest.insert(dest.end(), source.begin(), source.end());
}

//----------------------------------------------------------------------------------------
MeshConsolidator::MeshConsolidator(
    std::initializer_list<ObjFilePath> objFileList)
{

  MeshId meshId;
  vector<vec3> positions;
  vector<vec3> normals;
  vector<vec2> uvs;
  vector<vec3> tangents;
  vector<vec3> bitangents;
  BatchInfo batchInfo;
  unsigned long indexOffset(0);

  for (const ObjFilePath &objFile : objFileList)
  {
    positions.clear();
    normals.clear();
    uvs.clear();
    tangents.clear();
    bitangents.clear();
    std::cout << "Loading: " << " " << objFile << std::endl;
    ObjFileDecoder::decode(objFile.c_str(), meshId, positions, normals, uvs);
    std::cout << "Got values: " << " " << objFile << std::endl;

    uint numIndices = positions.size();
    if (numIndices != normals.size())
    {
      throw Exception("Error within MeshConsolidator: "
                      "positions.size() != normals.size()\n");
    }

    if (uvs.size() == 0)
    {
      for (int i = 0; i < positions.size(); i++) {
        uvs.emplace_back(glm::vec2(0.0f, 0.0f));
      }
    }
    
    for (int i = 0; i < positions.size(); i += 3)
    {
      glm::vec3 &v0 = positions[i + 0];
      glm::vec3 &v1 = positions[i + 1];
      glm::vec3 &v2 = positions[i + 2];

      glm::vec2 &uv0 = uvs[i + 0];
      glm::vec2 &uv1 = uvs[i + 1];
      glm::vec2 &uv2 = uvs[i + 2];

      // Edges of the triangle : position delta
      glm::vec3 deltaPos1 = v1 - v0;
      glm::vec3 deltaPos2 = v2 - v0;

      // UV delta
      glm::vec2 deltaUV1 = uv1 - uv0;
      glm::vec2 deltaUV2 = uv2 - uv0;

      float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
      glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
      glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
      tangents.push_back(tangent);
      tangents.push_back(tangent);
      tangents.push_back(tangent);

      // Same thing for bitangents
      bitangents.push_back(bitangent);
      bitangents.push_back(bitangent);
      bitangents.push_back(bitangent);
      
    }

    batchInfo.startIndex = indexOffset;
    batchInfo.numIndices = numIndices;

    m_batchInfoMap[meshId] = batchInfo;

    appendVector(m_vertexPositionData, positions);
    appendVector(m_vertexNormalData, normals);
    appendVector(m_vertexUVData, uvs);
    appendVector(m_vertexTangentData, tangents);
    appendVector(m_vertexBitangentData, bitangents);

    indexOffset += numIndices;
  }
}

//----------------------------------------------------------------------------------------
void MeshConsolidator::getBatchInfoMap(
    BatchInfoMap &batchInfoMap) const
{
  batchInfoMap = m_batchInfoMap;
}

//----------------------------------------------------------------------------------------
// Returns the starting memory location for vertex position data.
const float *MeshConsolidator::getVertexPositionDataPtr() const
{
  return &(m_vertexPositionData[0].x);
}

//----------------------------------------------------------------------------------------
// Returns the starting memory location for vertex normal data.
const float *MeshConsolidator::getVertexNormalDataPtr() const
{
  return &(m_vertexNormalData[0].x);
}

//----------------------------------------------------------------------------------------
// Returns the starting memory location for vertex uv data.
const float *MeshConsolidator::getVertexUVDataPtr() const
{
  return &(m_vertexUVData[0].x);
}

const float *MeshConsolidator::getVertexTangentDataPtr() const
{
  return &(m_vertexTangentData[0].x);
}

const float *MeshConsolidator::getVertexBitangentDataPtr() const
{
  return &(m_vertexBitangentData[0].x);
}

//----------------------------------------------------------------------------------------
// Returns the total number of bytes of all vertex position data.
size_t MeshConsolidator::getNumVertexPositionBytes() const
{
  return m_vertexPositionData.size() * sizeof(vec3);
}

//----------------------------------------------------------------------------------------
// Returns the total number of bytes of all vertex normal data.
size_t MeshConsolidator::getNumVertexNormalBytes() const
{
  return m_vertexNormalData.size() * sizeof(vec3);
}

//----------------------------------------------------------------------------------------
// Returns the total number of bytes of all vertex normal data.
size_t MeshConsolidator::getNumVertexUVBytes() const
{
  return m_vertexUVData.size() * sizeof(vec2);
}

size_t MeshConsolidator::getNumVertexTangentBytes() const
{
  return m_vertexTangentData.size() * sizeof(vec3);
}

size_t MeshConsolidator::getNumVertexBitangentBytes() const
{
  return m_vertexBitangentData.size() * sizeof(vec3);
}
