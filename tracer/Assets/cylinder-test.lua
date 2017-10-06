-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')
scene_root:scale(100, 100, 100)


s1 = gr.cylinder('cylinder1')
scene_root:add_child(s1)
s1:set_material(mat1)
s1:rotate('X', -135)
--s1:rotate('Y', 45)
--s1:rotate('Z', 45)
--s1:translate(0, -2, 0)


--sphere = gr.sphere('sphere')
--scene_root:add_child(sphere)
--sphere:set_material(mat1)
--sphere:translate(0, 2, 0)



white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'cylinder.png', 500, 500,
    {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light})
