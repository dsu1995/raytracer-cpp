mat1 = gr.material({1, 0.1, 0.1}, {0.8, 0.5, 0.5}, 25)
mat2 = gr.material({0.2, 0.2, 0.9}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({0.2, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)
mat4 = gr.material({0.9, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)


marble_tex = gr.texture('../../textures/marble2.png', 100, 100)

root = gr.node('root')

----------------------------------------
-- sphere
size = 30
step = 1.5
for i = 0, size do
    for j = 0, size do
        for k = 0, size do
            local sphere = gr.sphere('sphere')
            root:add_child(sphere)
            sphere:set_material(mat3)
            sphere:translate(-5 + i * step, j * step, 5 - k * step)
        end
    end
end
----------------------------------------


white_light = gr.light({-10.0, 15.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
white_light2 = gr.light({10, 15.0, 0.0}, {0.9, 0.9, 0.9}, {5, 0, 0})


gr.render(
    root,
    'grid.png',
    1920 / 2, 1080 / 2,
    {0, 4, 20}, {0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, { white_light, white_light2 },
    false, -- supersampling
    1, -- distributed sampling
    true -- grid acceleration
)
