-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0, 0, 0.75, 1.9)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
scene_root:add_child(s2)
s2:set_material(mat1)

s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(mat2)

b1 = gr.nh_box('b1', {-200, -125, 0}, 100)
scene_root:add_child(b1)
b1:set_material(mat4)

--b1:translate(200, 125, 0)
--b1:scale(1/100, 1/100, 1/100)
--b1:translate(-0.5, -0.5, -0.5)
--b1:rotate('Y', 30)
--b1:translate(0.5, 0.5, 0.5)
--b1:scale(100, 100, 100)
--b1:translate(-600, -55, -200)
----------------------------------------------------------
lens = gr.nh_sphere('lens', {-600, 25, -200}, 25)
scene_root:add_child(lens)
lens:set_material(mat3)
-------------------------------------------------
cylinder = gr.cylinder('cylinder')
--scene_root:add_child(cylinder)
cylinder:set_material(mat4)
cylinder:rotate('Y', -45)
cylinder:rotate('Z', 45)
cylinder:scale(25, 25, 25)
cylinder:translate(-650, 0, -150)
----------------------------------------------------------
cone = gr.cone('cone')
--scene_root:add_child(cone)
cone:set_material(mat4)
cone:scale(25, 25, 50)
cone:rotate('Y', -90)
cone:translate(-650, 0, -150)
------------------------------------------------
cls1 = gr.nh_sphere('cls1', {-0.5, 0, 0}, 0.75)
cls1:set_material(mat4)

cls2 = gr.nh_sphere('cls2', {0.5, 0, 0}, 0.75)
cls2:set_material(mat4)

inter1 = gr.intersection('inter1', cls1, cls2)
scene_root:add_child(inter1)
inter1:scale(50, 50, 50)
--inter1:rotate('Y', 90)
inter1:translate(-650, 0, -150)

-------------------------------------------------

s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
scene_root:add_child(s4)
s4:set_material(mat3)

s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat1)

-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'meshes/smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'refraction-test-converging.png', 500, 500,
    {-800, 0, -150}, {1, 0, 0}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light}, false)
