-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {1, 1, 1}, 25, 1, 0, 0.75, 1.9)


chessboard = gr.texture('textures/chessboard.png', 50, 100)

--earth_normal = gr.normal_map('normals/cobblestone.png', 1, 5, 1.333 * 5)
earth_normal = gr.normal_map('normals/earth3.png')

scene_root = gr.node('root')

lens = gr.sphere('lens')
scene_root:add_child(lens)
lens:set_material(mat4)
--lens:set_normal(earth_normal)
lens:rotate('Y', 70)
lens:rotate('X', 45)
lens:scale(2, 2, 2)
lens:translate(0, 0, 5)

back_wall = gr.nh_box('back_wall', {-0.5, -0.5, -0.5}, 1)
scene_root:add_child(back_wall)
back_wall:set_material(mat1)
back_wall:set_texture(chessboard)
back_wall:scale(200, 200, 1)
back_wall:translate(0, 0, -5)



white_light = gr.light({4, 4, 15}, {0.9, 0.9, 0.9}, {1, 0, 0}, 0.2)

gr.render(scene_root, 'soft-shadows-test.png', 500, 500,
    {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, {white_light}, false, 50)
