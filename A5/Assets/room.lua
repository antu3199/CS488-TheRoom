-- Simple Scene:
-- An extremely simple scene that will render right out of the box
-- with the provided skeleton code.  It doesn't rely on hierarchical
-- transformations.

-- Create the top level root node named 'root'.
rootNode = gr.node('root')

-- Create a GeometryNode with MeshId = 'cube', and name = 'torso'.
-- MeshId's must reference a .obj file that has been previously loaded into
-- the MeshConsolidator instance within A5::init().


aLargeNumber = 999999999
tvStaticTime = 10.0

disableStaticTvFrame = gr.frame_enabled(tvStaticTime, 0)
enableStaticTvFrame = gr.frame_enabled(tvStaticTime, 1)
enableStaticTvSoundFrame = gr.frame_sound(tvStaticTime, "Sound/static.wav", 0.5, 0)


tvTurnOffTime = tvStaticTime + 7.5
stopStaticSoundFrame = gr.frame_stop_sound(tvTurnOffTime-0.01)
disableStaticTvFrame2 = gr.frame_enabled(tvTurnOffTime, 0)
enableStaticTvFrame2 = gr.frame_enabled(tvTurnOffTime, 1)



tvDisplayScreenTime = tvTurnOffTime + 5.0
bearStartDanceTime = tvDisplayScreenTime + 2.0


disableDisplayTvFrame = gr.frame_enabled(tvDisplayScreenTime, 0)
enableDisplayTvFrame = gr.frame_enabled(tvDisplayScreenTime, 1)
enableDisplayTvMusicFrame = gr.frame_sound(tvDisplayScreenTime, "Sound/junglemusic.wav", 0.5, 0)

bearDanceTime = 0.5
danceTimeFrom = bearStartDanceTime
danceTimeTo = bearStartDanceTime + (bearDanceTime / 2.0)
bearDanceFrame1 = gr.frame_rotate(danceTimeFrom, danceTimeTo, 'z', 0.0, 45)
danceTimeFrom = danceTimeTo;
danceTimeTo = danceTimeTo + bearDanceTime
bearDanceFrame2 = gr.frame_rotate(danceTimeFrom, danceTimeTo, 'z', 45, -45)
danceTimeFrom = danceTimeTo;
danceTimeTo = danceTimeTo + (bearDanceTime / 2.0)
bearDanceFrame3 = gr.frame_rotate(danceTimeFrom, danceTimeTo, 'z', -45, 0)
bearStartDanceEndTime =  bearStartDanceTime + (danceTimeTo - bearStartDanceTime) * 10 + 0.5

bearFallStart = bearStartDanceEndTime
bearFallStopMusicFrame = gr.frame_stop_sound(bearFallStart-0.01)
bearFallSoundFrame = gr.frame_sound(bearFallStart, "Sound/fall.wav", 0.5, 0)

bearFallEnd = bearFallStart + 0.5
bearFallFrame1 = gr.frame_rotate(bearFallStart, bearFallEnd, 'x', 0, 90)
bearFallFrame2 = gr.frame_translate(bearFallStart, bearFallEnd, {0.421, 1.353166, -3.781326}, {0.421, -0.279, -3.392})

disableBearFallFrame = gr.frame_enabled(bearFallStart, 0)
enableBearFallFrame = gr.frame_enabled(bearFallStart, 1)
eventsAfterBearFall = bearFallEnd + 2.0

giggleSound = gr.frame_sound(eventsAfterBearFall, "Sound/laugh2.wav", 0.5, 0)


ghostEvents = eventsAfterBearFall + 6.0
ghostSoundEffect =  gr.frame_sound(ghostEvents, "Sound/ghost.wav", 1.0, 1)

cabinetFloatTime = 1.0
cabinetFloatFrom = ghostEvents
cabinetFloatTo = ghostEvents + cabinetFloatTime
cabinetFloatFrame1 = gr.frame_translate(cabinetFloatFrom, cabinetFloatTo, {-1.475, -0.215, -4.025}, {-1.475, 0.75, -4.025})
cabinetFloatFrom = cabinetFloatTo
cabinetStartLoop = cabinetFloatFrom
cabinetFloatTo = cabinetFloatTo + cabinetFloatTime
cabinetFloatFrame2 = gr.frame_translate(cabinetFloatFrom, cabinetFloatTo, {-1.475, 0.75, -4.025}, {-1.475, 0.5, -4.025})

