-- A simple scene with five spheres

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
scene_root:add_child(s1)
s1:set_material(mat1)

s2 = gr.nh_sphere('s2', {200, 50, -100}, 150)
scene_root:add_child(s2)
s2:set_material(mat1)

s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(mat2)

s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
scene_root:add_child(s4)
s4:set_material(mat3)

s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat1)

mickey = gr.mesh('mickey', 'mickey.obj')
scene_root:add_child(mickey)
mickey:set_material(mat3)
mickey:scale(250, 250, 250)
mickey:rotate('x', -90)
mickey:rotate('y', -90)
mickey:translate(-200, -200, 0)

steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
steldodec:set_material(mat3)
scene_root:add_child(steldodec)


cow_poly = gr.mesh('cow', 'cow.obj')
factor = 20
hide = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
cow_poly:set_material(hide)
cow_poly:translate(0.0, 3.637, 0.0)
cow_poly:scale(factor, factor, factor)
cow_poly:translate(-250.0, 0.0, 0.0)
scene_root:add_child(cow_poly)

buckyball = gr.mesh( 'buckyball', 'buckyball.obj' )
scene_root:add_child(buckyball)
stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0)
buckyball:set_material(stone)
buckyball:scale(50, 50, 50)
buckyball:translate(200, -200, 0)

-- the arc
gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25)

arc = gr.node('arc')
scene_root:add_child(arc)
arc:scale(30, 30, 30)
arc:rotate('Y', 30)
arc:translate(250,-50,50)
p1 = gr.cube('p1')
arc:add_child(p1)
p1:set_material(gold)
p1:scale(0.8, 4, 0.8)
p1:translate(-2.4, 0, -0.4)

p2 = gr.cube('p2')
arc:add_child(p2)
p2:set_material(gold)
p2:scale(0.8, 4, 0.8)
p2:translate(1.6, 0, -0.4)

s = gr.sphere('s')
arc:add_child(s)
s:set_material(gold)
s:scale(4, 0.6, 0.6)
s:translate(0, 4, 0)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

gr.render(scene_root, 'simple.png', 500, 500,
	  {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
