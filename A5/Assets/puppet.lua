
-- Pikachu made using spheres and cubes
-- I spent WAYYY too much time on this...

function createLeftEye()
  eye = gr.mesh('sphere2', 'EyeLeft')
  eye:set_material(blackMat)
  eye:scale(0.7320276, 0.7692921, 0.03729513)
  
  eye:rotate('z', -14.6)
  eye:rotate('y', 24.473)
  eye:rotate('x', -34.91)
  eye:translate(1.992736, 3.748168, 2.544966)

  pupil = gr.mesh('sphere2', 'PupilLeft')
  pupil:set_material(whiteMat)
  pupil:scale(0.5000001, 0.5000001, 1.003456)
  pupil:translate(-0.246, 0.252, 2.2)
  eye:add_child(pupil)
  
  return eye
end

function createRightEye()
  eye = gr.mesh('sphere2', 'EyeRight')
  eye:set_material(blackMat)
  eye:scale(0.7300758, 0.7751737, 0.03742633)
  
  eye:rotate('z', -14.6)
  eye:rotate('y', -24.89)
  eye:rotate('x', -34.91)
  eye:translate(-2.025584, 3.68819, 2.659778)
  
  pupil = gr.mesh('sphere2', 'PupilRight')
  pupil:set_material(whiteMat)
  pupil:scale(0.5000001, 0.5000001, 1.003456)
  pupil:translate(0.152, 0.351, 2.7)
  eye:add_child(pupil)

  return eye
end

function createCheekRight()
  cheek = gr.mesh('sphere2', 'CheekRight')
  cheek:set_material(redMat)
  cheek:scale(0.9074832, 0.994365, 0.03660187)
  cheek:rotate('z', -11.352)
  cheek:rotate('y', -46.387)
  cheek:rotate('x', -8.583)
  cheek:translate(-3.15, 1.88, 2.53)
  return cheek
end

function createCheekLeft()
  cheek = gr.mesh('sphere2', 'CheekLeft')
  cheek:set_material(redMat)
  cheek:scale(0.908886, 0.9959791, 0.03672051)
  cheek:rotate('z', -11.352)
  cheek:rotate('y', 46.387)
  cheek:rotate('x', -8.583)
  cheek:translate(3.21, 1.92, 2.61)
  return cheek
end

function createMouth()
  mouth = gr.node('mouth')
  mouth:translate(0.29, 2.45, 3.93)

  mouth1 = gr.mesh('cube', 'mouth1')
  mouth1:set_material(blackMat)
  mouth1:scale(0.1435264, 0.1799691, 0.9423582)
  mouth1:rotate('y', 90.00001)
  mouth1:rotate('z', 20.986)
  mouth1:translate(0.7319157, 0.09461951, 0.2474415)
  
  mouth2 = gr.mesh('cube', 'mouth2')
  mouth2:set_material(blackMat)
  mouth2:scale(0.143871, 0.1805058, 0.6435479)
  mouth2:rotate('y', 90.00001)
  mouth2:rotate('z', -20.986)
  mouth2:translate(0.05307126, 0.0826242, 0.2438533)

  mouth3 = gr.mesh('cube', 'mouth3')
  mouth3:set_material(blackMat)
  mouth3:scale(-0.1435302, 0.1799701, 0.9423608)
  mouth3:rotate('y', 90)
  mouth3:rotate('z', -20.986)
  mouth3:translate(-1.141916, 0.09461951, 0.2474415)

  mouth4 = gr.mesh('cube', 'mouth4')
  mouth4:set_material(blackMat)
  mouth4:scale(-0.14387, 0.18051, 0.6435503)
  mouth4:rotate('y', 270)
  mouth4:rotate('z', 20.986)
  mouth4:translate(-0.4630713, 0.0826242, 0.2438533)

  mouth:add_child(mouth1)
  mouth:add_child(mouth2)
  mouth:add_child(mouth3)
  mouth:add_child(mouth4)
  return mouth
end

function createNose()
  nose = gr.mesh('sphere2', 'nose')
  nose:set_material(blackMat)
  nose:scale(0.182485, 0.1863969, 0.03799299)
  nose:rotate('x', -54.263)
  nose:translate(0.02189875, 3.300335, 3.81148)
  return nose
end