cabinetFloatFrom = cabinetFloatTo
cabinetFloatTo = cabinetFloatTo + cabinetFloatTime
cabinetFloatFrame3 = gr.frame_translate(cabinetFloatFrom, cabinetFloatTo, {-1.475, 0.5, -4.025}, {-1.475, 0.75, -4.025})

chair1FloatStart = ghostEvents + 4.0
chair1FloatEnd = chair1FloatStart + 2.0

chair1FloatFrame1_1 = gr.frame_translate(chair1FloatStart, chair1FloatEnd, {-0.184, 0.159, -2.085}, {-0.1921744, 0.5831718, -2.087823})

chair1FloatFrame1_4 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'x', 0, 45)
chair1FloatFrame1_3 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'y', 0, -130)
chair1FloatFrame1_2 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'z', 0, 45)


chair1FloatStart = chair1FloatEnd
chair1FloatEnd = chair1FloatEnd + 2.0
chair1FloatLoop = chair1FloatStart

chair1FloatFrame2_1 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'x', 45, 13)
chair1FloatFrame2_2 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'y', -130, 270)
chair1FloatFrame2_3 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'z', 45, 60)
chair1FloatFrame2_4 = gr.frame_translate(chair1FloatStart, chair1FloatEnd, {-0.1921744, 0.5831718, -2.087823}, {-0.6921744, 0.8831718, -1.987823}) 

chair1FloatStart = chair1FloatEnd
chair1FloatEnd = chair1FloatEnd + 2.0
chair1FloatFrame3_1 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'x', 13, -50)
chair1FloatFrame3_2 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'y', 270, -450)
chair1FloatFrame3_3 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'z', 60, -120)
chair1FloatFrame3_4 = gr.frame_translate(chair1FloatStart, chair1FloatEnd, {-0.6921744, 0.8831718, -1.987823}, {-0.1921744, 0.5831718, -2.087823}) 

chair1FloatStart = chair1FloatEnd
chair1FloatEnd = chair1FloatEnd + 2.0
chair1FloatFrame4_1 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'x', -50, 45)
chair1FloatFrame4_2 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'y', -450, -130)
chair1FloatFrame4_3 = gr.frame_rotate(chair1FloatStart, chair1FloatEnd, 'z', -120, 45)


-- Chair 2 float

chair2FloatStart = ghostEvents + 7.0
chair2FloatEnd = chair2FloatStart + 2.0

chair2FloatFrame1_1 = gr.frame_translate(chair2FloatStart, chair2FloatEnd, {1.243, 0.083, -2.311}, {1.421744, 0.789, -2.57823})

chair2FloatFrame1_4 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'x', 0, 45)
chair2FloatFrame1_3 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'y', 0, -130)
chair2FloatFrame1_2 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'z', 0, 45)

chair2FloatStart = chair2FloatEnd
chair2FloatEnd = chair2FloatEnd + 2.0
chair2FloatLoop = chair2FloatStart

chair2FloatFrame2_1 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'x', 45, 13)
chair2FloatFrame2_2 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'y', -130, 270)
chair2FloatFrame2_3 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'z', 45, 60)
chair2FloatFrame2_4 = gr.frame_translate(chair2FloatStart, chair2FloatEnd, {1.421744, 0.789, -2.57823}, {1.521744, 0.589, -2.07823}) 


chair2FloatStart = chair2FloatEnd
chair2FloatEnd = chair2FloatEnd + 2.0
chair2FloatFrame3_1 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'x', 13, 405)
chair2FloatFrame3_2 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'y', 270, 490)
chair2FloatFrame3_3 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'z', 60, 420)
chair2FloatFrame3_4 = gr.frame_translate(chair2FloatStart, chair2FloatEnd, {1.521744, 0.589, -2.07823}, {1.421744, 0.789, -2.57823}) 

chair2FloatStart = chair2FloatEnd
chair2FloatEnd = chair2FloatEnd + 2.0
chair2FloatFrame4_1 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'x', 405, 45)
chair2FloatFrame4_2 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'y', 490, -130)
chair2FloatFrame4_3 = gr.frame_rotate(chair2FloatStart, chair2FloatEnd, 'z', 420, 45)


