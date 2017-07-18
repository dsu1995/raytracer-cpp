-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 25)


scene_root = gr.node('root')

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

white_light = gr.light({-1.0, 1.5, 4.0}, {0.9, 0.9, 0.9}, {1.5, 0, 0})
orange_light = gr.light({4.0, 1.0, 1.5}, {0.7, 0.0, 0.7}, {1.5, 0, 0})

gr.render(scene_root, 'aabb-test.png', 100, 100,
    {-7.5, 0, -1.5}, {-1, 0, 0}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light}, true)