function createRightEar()
  earJoint = gr.joint('EarJoint', {-135, 0, 30}, {-45, 0, 45});
  earJoint:translate(-2.2, 4.333438, -0.6801013)
  ear = gr.mesh('sphere2', 'Ear')
  ear:set_material(yellowSkin)
  ear:scale(1.116097, 3.264584, 1.012175)
  ear:rotate('z', 59.239)
  ear:rotate('y', -30.347)
  ear:translate(-1.266446, 1.531534, -0.7123073)
  earJoint:add_child(ear)

  hair = gr.mesh('sphere2', 'Hair')
  hair:set_material(blackMat)
  hair:scale(0.6669767, 2.108619, 0.8760028)
  hair:rotate('z', 61.62)
  hair:rotate('y', -37.738)
  hair:rotate('x', -3.08)
  hair:translate(-3.128096, 2.651505, -1.703372)
  earJoint:add_child(hair)

  return earJoint;
end

function createLeftEar()
  earJoint = gr.joint('EarJoint', {-135, 0, 30}, {-45, 0, 45});
  earJoint:translate(2.711726, 4.327051, -0.826044)

  ear = gr.mesh('sphere2', 'Ear')
  ear:set_material(yellowSkin)
  ear:scale(1.101941, 3.2071, 1.051225)
  ear:rotate('z', 58.954)
  ear:rotate('y', -145.589)
  ear:rotate('x', -8.007)
  ear:translate(1.182225, 1.406068, -0.5083308)
  earJoint:add_child(ear)

  hair = gr.mesh('sphere2', 'Hair')
  hair:set_material(blackMat)
  hair:scale(0.658509, 2.072209, 0.9068874)
  hair:rotate('z', 62.381)
  hair:rotate('y', -153.094)
  hair:rotate('x', -11.057)
  hair:translate(2.92367, 2.517615, -1.620095)
  earJoint:add_child(hair)

  return earJoint;
end

function createHead()
  headJoint = gr.joint('HeadJoint', {-45, 0, 45}, {-45, 0, 45});
  headJoint:translate(0, 7.444369, 0)
  
  headMesh1 = gr.mesh('sphere2', 'Head')
  headMesh1:set_material(yellowSkin)
  headMesh1:scale(3.649701, 3.998494, 3.587859)
  headMesh1:translate(0, 1.824891, -0.09928615)

  headMesh2 = gr.mesh('sphere2', 'Head')
  headMesh2:set_material(yellowSkin)
  headMesh2:rotate('x', -18.702)
  headMesh2:scale(3.649701, 2.849268, 3.636609)
  headMesh2:translate(0, 1.141149, 0.6972177)

  eyeRight = createRightEye()
  eyeLeft = createLeftEye()
  cheekRight = createCheekRight()
  cheekLeft = createCheekLeft()
  mouth = createMouth()
  nose = createNose()
  rightEar = createRightEar()
  leftEar = createLeftEar()

  headJoint:add_child(headMesh1)
  headJoint:add_child(headMesh2)
  headJoint:add_child(eyeRight)
  headJoint:add_child(eyeLeft)
  headJoint:add_child(cheekRight)
  headJoint:add_child(cheekLeft)
  headJoint:add_child(mouth)
  headJoint:add_child(nose)
  headJoint:add_child(rightEar)
  headJoint:add_child(leftEar)
  
  return headJoint
end


function createLeftArm()
  armJoint = gr.joint('ArmJoint', {-45, 0, 90}, {-45, 0, 45});
  armJoint:translate(2.335005, 5.059557, 1.117704)

  armMesh = gr.mesh('sphere2', 'ArmMesh')
  armMesh:set_material(yellowSkin)

  armMesh:scale(0.919773, 1.31387, 2.722843)
  armMesh:rotate('z', 4.092)
  armMesh:rotate('y', 29.456)
  armMesh:rotate('x', -10.34)
  armMesh:translate(0.9890695, 0.6819997, 1.273378)

  armJoint:add_child(armMesh)

  return armJoint
end

function createRightArm()
  armJoint = gr.joint('ArmJoint', {-45, 0, 90}, {-45, 0, 45});
  armJoint:translate(-2.395006, 5.059557, 1.117704)

  armMesh = gr.mesh('sphere2', 'ArmMesh')
  armMesh:set_material(yellowSkin)
  armMesh:scale(0.9160587, 1.287137, 2.662189)
  armMesh:rotate('z', -0.7180001)
  armMesh:rotate('y', -29.456)
  armMesh:rotate('x', -10.34)
  armMesh:translate(-0.989069, 0.6819997, 1.273378)

  armJoint:add_child(armMesh)

  return armJoint
end