tableResizeStart = ghostEvents + 10.0
tableResizeLoop = tableResizeStart
tableResizeDuration = 2.0
tableResizeEnd = tableResizeStart + tableResizeDuration
tableResizeFrame1 = gr.frame_scale(tableResizeStart, tableResizeEnd, {1.0, 1.0, 1.0}, {2.0, 1.0, 1.0})
tableResizeStart = tableResizeEnd
tableResizeEnd = tableResizeStart + tableResizeDuration
tableResizeFrame2 = gr.frame_scale(tableResizeStart, tableResizeEnd, {2.0, 1.0, 1.0}, {1.0, 1.0, 1.0})
tableResizeStart = tableResizeEnd
tableResizeEnd = tableResizeStart + tableResizeDuration
tableResizeFrame3 = gr.frame_scale(tableResizeStart, tableResizeEnd, {1.0, 1.0, 1.0}, {1.0, 1.0, 2.0})
tableResizeStart = tableResizeEnd
tableResizeEnd = tableResizeStart + tableResizeDuration
tableResizeFrame4 = gr.frame_scale(tableResizeStart, tableResizeEnd, {1.0, 1.0, 2.0}, {1.0, 1.0, 1.0})
tableResizeStart = tableResizeEnd
tableResizeEnd = tableResizeStart + tableResizeDuration
tableResizeFrame5 = gr.frame_scale(tableResizeStart, tableResizeEnd, {1.0, 1.0, 1.0}, {2.0, 1.0, 2.0})
tableResizeStart = tableResizeEnd
tableResizeEnd = tableResizeStart + tableResizeDuration
tableResizeFrame6 = gr.frame_scale(tableResizeStart, tableResizeEnd, {2.0, 1.0, 2.0}, {1.0, 1.0, 1.0})

endTvFlickerStart = ghostEvents + 4.0
endTvFlickerLoop = endTvFlickerStart
-- Show tv static
endTvFlickerDisabledFrame1 = gr.frame_enabled(endTvFlickerStart, 0)
endTvFlickerEnabledFrame1 = gr.frame_enabled(endTvFlickerStart, 1)
endTvFlickerStartSound1 = gr.frame_sound(endTvFlickerStart, "Sound/static.wav", 0.5, 0)

-- Hide tv static
endTvFlickerStart = endTvFlickerStart + 1.0
endTvFlickerDisabledFrame2 = gr.frame_enabled(endTvFlickerStart, 0)
endTvFlickerEnabledFrame2 = gr.frame_enabled(endTvFlickerStart, 1)
endTvFlickerStartSound2 = gr.frame_stop_sound(endTvFlickerStart)

-- Show tv static
endTvFlickerStart = endTvFlickerStart + 3.0
endTvFlickerDisabledFrame3 = gr.frame_enabled(endTvFlickerStart, 0)
endTvFlickerEnabledFrame3 = gr.frame_enabled(endTvFlickerStart, 1)
endTvFlickerStartSound3 = gr.frame_sound(endTvFlickerStart, "Sound/static.wav", 0.5, 0)

-- Hide tv static
endTvFlickerStart = endTvFlickerStart + 5.0
endTvFlickerDisabledFrame4 = gr.frame_enabled(endTvFlickerStart, 0)
endTvFlickerEnabledFrame4 = gr.frame_enabled(endTvFlickerStart, 1)
endTvFlickerStartSound4 = gr.frame_stop_sound(endTvFlickerStart-0.01)

-- Show tv Static
endTvFlickerStart = endTvFlickerStart + 1.5
endTvFlickerDisabledFrame5 = gr.frame_enabled(endTvFlickerStart, 0)
endTvFlickerEnabledFrame5 = gr.frame_enabled(endTvFlickerStart, 1)
endTvFlickerStartSound5 = gr.frame_sound(endTvFlickerStart, "Sound/static.wav", 0.5, 0)

-- Hide tv static
endTvFlickerStart = endTvFlickerStart + 1.0
endTvFlickerDisabledFrame6 = gr.frame_enabled(endTvFlickerStart, 0)
endTvFlickerEnabledFrame6 = gr.frame_enabled(endTvFlickerStart, 1)
endTvFlickerStartSound6 = gr.frame_stop_sound(endTvFlickerStart)

