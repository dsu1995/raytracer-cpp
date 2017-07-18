mat1 = gr.material({1, 0.1, 0.1}, {0.8, 0.5, 0.5}, 25)
mat2 = gr.material({0.2, 0.2, 0.9}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({0.2, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)
mat4 = gr.material({0.9, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)


wood_tex = gr.texture('../../textures/wood.png', 2, 2)
marble_tex = gr.texture('../../textures/marble2.png')


root = gr.node('root')

----------------------------------------
-- background
back = gr.nh_box('back', {-0.5, -0.5, -0.5}, 1)
root:add_child(back)
back:set_material(mat1)
back:set_texture(wood_tex)
back:scale(40, 20, 1)
back:translate(0, 0, -5)

bottom = gr.nh_box('bottom', {-0.5, -0.5, -0.5}, 1)
root:add_child(bottom)
bottom:set_material(mat1)
bottom:set_texture(marble_tex)
bottom:scale(14, 1, 15)
bottom:translate(0, -2, 0)
----------------------------------------
-- cone
cone = gr.cone('cone')
root:add_child(cone)
cone:set_material(mat1)
cone:rotate('X', 90)
cone:scale(1, 2, 1)
cone:translate(-3, 0.5, 3)
----------------------------------------
-- cylinder
cylinder = gr.cylinder('cylinder')
root:add_child(cylinder)
cylinder:set_material(mat2)
cylinder:rotate('X', 90)
cylinder:translate(0.5, -0.5, 3)
----------------------------------------
-- sphere
sphere = gr.sphere('sphere')
root:add_child(sphere)
sphere:set_material(mat3)
sphere:translate(-2, -0.5, -1)
----------------------------------------
-- cube
cube = gr.cube('cube')
root:add_child(cube)
cube:set_material(mat4)
cube:scale(2, 2, 2)
cube:translate(3, -1.5, -3)


----------------------------------------


white_light = gr.light({-10.0, 15.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
white_light2 = gr.light({10, 5.0, 0.0}, {0.9, 0.9, 0.9}, {5, 0, 0})


gr.render(
    root,
    'primitives.png',
    1920, 1080,
    {0, 4, 12}, {0, -0.5, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, { white_light, white_light2 },
    true, -- supersampling
    1, -- distributed sampling
    true -- grid acceleration
)
