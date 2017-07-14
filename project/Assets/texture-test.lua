-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0, 0.75, 1.9)


earth_tex = gr.texture('textures/earth.png')
moon_tex = gr.texture('textures/moon.png')

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, 0}, 200)
scene_root:add_child(s1)
s1:set_material(mat1)
s1:set_texture(earth_tex)
--s1:rotate('Y', 45)
--s1:rotate('X', 45)
s1:translate(-125, 0, 0)

moon = gr.sphere('moon')
scene_root:add_child(moon)
moon:set_material(mat1)
moon:set_texture(moon_tex)
moon:scale(100, 100, 100)
moon:translate(200, 0, 0)



white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'texture-test.png', 500, 500,
    {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light, orange_light}, false)