function createRightFoot()
  footJoint = gr.joint('FootJoint', {-45, 0, 45}, {0, 0, 0});
  footJoint:translate(-2.535387, -5.184281, -0.2800007)

  footMesh = gr.mesh('sphere2', 'FootMesh')
  footMesh:set_material(yellowSkin)
  footMesh:scale(0.5838581, 0.4989315, 1.494271)
  footMesh:rotate('z', -27.317)
  footMesh:rotate('y', -2.252)
  footMesh:rotate('x', 3.315)
  footMesh:translate(0.05224276, -0.488389, 0.5089221)
  footJoint:add_child(footMesh)

  return footJoint
end


function createRightLeg()
  legJoint = gr.joint('LegJoint', {-30, 0, 30}, {0, 0, 0});
  legJoint:translate(-0.4416137, 0.9750898, 0.7561715)

  legMesh = gr.mesh('sphere2', 'LegMesh')
  legMesh:set_material(yellowSkin)
  legMesh:scale(2.29182, 3.507299, 2.416351)
  legMesh:rotate('z', 4.286)
  legMesh:rotate('y', -98.926)
  legMesh:rotate('x', -27.941)
  legMesh:translate(-1.153306, -2.2716, -0.3852881)

  legJoint:add_child(legMesh)
  foot = createRightFoot()
  legJoint:add_child(foot)
  
  return legJoint
end

function createLeftFoot()
  footJoint = gr.joint('FootJoint', {-45, 0, 45}, {0, 0, 0});
  footJoint:translate(2.154451, -5.184281, -0.2800007)

  footMesh = gr.mesh('sphere2', 'FootMesh')
  footMesh:set_material(yellowSkin)
  footMesh:scale(0.5833051, 0.5700849, 1.500414)
  footMesh:rotate('z', 4.474)
  footMesh:rotate('y', -2.252)
  footMesh:rotate('x', 3.315)
  footMesh:translate(0.1358793, -0.5059385, 0.4935758)
  footJoint:add_child(footMesh)

  return footJoint
end

function createLeftLeg()
  legJoint = gr.joint('LegJoint', {-30, 0, 30}, {0, 0, 0});
  legJoint:translate(0.4416134, 0.9750898, 0.7561715)

  legMesh = gr.mesh('sphere2', 'LegMesh')
  legMesh:set_material(yellowSkin)
  legMesh:scale(2.298528, 3.514125, 2.418531)
  legMesh:rotate('z', 4.286)
  legMesh:rotate('y', 98.926)
  legMesh:rotate('x', -27.941)
  legMesh:translate(1.153306, -2.2716, -0.3852881)

  foot = createLeftFoot()

  legJoint:add_child(legMesh)
  legJoint:add_child(foot)
  return legJoint
end

function createTail()
  tailJoint1 = gr.joint('TailJoint1', {-90, 0, 90}, {-45, 0, 0});
  tailJoint1:translate(0, -1.149722, -3.024585)
  tailMesh1 = gr.mesh('cube', 'TailMesh1')
  tailMesh1:set_material(brownMat)
  tailMesh1:scale(0.145988, 0.4560593, 2.31062)
  tailMesh1:rotate('x', 18.285)
  tailMesh1:translate(0, 0.1768596, -1.316676)
  tailJoint1:add_child(tailMesh1)

  tailJoint2 = gr.joint('TailJoint', {-45, 0, 45}, {0, 0, 0});
  tailJoint2:translate(0, 1.081256, -1.760377)
  tailMesh2 = gr.mesh('cube', 'TailMesh')
  tailMesh2:set_material(yellowSkin)
  tailMesh2:scale(0.145988, 0.9232996, 2.769707)
  tailMesh2:rotate('x', 27.079)
  tailMesh2:translate(0, 0.405973, -0.8874035)
  tailMesh2_2 = gr.mesh('cube', 'TailMesh')
  tailMesh2_2:set_material(yellowSkin)
  tailMesh2_2:scale(0.146, 0.9183609, 2.796458)
  tailMesh2_2:rotate('x', -31.71)
  tailMesh2_2:translate(0, 1.330468, -0.8224721)
  tailJoint2:add_child(tailMesh2)
  tailJoint2:add_child(tailMesh2_2)
  tailJoint1:add_child(tailJoint2)

  tailJoint3 = gr.joint('TailJoint', {-45, 0, 45}, {0, 0, 0});
  tailJoint3:translate(0, 1.764228, -0.1812539)
  tailMesh3 = gr.mesh('cube', 'TailMesh')
  tailMesh3:set_material(yellowSkin)
  tailMesh3:scale(0.1460842, 1.426769, 4.228782)
  tailMesh3:rotate('x', 39.304)
  tailMesh3:translate(0, 1.134528, -1.199543)
  tailJoint3:add_child(tailMesh3)
  tailJoint2:add_child(tailJoint3)

  tailJoint4 = gr.joint('TailJoint', {-45, 0, 45}, {0, 0, 0});
  tailJoint4:translate(0, 1.8592, -2.00457)
  tailMesh4 = gr.mesh('cube', 'TailMesh')
  tailMesh4:set_material(yellowSkin)
  tailMesh4:scale(0.146132, 1.440101, 5.316118)
  tailMesh4:rotate('x', -35.889)
  tailMesh4:translate(0, 1.288966, 1.343489)
  tailJoint4:add_child(tailMesh4)
  tailJoint3:add_child(tailJoint4)

  tailJoint5 = gr.joint('TailJoint', {-45, 0, 45}, {0, 0, 0});
  tailJoint5:translate(0, 2.310634, 2.921554)
  tailMesh5 = gr.mesh('cube', 'TailMesh')
  tailMesh5:set_material(yellowSkin)
  tailMesh5:scale(0.1460801, 2.401183, 8.302545)
  tailMesh5:rotate('x', 39.304)
  tailMesh5:translate(0, 2.24, -3.27)
  tailJoint5:add_child(tailMesh5)
  tailJoint4:add_child(tailJoint5)

  return tailJoint1

