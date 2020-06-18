-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

-- Create the top level root node named 'root'.
rootNode = gr.node('root')

-- Create a GeometryNode with MeshId = 'cube', and name = 'torso'.
-- MeshId's must reference a .obj file that has been previously loaded into
-- the MeshConsolidator instance within A5::init().
cubeMesh = gr.mesh('cube', 'name-of-cube')
cubeMesh:scale(1.0, 1.0, 1.0)
cubeMesh:rotate('y', 45.0)
cubeMesh:translate(0.0, 0, 0)
cubeMesh:set_material(gr.material({0.8, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))

-- Add the cubeMesh GeometryNode to the child list of rootnode.
rootNode:add_child(cubeMesh)

-- Create a GeometryNode with MeshId = 'sphere', and name = 'head'.
sphereMesh = gr.mesh('sphere', 'name-of-sphere')
sphereMesh:scale(0.5, 0.5, 0.5)
sphereMesh:translate(0.0, 0.5, 0)
sphereMesh:set_material(gr.material({0.4, 0.8, 0.4}, {0.8, 0.8, 0.8}, 50.0))

-- Add the sphereMesh GeometryNode to the child list of rootnode.
rootNode:add_child(sphereMesh)

cube2Joint = gr.joint('cube2-joint', {-10, 0, 10}, {-10, 0, 10});
cube2Joint:scale(0.5, 0.5, 0.5)
cube2Joint:translate(1.0, 0.5, 0)

cubeMesh2 = gr.mesh('cube', 'name-of-cube2')
cubeMesh2:set_material(gr.material({0.75, 0.75, 0.75}, {0.8, 0.8, 0.8}, 10.0))

cubeMesh3 = gr.mesh('cube', 'name-of-cube3')
cubeMesh3:set_material(gr.material({0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, 10.0))
cubeMesh3:translate(1.0, 1.0, 0)
cubeMesh2:add_child(cubeMesh3)

cube2Joint:add_child(cubeMesh2)

cubeMesh2_2 = gr.mesh('cube', 'name-of-cube2_2')
cubeMesh2_2:set_material(gr.material({0.6, 0.6, 0.6}, {0.8, 0.8, 0.8}, 10.0))
cubeMesh2_2:translate(1.5, 0, 0)
cube2Joint:add_child(cubeMesh2_2)



rootNode:add_child(cube2Joint)

-- Return the root with all of it's childern.  The SceneNode A5::m_rootNode will be set
-- equal to the return value from this Lua script.
return rootNode
