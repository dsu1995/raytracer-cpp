default_mat = gr.material({1, 0.1, 0.1}, {0.8, 0.5, 0.5}, 25)

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)


wood_tex = gr.texture('textures/wood.png', 2, 2)
marble_tex = gr.texture('textures/marble2.png')


root = gr.node('root')

----------------------------------------
-- background
back = gr.nh_box('back', {-0.5, -0.5, -0.5}, 1)
root:add_child(back)
back:set_material(default_mat)
back:set_texture(wood_tex)
back:scale(40, 20, 1)
back:translate(0, 0, -5)

bottom = gr.nh_box('bottom', {-0.5, -0.5, -0.5}, 1)
root:add_child(bottom)
bottom:set_material(default_mat)
bottom:set_texture(marble_tex)
bottom:scale(14, 1, 15)
bottom:translate(0, -2, 0)
----------------------------------------
cube = gr.nh_box('cube', {-0.5, -0.5, -0.5}, 1)
cube:set_material(mat2)

cube_node = gr.node('cube_node')
cube_node:add_child(cube)
root:add_child(cube_node)
cube_node:translate(-4, -0.5, 5)

sphere = gr.sphere('sphere')
sphere:set_material(mat3)
sphere:scale(1 / 1.5, 1 / 1.5, 1 / 1.5)

sphere_node = gr.node('sphere_node')
sphere_node:add_child(sphere)
root:add_child(sphere_node)
sphere_node:translate(-2, 0, 6)

inter = gr.intersection('inter', cube, sphere)

inter_node = gr.node('inter')
inter_node:add_child(inter)
root:add_child(inter_node)
inter_node:translate(-3, 1.5, 6)

cylinder1 = gr.cylinder('cylinder1')
cylinder1:set_material(mat3)
cylinder1:scale(0.3, 0.3, 0.8)

cylinder1_node = gr.node('cylinder1_node')
cylinder1_node:add_child(cylinder1)
root:add_child(cylinder1_node)
cylinder1_node:translate(4, 0, 6)

cylinder2 = gr.cylinder('cylinder2')
cylinder2:set_material(mat3)
cylinder2:scale(0.3, 0.3, 0.8)
cylinder2:rotate('Y', 90)

cylinder2_node = gr.node('cylinder2_node')
cylinder2_node:add_child(cylinder2)
root:add_child(cylinder2_node)
cylinder2_node:translate(2.5, 0, 6)

union1 = gr.union('union1', cylinder1, cylinder2)

union1_node = gr.node('union1_node')
union1_node:add_child(union1)
root:add_child(union1_node)
union1_node:translate(3, 1, 6)

cylinder3 = gr.cylinder('cylinder3')
cylinder3:set_material(mat3)
cylinder3:scale(0.3, 0.3, 0.8)
cylinder3:rotate('X', 90)

cylinder3_node = gr.node('cylinder3_node')
cylinder3_node:add_child(cylinder3)
root:add_child(cylinder3_node)
cylinder3_node:translate(1, 1, 6)

union2 = gr.union('union2', union1, cylinder3)

union2_node = gr.node('union2_node')
union2_node:add_child(union2)
root:add_child(union2_node)
union2_node:translate(2, 2.5, 6)

diff = gr.difference('diff', inter, union2)
root:add_child(diff)
diff:scale(1.5, 1.5, 1.5)
diff:rotate('Y', 45)
diff:rotate('X', 45)
diff:translate(-0.5, 3, 6)
---------------------------------------------



white_light = gr.light({-10.0, 15.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
white_light2 = gr.light({10, 5.0, 0.0}, {0.9, 0.9, 0.9}, {5, 0, 0})

gr.render(
    root,
    'csg.png',
    1920, 1080,
    {0, 3, 12}, { 0, -0.2, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, { white_light, white_light2 },
    true, -- supersampling
    1, -- distributed sampling
    false -- grid acceleration
)

