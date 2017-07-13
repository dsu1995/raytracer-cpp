-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0.9)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 25)


scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, -4}, 1)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.nh_sphere('s2', {2, 0.5, -1}, 1.5)
scene_root:add_child(s2)
s2:set_material(mat1)

s3 = gr.nh_sphere('s3', {0, -12, -5}, 10)
scene_root:add_child(s3)
s3:set_material(mat2)

b1 = gr.nh_box('b1', {-2, -1.25, 0}, 1)
scene_root:add_child(b1)
b1:set_material(mat4)

--b1:translate(200, 125, 0)
--b1:scale(1/100, 1/100, 1/100)
--b1:translate(-0.5, -0.5, -0.5)
--b1:rotate('Y', 30)
--b1:translate(0.5, 0.5, 0.5)
--b1:scale(100, 100, 100)
--b1:translate(-600, -55, -200)
------------------------------------------------
mirror_sphere = gr.sphere('mirror_sphere')
mirror_sphere:set_material(mat4)

scene_root:add_child(mirror_sphere)
mirror_sphere:scale(0.15, 0.15, 0.15)
--mirror_sphere:rotate('Y', 90)
mirror_sphere:translate(-8.65, 0.2, -1.67)

mirror_cube = gr.nh_box('mirror_cube', {-9.5, 0.10, -1.50}, 0.40)
mirror_cube:set_material(mat4)
scene_root:add_child(mirror_cube)

mirror_cone = gr.cone('mirror_cone')
mirror_cone:set_material(mat4)
scene_root:add_child(mirror_cone)
mirror_cone:rotate('X', 90)
mirror_cone:scale(0.15, 0.30, 0.15)
mirror_cone:translate(-9, 0, -1.75)

mirror_cylinder = gr.cylinder('mirror_cylinder')
mirror_cylinder:set_material(mat4)
scene_root:add_child(mirror_cylinder)
mirror_cylinder:rotate('Z', 90)
mirror_cylinder:scale(0.15, 0.15, 0.15)
mirror_cylinder:translate(-9, -0.2, -1.3)
----------------------------------------------
-- backdrop
backdrop = gr.nh_box('backdrop', {-0.5, -0.5, -0.5}, 1)
scene_root:add_child(backdrop)
backdrop:set_material(grass)
backdrop:scale(4, 20, 20)
backdrop:translate(-18, 0, 0)

-------------------------------------------------

s4 = gr.nh_sphere('s4', {-1, 0.25, -3}, 0.50)
scene_root:add_child(s4)
s4:set_material(mat3)

s5 = gr.nh_sphere('s5', {0, 1, -2.5}, 0.25)
scene_root:add_child(s5)
s5:set_material(mat1)

-- A small stellated dodecahedron.

steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)

white_light = gr.light({-1.0, 1.5, 4.0}, {0.9, 0.9, 0.9}, {1.5, 0, 0})
orange_light = gr.light({4.0, 1.0, 1.5}, {0.7, 0.0, 0.7}, {1.5, 0, 0})

gr.render(scene_root, 'reflection-test-simple.png', 500, 500,
    {-8, 0, -1.5}, {-1, 0, 0}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light}, false)
