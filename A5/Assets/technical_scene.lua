-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

-- Create the top level root node named 'root'.
rootNode = gr.node('root')

-- Create a GeometryNode with MeshId = 'cube', and name = 'torso'.
-- MeshId's must reference a .obj file that has been previously loaded into
-- the MeshConsolidator instance within A5::init().

noMat = gr.standard_material("uvShader", "null", "null", {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0.0)

function createRoom()
  roomContainer = gr.node('root')
  textureMat1 = gr.standard_material("uvShader", "room/tex/floorNew.jpg", "room/tex/NormalMap.jpg", {1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 100)

  floorMesh = gr.mesh("Floor_Plane.012", "floor")
  floorMesh:set_standard_material(textureMat1)
  roomContainer:add_child(floorMesh)
  
  textureMat2 = gr.standard_material("uvShader", "room/tex/coatOfArms.jpg", "null", {1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 10.0)
  myMesh = gr.mesh("Template.006_Template.007", "fireplacetop")
  myMesh:set_standard_material(textureMat2)
  roomContainer:add_child(myMesh)
  
  textureMat3 = gr.standard_material("uvShader", "room/tex/coatOfArmsOthers.tga", "null", {1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 10.0)
  myMesh = gr.mesh("Cube.001", "fireplaceborder")
  myMesh:set_standard_material(textureMat3)
  roomContainer:add_child(myMesh)
  
  
  textureMat4 = gr.standard_material("uvShader", "room/tex/aoc.tga", "null", {1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 10.0)
  myMesh = gr.mesh("Template.005", "fireplace1")
  myMesh:set_standard_material(textureMat4)
  roomContainer:add_child(myMesh)
  
  
  textureMat5 = gr.standard_material("uvShader", "room/tex/chimney.tga", "null", {1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 10.0)
  myMesh = gr.mesh("Template.003", "chimney")
  myMesh:set_standard_material(textureMat5)
  roomContainer:add_child(myMesh)

  return roomContainer;
end

-- Create room
room = createRoom()
rootNode:add_child(room)


-- Create lights

cubeMesh = gr.light('light', {1.0, 1.0, 1.0}, 1.0)
cubeMesh:scale(0.1, 0.1, 0.1)
cubeMesh:translate(0.0, 0.0, -3.0)
rootNode:add_child(cubeMesh)

light2 = gr.light('light', {1.0, 1.0, 1.0}, 1.0)
light2:translate(1.0, 0.0, -2.0)
rootNode:add_child(light2)


-- Create animation
tester = gr.frame_translate(1.0, 2.0, {0.0, 0.0, -3.0}, {2.0, 0.0, -3.0})
cubeMesh:set_frame(tester)

tester2 = gr.frame_translate(2.0, 3.0, {2.0, 0.0, -3.0}, {-2.0, 0.0, -3.0})
cubeMesh:set_frame(tester2)

tester3 = gr.frame_translate(3.0, 4.0, {-2.0, 0.0, -3.0}, {0.0, 0.0, -3.0})
cubeMesh:set_frame(tester3)
cubeMesh:anim_loop(0.0, 9999)

ps2 = gr.particle_system("generic_ps", "room/tex/floorNew.jpg")
cubeMesh:set_particle_system(ps2)

soundFrame = gr.frame_sound(1.0, "Sound/bell.wav", 0);
cubeMesh:set_frame(soundFrame);

cubeMesh2 = gr.mesh('uvcube', 'light2')
cubeMesh2:scale(0.1, 0.1, 0.1)
cubeMesh2:translate(0.0, 0.5, -3.0)
rootNode:add_child(cubeMesh2)
tester4 = gr.frame_rotate(1.0, 3.0, 'z', 0.0, 360.0)
cubeMesh2:set_frame(tester4)
tester6 = gr.frame_scale(1.0, 2.0, {0.1, 0.1, 0.1}, {0.3, 0.3, 0.3})
cubeMesh2:set_frame(tester6)
cubeMesh2:anim_loop(0.0, 9999)
cubeMesh2:set_standard_material(noMat)


cubeMesh3 = gr.mesh('uvcube', 'light3')
cubeMesh3:scale(0.1, 0.1, 0.1)
cubeMesh3:translate(0.0, 1.0, -3.0)
rootNode:add_child(cubeMesh3)
tester5 = gr.frame_scale(1.0, 2.0, {0.1, 0.1, 0.1}, {0.3, 0.3, 0.3})
cubeMesh3:set_frame(tester5)
cubeMesh3:anim_loop(0.0, 9999)
cubeMesh3:set_standard_material(noMat)

cubeMesh4 = gr.mesh('uvcube', 'light4')
cubeMesh4:scale(0.05, 0.05, 0.05)
cubeMesh4:translate(0.0, 0.0, -3.0)
rootNode:add_child(cubeMesh4)
cubeMesh4:set_standard_material(noMat)

-- Create perlin noise
planeMesh = gr.mesh("plane", "tv screen")
planeMesh:rotate('x', 86.69)
planeMesh:translate(-3, 0, -1.6)
tvMat = gr.standard_material("noiseShader", "null", "null", {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0.0)
planeMesh:set_standard_material(tvMat)
rootNode:add_child(planeMesh)

-- Create cel shading
cubeMesh5 = gr.mesh('Sphere', 'toon cube')
cubeMesh5:scale(0.1, 0.1, 0.1)
cubeMesh5:translate(0.5, 0.0, -2.0)
toonShader =  gr.standard_material("toonShader", "null", "room/tex/NormalMap.jpg", {1.0, 0.0, 0.0}, {0.8, 0.8, 0.8}, 10)
cubeMesh5:set_standard_material(toonShader)
rootNode:add_child(cubeMesh5)

-- Return the root with all of it's childern.  The SceneNode A5::m_rootNode will be set
-- equal to the return value from this Lua script.
return rootNode
