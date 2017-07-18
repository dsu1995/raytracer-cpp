mat1 = gr.material({1, 0.1, 0.1}, {0.8, 0.5, 0.5}, 25)
mat2 = gr.material({0.2, 0.2, 0.9}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({0.7, 0.7, 0.7}, {0.9, 0.9, 0.9}, 25)
mat4 = gr.material({0.9, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)


wood_tex = gr.texture('../../textures/wood.png', 2, 2)
marble_tex = gr.texture('../../textures/marble2.png')
party_tex = gr.texture('../../textures/party-hat.png')
mtn_dew_tex = gr.texture('../../textures/mtn-dew.png')
can_top_tex = gr.texture('../../textures/can-top.png')
tennis_tex = gr.texture('../../textures/tennis.png')
pingpong_tex = gr.texture('../../textures/pingpong.png')
earth_tex = gr.texture('../../textures/earth.png')

earth_norm = gr.normal_map('../../normals/earth2.png')

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
-- sphere
sphere = gr.sphere('sphere')
root:add_child(sphere)
sphere:set_material(mat3)
sphere:set_normal(earth_norm)
sphere:scale(2, 2, 2)
sphere:rotate('Y', 180)
sphere:translate(-2.5, 1, 6)
----------------------------------------
sphere2 = gr.sphere('sphere2')
root:add_child(sphere2)
sphere2:set_material(mat3)
sphere2:set_texture(earth_tex)
sphere2:set_normal(earth_norm)
sphere2:scale(2, 2, 2)
sphere2:rotate('Y', 160)
sphere2:translate(2.5, 1, 6)
-----------------------------------------


white_light = gr.light({10.0, 15.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0})


gr.render(
    root,
    'bump2.png',
    1920, 1080,
    {0, 4, 12}, {0, -0.5, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, { white_light },
    true, -- supersampling
    1, -- distributed sampling
    true -- grid acceleration
)
