mat1 = gr.material({1, 0.1, 0.1}, {0.8, 0.5, 0.5}, 25)
mat2 = gr.material({0.2, 0.2, 0.9}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({0.2, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)
mat4 = gr.material({0.9, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)


wood_tex = gr.texture('../../textures/wood.png', 2, 2)
marble_tex = gr.texture('../../textures/marble2.png')
party_tex = gr.texture('../../textures/party-hat.png')
mtn_dew_tex = gr.texture('../../textures/mtn-dew.png')
can_top_tex = gr.texture('../../textures/can-top.png')
tennis_tex = gr.texture('../../textures/tennis.png')
pingpong_tex = gr.texture('../../textures/pingpong.png')

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
cone:set_texture(party_tex)
cone:rotate('X', 90)
cone:scale(1, 2, 1)
cone:scale(2, 2, 2)
cone:translate(-3, 2.5, 3)
----------------------------------------
-- cylinder
cylinder = gr.cylinder('cylinder')
root:add_child(cylinder)
cylinder:set_material(mat2)
cylinder:set_texture(mtn_dew_tex)
cylinder:rotate('X', 90)
cylinder:scale(0.7, 1, 0.7)
cylinder:translate(0.5, -0.5, 3)

cylinder_top = gr.cylinder('cylinder_top')
root:add_child(cylinder_top)
cylinder_top:set_material(mat2)
cylinder_top:set_texture(can_top_tex)
cylinder_top:rotate('X', 90)
cylinder_top:scale(0.68, 0.01, 0.68)
cylinder_top:translate(0.5, 0.55, 3)
----------------------------------------
-- sphere
sphere = gr.sphere('sphere')
root:add_child(sphere)
sphere:set_material(mat3)
sphere:set_texture(pingpong_tex)
sphere:scale(0.5, 0.5, 0.5)
sphere:rotate('Y', 90)
sphere:rotate('X', -20)
sphere:translate(3, -0.5, 3)
----------------------------------------


white_light = gr.light({-10.0, 15.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 1)


gr.render(
    root,
    'soft-shadow.png',
    1920, 1080,
    {0, 4, 12}, {0, -0.5, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, { white_light},
    true, -- supersampling
    50, -- distributed sampling
    false -- grid acceleration
)
