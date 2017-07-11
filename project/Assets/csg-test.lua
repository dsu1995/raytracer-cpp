-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')
scene_root:scale(100, 100, 100)


c1 = gr.cylinder('cylinder1')
c1:set_material(mat1)
c1:rotate('X', 90)
--s1:rotate('Y', 45)
--s1:rotate('Z', 45)
c1:scale(0.8, 0.8, 0.8)
c1:translate(0, -0, 0)


sphere = gr.sphere('sphere')
sphere:set_material(mat2)
sphere:translate(0, 0, 0)

--union = gr.union('union', c1, sphere)
--scene_root:add_child(union)
--union:rotate('Z', 90)

inter = gr.intersection('inter', c1, sphere)
scene_root:add_child(inter)
inter:rotate('X', 250)
inter:rotate('Z', 300)
inter:translate(0, 2, 0)


c2 = gr.cylinder('cylinder2')
c2:set_material(mat3)
c2:rotate('Y', 90)
c2:translate(-2, 0, 0)

c3 = gr.cylinder('cylinder3')
c3:set_material(mat3)
c3:rotate('Y', 90)
c3:translate(2, 0, 0)

union2 = gr.union('union', c2, c3)
--scene_root:add_child(union2)

sphere2 = gr.sphere('sphere2')
sphere2:set_material(mat4)
sphere2:scale(2, 2, 2)

inter2 = gr.intersection('inter2', union2, sphere2)
scene_root:add_child(inter2)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'csg-test.png', 500, 500,
    {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light})
