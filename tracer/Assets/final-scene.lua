mat1 = gr.material({1, 0.1, 0.1}, {0.8, 0.5, 0.5}, 25)
mat2 = gr.material({0.2, 0.2, 0.9}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({0.7, 0.7, 0.7}, {0.9, 0.9, 0.9}, 25)
mat4 = gr.material({0.9, 0.9, 0.2}, {0.9, 0.2, 0.9}, 25)


wood_tex = gr.texture('textures/wood.png')
marble_tex = gr.texture('textures/marble2.png')
party_tex = gr.texture('textures/party-hat.png')
mtn_dew_tex = gr.texture('textures/mtn-dew.png')
can_top_tex = gr.texture('textures/can-top.png')
tennis_tex = gr.texture('textures/tennis.png')
pingpong_tex = gr.texture('textures/pingpong.png')
earth_tex = gr.texture('textures/earth.png')

earth_norm = gr.normal_map('normals/earth2.png')

root = gr.node('root')

----------------------------------------
-- background
--back = gr.nh_box('back', {-0.5, -0.5, -0.5}, 1)
--root:add_child(back)
--back:set_material(mat1)
--back:set_texture(wood_tex)
--back:scale(40, 20, 1)
--back:translate(0, 0, -5)

mat_reflective = gr.material({0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, 0, 0, 0.75)


mirror = gr.nh_box('mirror', {-0.5, -0.5, -0.5}, 1)
root:add_child(mirror)
mirror:set_material(mat_reflective)
mirror:scale(14, 20, 1)
mirror:rotate('Y', 45)
mirror:translate(-5, 0, -5)

mat_glossy = gr.material({0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, 0, 2, 0.75)


mirror2 = gr.nh_box('mirror2', {-0.5, -0.5, -0.5}, 1)
root:add_child(mirror2)
mirror2:set_material(mat_glossy)
mirror2:scale(20, 20, 1)
mirror2:rotate('Y', -60)
mirror2:translate(6, 0, -5)

bottom = gr.nh_box('bottom', {-0.5, -0.5, -0.5}, 1)
root:add_child(bottom)
bottom:set_material(mat1)
bottom:set_texture(wood_tex)
bottom:scale(20, 1, 20)
bottom:translate(0, -2, 0)
----------------------------------------
globe = gr.node('globe')
root:add_child(globe)
globe:rotate('Y', 20)
--globe:scale(0.8, 0.8, 0.8)
globe:translate(-3, 1, 2)

-- earth
--earth_node = gr.node('earth_node')
--globe:add_child(earth_node)
--earth_node:rotate('Z', 10)


earth = gr.sphere('earth')
--earth_node:add_child(earth)
earth:set_material(mat3)
earth:set_texture(earth_tex)
earth:set_normal(earth_norm)
earth:scale(1.5, 1.5, 1.5)
earth:rotate('Y', 140)
--earth:translate(0, 0, 0.5)
--earth:scale(1.19, 1.19, 1.19)

black_metal = gr.material({0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, 25, 1, 0.1)
earth_axis = gr.cylinder('earth_axis')
--earth_node:add_child(earth_axis)
earth_axis:set_material(black_metal)
earth_axis:rotate('X', 90)
earth_axis:scale(0.1, 1.8, 0.1)

earth_node = gr.union('earth_node', earth, earth_axis)


base_axis = gr.cylinder('base_axis')
globe:add_child(base_axis)
base_axis:set_material(black_metal)
base_axis:rotate('X', 90)
base_axis:scale(0.1, 0.3, 0.1)
base_axis:translate(0.3, -2.05, 0)

base = gr.cylinder('base')
globe:add_child(base)
base:set_material(black_metal)
base:rotate('X', 90)
base:scale(1, 0.1, 1)
base:translate(0.3, -2.3, 0)

neg_mat = gr.material({0, 0, 0}, {0, 0, 0}, 0)

arc_sphere = gr.sphere('arc_sphere')
arc_sphere:set_material(black_metal)
arc_sphere:scale(1.8, 1.8, 1.8)

slice1 = gr.nh_box('slice1', {-0.5, -0.5, -0.5}, 1)
slice1:set_material(neg_mat)
slice1:scale(10, 10, 2)
slice1:rotate('Y', 5)
slice1:translate(0, 0, 1)

slice2 = gr.nh_box('slice2', {-0.5, -0.5, -0.5}, 1)
slice2:set_material(neg_mat)
slice2:scale(10, 10, 2)
slice2:rotate('Y', -5)
slice2:translate(0, 0, -1)

slice3 = gr.sphere('slice3')
slice3:set_material(neg_mat)
slice3:scale(1.65, 1.65, 1.65)

arc1 = gr.union('arc1', slice1, slice2)
arc2 = gr.union('arc2', arc1, slice3)

arc3 = gr.difference('arc3', arc_sphere, arc2)

--earth_node:add_child(arc3)
earth_node2 = gr.union('earth_node2', earth_node, arc3)
globe:add_child(earth_node2)
earth_node2:rotate('Z', 10)
-----------------------------------------
glass = gr.material({1, 1, 1}, {1, 1, 1}, 0, 0, 0, 0.9, 1.5)

tree_top = gr.cone('tree_top')
tree_top:set_material(glass)
tree_top:rotate('X', 90)
tree_top:scale(0.8, 1.6, 0.8)
tree_top:translate(0, 2, 0)

tree_bottom = gr.cylinder('tree_bottom')
tree_bottom:set_material(glass)
tree_bottom:rotate('X', 90)
tree_bottom:scale(0.2, 0.35, 0.2)

tree = gr.node('tree')
tree:add_child(tree_bottom)
tree:add_child(tree_top)
tree:scale(1.5, 1.5, 1.5)
tree:translate(1, -0.8, 2)


root:add_child(tree)
------------------------------------------
mat3b = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0, 0.75)


trophy = gr.node('trophy')
--trophy:scale(1.5, 1.5, 1.5)
trophy:translate(4.5, 0.5, 2.5)
root:add_child(trophy)

cube2 = gr.cube('cube2')
cube2:set_material(mat2)
cube2:translate(-0.5, -0.5, -0.5)
cube2:scale(1.5, 1.5, 1.5)

sphere4 = gr.sphere('sphere4')
sphere4:set_material(mat3b)
--scene_root:add_child('sphere4')

inter3 = gr.intersection('inter3', cube2, sphere4)
--scene_root:add_child(inter3)
--inter3:rotate('Y', 45)
--inter3:rotate('X', 45)


cylinder1 = gr.cylinder('cylinder1')
cylinder1:set_material(mat3b)
cylinder1:scale(0.5, 0.5, 2)

cylinder2 = gr.cylinder('cylinder2')
cylinder2:set_material(mat3b)
cylinder2:scale(0.5, 0.5, 2)
cylinder2:rotate('Y', 90)

cylinder3 = gr.cylinder('cylinder3')
cylinder3:set_material(mat3b)
cylinder3:scale(0.5, 0.5, 2)
cylinder3:rotate('X', 90)

union4 = gr.union('union4', cylinder3, cylinder2)
union5 = gr.union('union5', cylinder1, union4)

--scene_root:add_child(union5)
--union5:rotate('Y', 45)
--union5:rotate('X', 45)


diff = gr.difference("diff", inter3, union5)
trophy:add_child(diff)
diff:rotate('Y', 45)
diff:rotate('X', 45)

c2 = gr.cylinder('cylinder2')
c2:set_material(mat3b)
c2:rotate('Y', 90)
c2:translate(-2, 0, 0)

c3 = gr.cylinder('cylinder3')
c3:set_material(mat3b)
c3:rotate('Y', 90)
c3:translate(2, 0, 0)

union2 = gr.union('union2', c2, c3)

sphere2 = gr.sphere('sphere2')
sphere2:set_material(mat3b)
sphere2:scale(2, 2, 2)

inter2 = gr.intersection('inter2', union2, sphere2)
trophy:add_child(inter2)
inter2:rotate('Z', -90)

------------------------------------------------



white_light = gr.light({-10.0, 15.0, 20.0}, {0.9, 0.9, 0.9}, {1, 0, 0}, 1)


gr.render(
    root,
    'final-scene.png',
    1920, 1080,
    {1, 4, 12}, {0, -0.25, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, { white_light },
    true, -- supersampling
    50, -- distributed sampling
    true -- grid acceleration
)
