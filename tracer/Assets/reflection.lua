mat1 = gr.material({1, 0.1, 0.1}, {0.9, 0.9, 0.9}, 25)
mat_reflective = gr.material({0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, 0, 0, 0.75)
mat2 = gr.material({0, 0, 0}, {0.7, 0.7, 0.7}, 25)


wood_tex = gr.texture('textures/wood.png')
marble_tex = gr.texture('textures/marble2.png')
party_tex = gr.texture('textures/party-hat2.png')
mtn_dew_tex = gr.texture('textures/mtn-dew.png')
can_top_tex = gr.texture('textures/can-top.png')
tennis_tex = gr.texture('textures/tennis.png')
pingpong_tex = gr.texture('textures/pingpong.png')

root = gr.node('root')

----------------------------------------
-- background
mirror = gr.nh_box('mirror', {-0.5, -0.5, -0.5}, 1)
root:add_child(mirror)
mirror:set_material(mat_reflective)
mirror:scale(14, 20, 1)
mirror:rotate('Y', 45)
mirror:translate(-5, 0, -5)

mirror2 = gr.nh_box('mirror2', {-0.5, -0.5, -0.5}, 1)
root:add_child(mirror2)
mirror2:set_material(mat_reflective)
mirror2:scale(20, 20, 1)
mirror2:rotate('Y', -60)
mirror2:translate(6, 0, -5)

bottom = gr.nh_box('bottom', {-0.5, -0.5, -0.5}, 1)
root:add_child(bottom)
bottom:set_material(mat1)
bottom:set_texture(wood_tex)
bottom:scale(30, 1, 30)
bottom:translate(0, -2, 0)
----------------------------------------
-- cone
cone = gr.cone('cone')
root:add_child(cone)
cone:set_material(mat1)
cone:set_texture(party_tex)
cone:rotate('X', 90)
cone:rotate('Y', 180)
cone:scale(1, 2, 1)
cone:scale(2, 2, 2)
cone:translate(-3, 2.5, 2)
----------------------------------------
-- cylinder
cylinder = gr.cylinder('cylinder')
root:add_child(cylinder)
cylinder:set_material(mat1)
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
sphere:set_material(mat1)
sphere:set_texture(pingpong_tex)
sphere:scale(0.5, 0.5, 0.5)
sphere:rotate('Y', 90)
sphere:rotate('X', -20)
sphere:translate(3, -1, 3)
----------------------------------------


white_light = gr.light({-10.0, 15.0, 25.0}, {0.9, 0.9, 0.9}, {1.5, 0.01, 0})
white_light2 = gr.light({10, 5.0, 15.0}, {0.9, 0.9, 0.9}, {5, 0, 0})


gr.render(
    root,
    'reflection.png',
    1920, 1080,
    {1, 6, 12}, {0, -0.5, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3},
    {
        white_light,
        white_light2
    },
    true, -- supersampling
    1, -- distributed sampling
    true -- grid acceleration
)
