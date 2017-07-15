-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0, 0, 0.75, 1.9)


earth_tex = gr.texture('textures/earth.png')
moon_tex = gr.texture('textures/moon.png')
marble_tex = gr.texture('textures/marble.png', 2, 2)

earth_normal = gr.normal_map('normals/earth2.png', 1)


scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, 0}, 300)
--scene_root:add_child(s1)
s1:set_material(mat1)
s1:set_texture(earth_tex)
s1:set_normal(earth_normal)
s1:rotate('Y', 90)
s1:rotate('X', 30)
--s1:translate(-125, 0, 0)

moon = gr.sphere('moon')
--scene_root:add_child(moon)
moon:set_material(mat1)
moon:set_texture(moon_tex)
moon:scale(100, 100, 100)
moon:translate(200, 0, 0)


wall = gr.nh_box('wall', {-0.5, -0.5, -0.5}, 1)
--scene_root:add_child(wall)
wall:set_material(mat1)
wall:set_texture(earth_tex)
wall:set_normal(earth_normal)
wall:scale(400, 400, 400)
--wall:rotate('X', -90)
wall:rotate('Y', -45)
--earth:translate('')

pillar = gr.cylinder('pillar')
scene_root:add_child(pillar)
pillar:set_material(mat1)
pillar:set_texture(earth_tex)
pillar:set_normal(earth_normal)
pillar:scale(200, 200, 200)
pillar:rotate('X', 90)
--pillar:rotate('Y', 270)
pillar:rotate('X', -90)

cone = gr.cone('cone')
--scene_root:add_child(cone)
cone:set_material(mat1)
cone:set_texture(earth_tex)
cone:set_normal(earth_normal)
cone:scale(200, 200, 400)
cone:rotate('X', 90)
cone:translate(0, 200, 0)
cone:rotate('Y', 90)
--cone:rotate('X', -90)


--white_light = gr.light({-400.0, 0.0, 400.0}, {0.9, 0.9, 0.9}, {2, 0, 0})
white_light = gr.light({400.0, 0.0, 800.0}, {0.9, 0.9, 0.9}, {2, 0, 0})


--white_light = gr.light({-200.0, 150.0, 300.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
--white_light2 = gr.light({200.0, 150.0, 300.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
--white_light3 = gr.light({0.0, -150.0, 300.0}, {0.9, 0.9, 0.9}, {1, 0, 0})

--orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'texture-test.png', 1000, 1000,
    {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {
        white_light,
--        white_light2,
--        white_light3
    }, false)