noMat = gr.standard_material("uvShader", "null", "null", {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0.0)
disableAtStart = gr.frame_enabled(0, 0)


function createRoom()
  roomContainer = gr.node('room')
  floorNew = gr.standard_material("uvShader", "room/tex/floorNew.jpg", "room/tex/NormalMap.jpg", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 100)
  coatOfArms = gr.standard_material("uvShader", "room/tex/coatOfArms.jpg", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  coatOfArmsOthers = gr.standard_material("uvShader", "room/tex/coatOfArmsOthers.tga", "room/tex/coatOfArmsNormal.jpg", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  aoc = gr.standard_material("uvShader", "room/tex/aoc.tga", "room/tex/aocNormal.jpg", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  chimney = gr.standard_material("uvShader", "room/tex/chimney.tga", "room/tex/chimneyNormal.jpg", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  burnt = gr.standard_material("uvShader", "room/tex/burnt.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10000000.0)
  fireplaceMarble = gr.standard_material("uvShader", "room/tex/fireplaceMarble.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  fireplaceMetal = gr.standard_material("uvShader", "room/tex/fireplaceMetal.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  blackMetal = gr.standard_material("uvShader", "room/tex/blackMetal.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  logs = gr.standard_material("uvShader", "room/tex/logs.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  whiteMat = gr.standard_material("uvShader", "null", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  carpet = gr.standard_material("uvShader", "room/tex/carpet2.jpg", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  chairWood = gr.standard_material("uvShader", "room/tex/chairWood.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  chairFabric = gr.standard_material("uvShader", "room/tex/chairFabric.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  table = gr.standard_material("uvShader", "room/tex/table.jpg", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  halfSmallWall = gr.standard_material("uvShader", "room/tex/HalfSmallWall.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  bigWallRight = gr.standard_material("uvShader", "room/tex/bigWallRight.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  leftBigWall = gr.standard_material("uvShader", "room/tex/leftBigWall.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  bigWallLeft = gr.standard_material("uvShader", "room/tex/bigWallLeft.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  borders = gr.standard_material("uvShader", "room/tex/borders.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  leftPillar = gr.standard_material("uvShader", "room/tex/leftPillar.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  pillars = gr.standard_material("uvShader", "room/tex/pillars.tga", "room/tex/pillarsNormal.jpg", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  borders2Bup = gr.standard_material("uvShader", "room/tex/borders2Bup.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  chairTop = gr.standard_material("uvShader", "room/tex/chairTop.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  armory = gr.standard_material("uvShader", "room/tex/armory.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  pillarsBottom = gr.standard_material("uvShader", "room/tex/pillarsBottom.tga", "room/tex/pillarsBottomNormal.jpg", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)
  smallDoor = gr.standard_material("uvShader", "room/tex/smallDoor.tga", "null", {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, 10.0)

  floorMesh = gr.mesh("Floor_Plane.012", "floor")
  floorMesh:set_standard_material(floorNew)
  roomContainer:add_child(floorMesh)
  
  myMesh = gr.mesh("Template.006_Template.007", "fireplacetop")
  myMesh:set_standard_material(coatOfArms)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Cube.001", "fireplaceborder")
  myMesh:set_standard_material(coatOfArmsOthers)
  roomContainer:add_child(myMesh)
  
  myMesh = gr.mesh("Template.005", "fireplace1")
  myMesh:set_standard_material(aoc)
  roomContainer:add_child(myMesh)
  
  myMesh = gr.mesh("Template.003", "chimney")
  myMesh:set_standard_material(chimney)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Circle.015", "fireplace2")
  myMesh:set_standard_material(aoc)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Circle.010_Circle.004", "firewood")
  myMesh:set_standard_material(burnt)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Cube_Cube.003", "fireplace stand")
  myMesh:set_standard_material(fireplaceMarble)
  roomContainer:add_child(myMesh)


  myMesh = gr.mesh("Cylinder_Cylinder.001", "fireplace stand")
  myMesh:set_standard_material(fireplaceMetal)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.022_Plane.008", "metal stuff")
  myMesh:set_standard_material(blackMetal)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Circle.014_Circle.016", "left fire wood")
  myMesh:set_standard_material(logs)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.008_Plane.006", "candle1")
  myMesh:set_standard_material(whiteMat)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.023_Plane.015", "candle2")
  myMesh:set_standard_material(whiteMat)
  roomContainer:add_child(myMesh)
  
  myMesh = createChair1(chairFabric, chairTop, chairWood)
  roomContainer:add_child(myMesh)
 -- myMesh = gr.mesh("Plane.006_Plane.003", "carpet")
 -- myMesh:set_standard_material(carpet)
 -- roomContainer:add_child(myMesh)


  myMesh = createChair2(chairFabric, chairTop, chairWood)
  roomContainer:add_child(myMesh)


  tableMesh = createTable(table)
  roomContainer:add_child(tableMesh)





  myMesh = gr.mesh("HalfSmallWall", "half wall")
  myMesh:set_standard_material(halfSmallWall)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane", "right plane")
  myMesh:set_standard_material(bigWallRight)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.001", "left plane")
  myMesh:set_standard_material(leftBigWall)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.002", "left plane 2")
  myMesh:set_standard_material(bigWallLeft)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.007_Plane.038", "top1")
  myMesh:set_standard_material(borders)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.013_Plane.009", "leftpillar")
  myMesh:set_standard_material(leftPillar)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.016", "pillars")
  myMesh:set_standard_material(pillars)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.021_Plane.010", "borders2Bup")
  myMesh:set_standard_material(borders2Bup)
  roomContainer:add_child(myMesh)

  
  cabinet = createCabinet(armory)
  roomContainer:add_child(cabinet)


  myMesh = gr.mesh("Plane.042_Plane.048", "pillarstand")
  myMesh:set_standard_material(pillarsBottom)
  roomContainer:add_child(myMesh)

  myMesh = gr.mesh("SmallDoor", "small door")
  myMesh:set_standard_material(smallDoor)
  roomContainer:add_child(myMesh)



  
  return roomContainer;
end

function createTable(table)
  tableContainer = gr.node("table")
  tableContainer:translate(0.522, -0.038, -2.548)

  tableMesh = gr.mesh("Circle.009_Circle.012", "table")
  tableMesh:translate(-0.522, 0.038, 2.548)
  tableMesh:set_standard_material(table)
  tableContainer:add_child(tableMesh)

  tableContainer:set_frame(tableResizeFrame1)
  tableContainer:set_frame(tableResizeFrame2)
  tableContainer:set_frame(tableResizeFrame3)
  tableContainer:set_frame(tableResizeFrame4)
  tableContainer:set_frame(tableResizeFrame5)
  tableContainer:set_frame(tableResizeFrame6)
  tableContainer:anim_loop(tableResizeLoop, aLargeNumber)

  return tableContainer
end

function createCabinet(armory)
  cabinetContainer = gr.node("cabinet")
  cabinetContainer:translate(-1.475, -0.215, -4.025)

  cabinet = gr.mesh("Plane.028_Plane.026", "cabinet")
  cabinet:set_standard_material(armory)
  cabinet:translate(1.475, 0.215, 4.025)
  cabinetContainer:add_child(cabinet)


  cabinetContainer:set_frame(cabinetFloatFrame1)
  cabinetContainer:set_frame(cabinetFloatFrame2)
  cabinetContainer:set_frame(cabinetFloatFrame3)
  cabinetContainer:anim_loop(cabinetStartLoop, aLargeNumber)
   
  return cabinetContainer
end

function createChair1(chairFabric, chairTop, chairWood)
  chairContainer = gr.node("chair")
  chairContainer:translate(-0.184, 0.159, -2.085)

  myMesh = gr.mesh("Cube.008_Cube.009", "chairfabric2")
  myMesh:set_standard_material(chairFabric)
  myMesh:translate(0.184, -0.159, 2.085)
  chairContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.026_Plane.023", "chairTop1")
  myMesh:set_standard_material(chairTop)
  myMesh:translate(0.184, -0.159, 2.085)
  chairContainer:add_child(myMesh)

  myMesh = gr.mesh("BezierCircle.003_Mesh.001", "chair1")
  myMesh:set_standard_material(chairWood)
  myMesh:translate(0.184, -0.159, 2.085)
  chairContainer:add_child(myMesh)

  chairContainer:set_frame(chair1FloatFrame1_1)
  chairContainer:set_frame(chair1FloatFrame1_2)
  chairContainer:set_frame(chair1FloatFrame1_3)
  chairContainer:set_frame(chair1FloatFrame1_4)
  chairContainer:set_frame(chair1FloatFrame2_1)
  chairContainer:set_frame(chair1FloatFrame2_2)
  chairContainer:set_frame(chair1FloatFrame2_3)
  chairContainer:set_frame(chair1FloatFrame2_4)
  chairContainer:set_frame(chair1FloatFrame3_1)
  chairContainer:set_frame(chair1FloatFrame3_2)
  chairContainer:set_frame(chair1FloatFrame3_3)
  chairContainer:set_frame(chair1FloatFrame3_4)
  chairContainer:set_frame(chair1FloatFrame4_1)
  chairContainer:set_frame(chair1FloatFrame4_2)
  chairContainer:set_frame(chair1FloatFrame4_3)
  chairContainer:anim_loop(chair1FloatLoop, aLargeNumber)

  return chairContainer;
end

function createChair2(chairFabric, chairTop, chairWood)
  chairContainer = gr.node("chair2")
  chairContainer:translate(1.243, 0.083, -2.311)

  myMesh = gr.mesh("Cube.003_Cube.023", "chairfabric1")
  myMesh:set_standard_material(chairFabric)
  myMesh:translate(-1.243, -0.083, 2.311)
  chairContainer:add_child(myMesh)

  myMesh = gr.mesh("Plane.024_Plane.030", "chairTop2")
  myMesh:set_standard_material(chairTop)
  myMesh:translate(-1.243, -0.083, 2.311)
  chairContainer:add_child(myMesh)

  myMesh = gr.mesh("BezierCircle.005_Mesh.004", "chair2")
  myMesh:set_standard_material(chairWood)
  myMesh:translate(-1.243, -0.083, 2.311)
  chairContainer:add_child(myMesh)


  chairContainer:set_frame(chair2FloatFrame1_1)
  chairContainer:set_frame(chair2FloatFrame1_2)
  chairContainer:set_frame(chair2FloatFrame1_3)
  chairContainer:set_frame(chair2FloatFrame1_4)
  chairContainer:set_frame(chair2FloatFrame2_1)
  chairContainer:set_frame(chair2FloatFrame2_2)
  chairContainer:set_frame(chair2FloatFrame2_3)
  chairContainer:set_frame(chair2FloatFrame2_4)
  chairContainer:set_frame(chair2FloatFrame3_1)
  chairContainer:set_frame(chair2FloatFrame3_2)
  chairContainer:set_frame(chair2FloatFrame3_3)
  chairContainer:set_frame(chair2FloatFrame3_4)
  chairContainer:set_frame(chair2FloatFrame4_1)
  chairContainer:set_frame(chair2FloatFrame4_2)
  chairContainer:set_frame(chair2FloatFrame4_3)

  chairContainer:anim_loop(chair2FloatLoop, aLargeNumber)

  return chairContainer;
end




function createWalls()
  wallContainer = gr.node('room')
  wallMat = gr.standard_material("uvShader", "objModels/tex/wood.jpg", "room/tex/NormalMap.jpg", {1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 100)
  planeMesh1 = gr.mesh("plane", "planeforward")
  planeMesh1:scale(4.337243, 1, 3)
  planeMesh1:rotate('x', 90)
  planeMesh1:translate(0, 2.34, -4.43)
  planeMesh1:set_standard_material(wallMat)
  wallContainer:add_child(planeMesh1)
  
  planeMesh = gr.mesh("plane", "planeright")
  planeMesh:scale(2.609632, 1, 3)
  planeMesh:rotate('x', 90)
  planeMesh:rotate('y', -90)
  planeMesh:translate(4.177, 2.34, -1.934)
  planeMesh:set_standard_material(wallMat)
  wallContainer:add_child(planeMesh)

  planeMesh3 = gr.mesh("plane", "planeleft")
  planeMesh3:scale(2.609632, 1, 3)
  planeMesh3:rotate('x', 90)
  planeMesh3:rotate('y', 90)
  planeMesh3:translate(-4.177, 2.34, -1.934)
  planeMesh3:set_standard_material(wallMat)
  wallContainer:add_child(planeMesh3)

  planeMesh4 = gr.mesh("plane", "planeback")
  planeMesh4:scale(4.337243, 1, 3)
  planeMesh4:rotate('x', 90)
  planeMesh4:rotate('y', 180)
  planeMesh4:translate(0, 2.34, 0.65)
  planeMesh4:set_standard_material(wallMat)
  wallContainer:add_child(planeMesh4)

  planeMesh5 = gr.mesh("plane", "planetop")
  planeMesh5:scale(4.337243, 1, 3.219738)
  planeMesh5:rotate('z', 180)
  planeMesh5:translate(0, 5.34, -1.89)
  planeMesh5:set_standard_material(wallMat)
  wallContainer:add_child(planeMesh5)

  return wallContainer
end





function createTV()
  tvContainer = gr.node("tv")
  blackMat = gr.standard_material("uvShader", "null", "null", {0.05, 0.05, 0.05}, {0.8, 0.8, 0.8}, 1.0)
  noiseMat = gr.standard_material("noiseShader", "null", "null", {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0.0)
  whiteMat = gr.standard_material("toonShader", "objModels/tex/trees.jpg", "null", {0.8, 0.8, 0.8}, {0.0, 0.0, 0.0}, 1.0)

  tvMat = gr.standard_material("uvShader", "null", "null", {0.0, 0.0, 0.0}, {0.8, 0.8, 0.8}, 1.0)
  tvMesh = gr.mesh("TV_Extrude_NURBS_1_Cap_23", "tv stand")
  tvMesh:scale(0.001687397, 0.001643364, 0.001)
  tvMesh:translate(0.42, 1.2261, -4.03)
  tvMesh:set_standard_material(tvMat)
  tvContainer:add_child(tvMesh)

  blackPlaneMesh = gr.mesh("plane", "tv screen1")
  blackPlaneMesh:set_standard_material(blackMat)
  blackPlaneMesh:scale(219.9552, 17, 377.7287)
  blackPlaneMesh:rotate('z', -90)
  blackPlaneMesh:rotate('y', -90)
  blackPlaneMesh:translate(0, 30,-13)
  tvMesh:add_child(blackPlaneMesh)

  noiseMesh = gr.mesh("plane", "tv screen1")
  noiseMesh:scale(219.9552, 17, 377.7287)
  noiseMesh:rotate('z', -90)
  noiseMesh:rotate('y', -90)
  noiseMesh:translate(0, 30,-13)
  noiseMesh:set_standard_material(noiseMat)
  noiseMesh:set_frame(disableAtStart)

  tvMesh:add_child(noiseMesh)

  displayPlaneMesh = gr.mesh("plane", "tv screen2")
  displayPlaneMesh:set_standard_material(whiteMat)
  displayPlaneMesh:scale(219.9552, 17, 377.7287)
  displayPlaneMesh:rotate('z', -90)
  displayPlaneMesh:rotate('y', -90)
  displayPlaneMesh:translate(0, 30,-13)
  displayPlaneMesh:set_frame(disableAtStart)

  tvMesh:add_child(displayPlaneMesh)


  light2 = gr.light('light', {1.0, 1.0, 1.0}, 1.0)
  light2:set_frame(disableAtStart)
  light2:translate(0.361, 1.354, -3.472)
  tvContainer:add_child(light2)

  
  -- Initial noise effect:

  blackPlaneMesh:set_frame(disableStaticTvFrame)
  noiseMesh:set_frame(enableStaticTvFrame)
  noiseMesh:set_frame(enableStaticTvSoundFrame)
  light2:set_frame(enableStaticTvFrame)
  
  -- Noise effect -> black
  noiseMesh:set_frame(stopStaticSoundFrame)
  noiseMesh:set_frame(disableStaticTvFrame2)
  light2:set_frame(disableStaticTvFrame2)
  blackPlaneMesh:set_frame(enableStaticTvFrame2)

  -- Show cartoon:
  blackPlaneMesh:set_frame(disableDisplayTvFrame)
  displayPlaneMesh:set_frame(enableDisplayTvFrame)
  light2:set_frame(enableDisplayTvFrame)
  displayPlaneMesh:set_frame(enableDisplayTvMusicFrame)
  

  -- Turn off TV
  displayPlaneMesh:set_frame(disableBearFallFrame)
  light2:set_frame(disableBearFallFrame)
  blackPlaneMesh:set_frame(enableBearFallFrame)


  -- End static loop
  noiseMesh:set_frame(endTvFlickerEnabledFrame1)
  light2:set_frame(endTvFlickerEnabledFrame1)
  blackPlaneMesh:set_frame(endTvFlickerDisabledFrame1)
  noiseMesh:set_frame(endTvFlickerStartSound1)

  noiseMesh:set_frame(endTvFlickerDisabledFrame2)
  light2:set_frame(endTvFlickerDisabledFrame2)
  blackPlaneMesh:set_frame(endTvFlickerEnabledFrame2)
  noiseMesh:set_frame(endTvFlickerStartSound2)

  noiseMesh:set_frame(endTvFlickerEnabledFrame3)
  light2:set_frame(endTvFlickerEnabledFrame3)
  blackPlaneMesh:set_frame(endTvFlickerDisabledFrame3)
  noiseMesh:set_frame(endTvFlickerStartSound3)

  noiseMesh:set_frame(endTvFlickerDisabledFrame4)
  light2:set_frame(endTvFlickerDisabledFrame4)
  blackPlaneMesh:set_frame(endTvFlickerEnabledFrame4)
  noiseMesh:set_frame(endTvFlickerStartSound4)

  noiseMesh:set_frame(endTvFlickerEnabledFrame5)
  light2:set_frame(endTvFlickerEnabledFrame5)
  blackPlaneMesh:set_frame(endTvFlickerDisabledFrame5)
  noiseMesh:set_frame(endTvFlickerStartSound5)

  noiseMesh:set_frame(endTvFlickerDisabledFrame6)
  light2:set_frame(endTvFlickerDisabledFrame6)
  blackPlaneMesh:set_frame(endTvFlickerEnabledFrame6)
  noiseMesh:set_frame(endTvFlickerStartSound6)

  noiseMesh:anim_loop(endTvFlickerLoop, aLargeNumber)
  blackPlaneMesh:anim_loop(endTvFlickerLoop, aLargeNumber)
  light2:anim_loop(endTvFlickerLoop, aLargeNumber)

  return tvContainer
end

function createBear()
  bearContainer = gr.node("bear")
  brownMat = gr.standard_material("toonShader", "null", "bear/furNormal.jpg", {0.56, 0.34, 0.15}, {0.8, 0.8, 0.8}, 10000000.0)
  blackMat = gr.standard_material("toonShader", "null", "room/tex/NormalMap.jpg", {0.1, 0.1, 0.1}, {0.8, 0.8, 0.8}, 10000.0)
  whiteMat = gr.standard_material("toonShader", "null", "room/tex/NormalMap.jpg", {0.66, 0.66, 0.66}, {0.8, 0.8, 0.8}, 10000.0)
  bearContainer:scale(0.4, 0.4, 0.4)
  bearContainer:rotate('x', 43.944)
  bearContainer:translate(0.421, 1.353166, -3.781326)

  myMesh = gr.mesh("polySurface1", "bearbase")
  myMesh:set_standard_material(brownMat)
  bearContainer:add_child(myMesh)

  myMesh = gr.mesh("choco:pSphere3", "eye1")
  myMesh:set_standard_material(blackMat)
  bearContainer:add_child(myMesh)

  myMesh = gr.mesh("choco:pSphere2", "eye2")
  myMesh:set_standard_material(blackMat)
  bearContainer:add_child(myMesh)

  myMesh = gr.mesh("choco:pSphere1", "nose")
  myMesh:set_standard_material(whiteMat)
  bearContainer:add_child(myMesh)

  bearContainer:set_frame(disableAtStart)
  bearContainer:set_frame(enableDisplayTvFrame)

  bearContainer:set_frame(bearDanceFrame1)
  bearContainer:set_frame(bearDanceFrame2)
  bearContainer:set_frame(bearDanceFrame3)

  bearContainer:set_frame(bearFallStopMusicFrame)
  bearContainer:set_frame(bearFallSoundFrame)

  bearContainer:set_frame(bearFallFrame1)
  bearContainer:set_frame(bearFallFrame2)
  
  bearContainer:anim_loop(bearStartDanceTime, bearStartDanceEndTime)
  

  return bearContainer
end


-- Create room
room = createRoom()
rootNode:add_child(room)

-- Create lights

light = gr.light('light', {1.0, 1.0, 1.0}, 2.0)
light:translate(0.444, -0.4, -4.20)
rootNode:add_child(light)


-- Create animation
fireplace = gr.node('fireplace')
fireplace:translate(0.444, -0.4, -4.20)
rootNode:add_child(fireplace)

ps2 = gr.particle_system("generic_ps", "fire_particle.png")
fireplace:set_particle_system(ps2)

walls = createWalls()
rootNode:add_child(walls)

rootNode:add_child(wallContainer)

tv = createTV()
rootNode:add_child(tv)

bear = createBear()
rootNode:add_child(bear)

soundFrame = gr.frame_sound(0.0, "Sound/fire.wav", 5.0, 1)

room:set_frame(soundFrame);
room:set_frame(giggleSound)
room:set_frame(ghostSoundEffect);


-- Return the root with all of it's childern.  The SceneNode A5::m_rootNode will be set
-- equal to the return value from this Lua script.
return rootNode
