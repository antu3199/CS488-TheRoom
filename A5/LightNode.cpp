// Winter 2019

#include "LightNode.hpp"

//---------------------------------------------------------------------------------------
LightNode::LightNode(
		const std::string & name
)
	: SceneNode(name)
{
	m_nodeType = NodeType::LightNode;
}
