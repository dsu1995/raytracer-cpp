-- A simple scene with some miscellaneous geometry.

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')
scene_root:scale(100, 100, 100)
--scene_root:rotate('Y', 120)

---------------------------------------------------
-- the floor
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0)
plane = gr.sphere('plane')
scene_root:add_child(plane)
plane:set_material(grass)
plane:scale(100, 100, 100)
plane:translate(0, 0, -110)
--------------------------------------------------
c1 = gr.cylinder('cylinder1')
c1:set_material(mat1)
c1:rotate('X', 90)
c1:scale(0.8, 0.8, 0.8)
--s1:rotate('Y', 45)
--s1:rotate('Z', 45)
c1:translate(0, -0, 0)


sphere = gr.sphere('sphere')
sphere:set_material(mat2)
sphere:translate(0, 0, 0)

--union = gr.union('union', c1, sphere)
--scene_root:add_child(union)
--union:rotate('Z', 90)

inter = gr.intersection('inter', c1, sphere)
--scene_root:add_child(inter)
inter:rotate('X', 250)
inter:rotate('Z', 300)
--inter:translate(0, 2, 0)

------------------------------------
c2 = gr.cylinder('cylinder2')
c2:set_material(mat3)
c2:rotate('Y', 90)
c2:translate(-2, 0, 0)

c3 = gr.cylinder('cylinder3')
c3:set_material(mat3)
c3:rotate('Y', 90)
c3:translate(2, 0, 0)

union2 = gr.union('union2', c2, c3)

sphere2 = gr.sphere('sphere2')
sphere2:set_material(mat4)
sphere2:scale(2, 2, 2)

inter2 = gr.intersection('inter2', union2, sphere2)
scene_root:add_child(inter2)
inter2:rotate('Z', -90)
--inter2:rotate('X', 30)
--inter2:translate(-1, 1, -1)

--------------------------------
--cube = gr.cube('cube')
--cube:set_material(mat2)
--cube:translate(-0.5, -0.5, -0.5)
--cube:scale(1.5, 1.5, 1.5)
--
--sphere3 = gr.sphere('sphere3')
--sphere3:set_material(mat3)
--sphere3:scale(0.9, 0.9, 0.9)
--
--sphere3a = gr.sphere('sphere3a')
--sphere3a:set_material(mat1)
--
--union3 = gr.difference('union3', cube, sphere3)
--inter3a = gr.intersection('inter3a', sphere3a, union3)
--scene_root:add_child(inter3a)
--inter3a:rotate('Y', -30)
--inter3a:rotate('X', 45)


----------------------------
cube2 = gr.cube('cube2')
cube2:set_material(mat2)
cube2:translate(-0.5, -0.5, -0.5)
cube2:scale(1.5, 1.5, 1.5)

sphere4 = gr.sphere('sphere4')
sphere4:set_material(mat3)
--scene_root:add_child('sphere4')

inter3 = gr.intersection('inter3', cube2, sphere4)
--scene_root:add_child(inter3)
--inter3:rotate('Y', 45)
--inter3:rotate('X', 45)


cylinder1 = gr.cylinder('cylinder1')
cylinder1:set_material(mat3)
cylinder1:scale(0.5, 0.5, 2)

cylinder2 = gr.cylinder('cylinder2')
cylinder2:set_material(mat3)
cylinder2:scale(0.5, 0.5, 2)
cylinder2:rotate('Y', 90)

cylinder3 = gr.cylinder('cylinder3')
cylinder3:set_material(mat3)
cylinder3:scale(0.5, 0.5, 2)
cylinder3:rotate('X', 90)

union4 = gr.union('union4', cylinder3, cylinder2)
union5 = gr.union('union5', cylinder1, union4)

--scene_root:add_child(union5)
--union5:rotate('Y', 45)
--union5:rotate('X', 45)


diff = gr.difference("diff", inter3, union5)
scene_root:add_child(diff)
diff:rotate('Y', 45)
diff:rotate('X', 45)



--------------------------------




white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(
    scene_root,
    'csg-test.png',
    500, 500,
    {0, 0, 800}, { 0, 0, -1}, {0, 1, 0}, 50,
    {0.3, 0.3, 0.3}, { white_light, orange_light },
    false
)
