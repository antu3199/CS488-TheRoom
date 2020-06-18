-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

-- Create the top level root node named 'root'.
rootNode = gr.node('root')

-- Create a GeometryNode with MeshId = 'cube', and name = 'torso'.
-- MeshId's must reference a .obj file that has been previously loaded into
-- the MeshConsolidator instance within A5::init().

originNode = gr.node('origin')
originNode:translate(0, 0, -5)




cubeJoint = gr.joint('CubeJoint', {-45, 0, 45}, {-45, 0, 45});


cubeMesh = gr.mesh('uvcube', 'name-of-cube')
cubeMesh:scale(1.0, 1.0, 1.0)
cubeMesh:rotate('y', 45.0)

-- Add the cubeMesh GeometryNode to the child list of rootnode.
cubeJoint:add_child(cubeMesh)
originNode:add_child(cubeJoint)


cubeMesh2 = gr.mesh('uvcube', 'name-of-cube')
cubeMesh2:translate(3.0, 0.0, 0.0)
originNode:add_child(cubeMesh2)

cubeMesh3 = gr.mesh('uvcube', 'name-of-cube')
cubeMesh3:translate(-3.0, 0.0, 0.0)
originNode:add_child(cubeMesh3)

textureMat1 = gr.standard_material("wall.jpg", "bump4.jpg", {1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 10.0)
cubeMesh:set_standard_material(textureMat1)

textureMat2 = gr.standard_material("wall2.jpg", "bump4.jpg", {0.8, 0.2, 0.2}, {0.9, 0.9, 0.9}, 25.0)
cubeMesh2:set_standard_material(textureMat2)


textureMat3 = gr.standard_material("null", "bump2.jpg", {0.8, 0.2, 0.2}, {0.9, 0.9, 0.9}, 25.0)
cubeMesh3:set_standard_material(textureMat3)


cubeMesh = gr.mesh('uvcube', 'light')
cubeMesh:scale(0.1, 0.1, 0.1)
cubeMesh:translate(0.0, 2.0, 0.0)
originNode:add_child(cubeMesh)

rootNode:add_child(originNode)

-- Return the root with all of it's childern.  The SceneNode A5::m_rootNode will be set
-- equal to the return value from this Lua script.
return rootNode