end

function createButt()
  buttJoint = gr.joint('ButtJoint', {-70, 0, 30}, {0, 0, 0});
  buttJoint:translate(-0.03000021, -2.561252, -0.8389783)
  
  buttMesh = gr.mesh('sphere2', 'Butt')
  buttMesh:set_material(yellowSkin)
  buttMesh:scale(3.6497, 3.611446, 3.600821)
  buttJoint:add_child(buttMesh)

  leftLeg = createLeftLeg()
  buttJoint:add_child(leftLeg)

  rightLeg = createRightLeg()
  buttJoint:add_child(rightLeg)

  tail = createTail()
  buttJoint:add_child(tail)


  return buttJoint
end

function createTorso()
  torsoJoint = gr.joint('TorsoJoint', {0, 0, 0}, {-50, 0, 50});
  torsoJoint:translate(0, -4.9, 0)

  torso = gr.mesh('sphere2', 'Torso')
  torso:set_material(yellowSkin)
  torso:scale(3.6497, 6.287072, 3.382656)
  torso:rotate('x', 4.74)
  torso:translate(-0.03000021, 4.949999, -0.2999997)

  backBrowning1 = gr.mesh('sphere2', 'backbrowning1')
  backBrowning1:set_material(brownMat)
  backBrowning1:scale(3.256043, 1.963722, 2.870097)
  backBrowning1:translate(-0.03000021, 6.243808, -0.7629538)
  torsoJoint:add_child(backBrowning1)

  backBrowning2 = gr.mesh('sphere2', 'backbrowning2')
  backBrowning2:set_material(brownMat)
  backBrowning2:scale(3.256043, 1.967235, 2.875299)
  backBrowning2:translate(-0.03000021, 4.790754, -0.8902287)
  torsoJoint:add_child(backBrowning2)

  head = createHead()

  rightArm = createRightArm()
  leftArm = createLeftArm()

  torsoJoint:add_child(head)
  torsoJoint:add_child(rightArm)
  torsoJoint:add_child(leftArm)

  torsoJoint:add_child(torso)
  return torsoJoint
end

rootNode = gr.node('root')

yellowSkin = gr.material({1.0, 1.0, 0.0}, {0.8, 0.8, 0.8}, 50.0)
redMat = gr.material({1.0, 0.0, 0.0}, {0.8, 0.8, 0.8}, 50.0)
blackMat = gr.material({0.0, 0.0, 0.0}, {0.8, 0.8, 0.8}, 50.0)
whiteMat = gr.material({1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 50.0)
brownMat = gr.material({0.566, 0.3456, 0.2055}, {0.8, 0.8, 0.8}, 50.0)

torso = createTorso()
butt = createButt()
rootNode:add_child(torso)
rootNode:add_child(butt)

-- Return the root with all of it's childern.  The SceneNode A5::m_rootNode will be set
-- equal to the return value from this Lua script.
return rootNode
