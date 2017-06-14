-- puppet.lua
-- A simplified puppet without posable joints, but that
-- looks roughly humanoid.

rootnode = gr.node('root')


red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- whole model translation helper
root_translation_helper = gr.node('root_translation_helper')
rootnode:add_child(root_translation_helper)
root_translation_helper:translate(0.0, 0.0, -1.0)


-- whole model rotation helper
root_rotation_helper = gr.node('root_rotation_helper')
root_translation_helper:add_child(root_rotation_helper)
root_rotation_helper:rotate('y', -20.0)


body_blue = gr.material({36 / 255.0, 80 / 255.0, 128 / 255.0}, {0.1, 0.1, 0.1}, 10)

-- torso
torso = gr.mesh('sphere', 'torso')
root_rotation_helper:add_child(torso)
torso:set_material(body_blue)
torso:scale(0.15, 0.15, 0.15)
torso:scale(0.75, 1.0, 0.6)
torso:rotate('x', 40)

-- tail
tail = gr.mesh('tetrahedron', 'tail')
torso:add_child(tail)
tail:set_material(body_blue)
tail:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
tail:scale(0.1, 0.8, 0.8)
tail:rotate('x', 180)
tail:translate(0, 0.7, 0.9)


belly_white = gr.material({187 / 255.0, 176 / 255.0, 155 / 255.0}, {0.1, 0.1, 0.1}, 10)

-- belly
belly = gr.mesh('sphere', 'belly')
torso:add_child(belly)
belly:set_material(belly_white)
belly:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
belly:scale(0.5, 0.7, 0.3)
belly:translate(0, 0, 0.6)


-- neck pivot
neck_pivot = gr.node('neck_pivot')
torso:add_child(neck_pivot)
neck_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
neck_pivot:translate(0, 1, 0)

-- neck joint
neck_joint = gr.joint('neck_joint', {-30, 0, 30}, {0, 0, 0})
neck_pivot:add_child(neck_joint)

-- neck
neck = gr.mesh('sphere', 'neck')
neck_joint:add_child(neck)
neck:set_material(body_blue)
neck:scale(0.3, 0.3, 0.3)
neck:translate(0, 0.15, 0)


-- head pivot
head_pivot = gr.node('head_pivot')
neck:add_child(head_pivot)
head_pivot:scale(1 / 0.3, 1 / 0.3, 1 / 0.3)
head_pivot:translate(0, 0.3, 0)

-- head left/right joint
head_lr_rotate_joint = gr.joint('head_lr_rotate_joint', {0, 0, 0}, {-45, 0, 45})
head_pivot:add_child(head_lr_rotate_joint)

-- head up/down rotation joint
head_ud_rotate_joint = gr.joint('head_ud_rotate_joint', {-30, 10, 30}, {0, 0, 0})
head_lr_rotate_joint:add_child(head_ud_rotate_joint)

-- head
head = gr.mesh('sphere', 'head')
head_ud_rotate_joint:add_child(head)
head:set_material(belly_white)
head:scale(0.65, 0.45, 0.8)
head:translate(0, 0.15, 0.5)

-- head top
head_top = gr.mesh('sphere', 'head_top')
head:add_child(head_top)
head_top:set_material(body_blue)
head_top:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
head_top:scale(0.6, 0.4, 0.75)
head_top:translate(0, 0.2, 0)


spike1 = gr.mesh('tetrahedron', 'spike1')
head_top:add_child(spike1)
spike1:set_material(belly_white)
spike1:scale(1 / 0.6, 1 / 0.4, 1 / 0.75)
spike1:scale(0.1, 0.8, 1.3)
spike1:rotate('x', 135)
spike1:translate(0.75, 4.2, -1.2)

spike1b = gr.mesh('tetrahedron', 'spike1b')
head_top:add_child(spike1b)
spike1b:set_material(body_blue)
spike1b:scale(1 / 0.6, 1 / 0.4, 1 / 0.75)
spike1b:scale(0.1, 0.8, 1.3)
spike1b:rotate('x', 135)
spike1b:translate(0.7, 4.19, -1.2)

spike2 = gr.mesh('tetrahedron', 'spike2')
head_top:add_child(spike2)
spike2:set_material(belly_white)
spike2:scale(1 / 0.6, 1 / 0.4, 1 / 0.75)
spike2:scale(0.1, 0.8, 1.3)
spike2:rotate('x', 135)
spike2:translate(-0.75, 4.2, -1.2)

spike2b = gr.mesh('tetrahedron', 'spike2b')
head_top:add_child(spike2b)
spike2b:set_material(body_blue)
spike2b:scale(1 / 0.6, 1 / 0.4, 1 / 0.75)
spike2b:scale(0.1, 0.8, 1.3)
spike2b:rotate('x', 135)
spike2b:translate(-0.7, 4.19, -1.2)

spike3 = gr.mesh('tetrahedron', 'spike3')
head_top:add_child(spike3)
spike3:set_material(body_blue)
spike3:scale(1 / 0.6, 1 / 0.4, 1 / 0.75)
spike3:scale(0.1, 0.6, 1.5)
spike3:rotate('x', 160)
spike3:translate(0, 3.7, 1.825)


iris_pink = gr.material({252 / 255.0, 186 / 255.0, 198 / 255.0}, {0.1, 0.1, 0.1}, 10)


-- left eye ball
left_eye_ball = gr.mesh('sphere', 'left_eye_ball')
head:add_child(left_eye_ball)
left_eye_ball:set_material(white)
left_eye_ball:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
left_eye_ball:scale(0.2, 0.2, 0.2)
left_eye_ball:translate(0.55, 0.5, 0.5)

-- left eye lid
left_eye_lid = gr.mesh('sphere', 'left_eye_lid')
left_eye_ball:add_child(left_eye_lid)
left_eye_lid:set_material(body_blue)
left_eye_lid:scale(0.9, 0.9, 0.9)
left_eye_lid:translate(-0.05, 0.3, 0)


-- left iris
left_iris = gr.mesh('sphere', 'left_iris')
left_eye_ball:add_child(left_iris)
left_iris:set_material(iris_pink)
left_iris:scale(0.3, 0.45, 0.3)
left_iris:translate(0.2, 0.1, 0.75)


-- right eye ball
right_eye_ball = gr.mesh('sphere', 'right_eye_ball')
head:add_child(right_eye_ball)
right_eye_ball:set_material(white)
right_eye_ball:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
right_eye_ball:scale(0.2, 0.2, 0.2)
right_eye_ball:translate(-0.55, 0.5, 0.5)

-- right eye lid
right_eye_lid = gr.mesh('sphere', 'right_eye_lid')
right_eye_ball:add_child(right_eye_lid)
right_eye_lid:set_material(body_blue)
right_eye_lid:scale(0.9, 0.9, 0.9)
right_eye_lid:translate(0.05, 0.3, 0)


-- right iris
right_iris = gr.mesh('sphere', 'right_iris')
right_eye_ball:add_child(right_iris)
right_iris:set_material(iris_pink)
right_iris:scale(0.3, 0.45, 0.3)
right_iris:translate(-0.2, 0.1, 0.75)


tongue_pink = gr.material({241 / 255.0, 161 / 255.0, 176 / 255.0}, {0.1, 0.1, 0.1}, 10)

-- tongue
tongue = gr.mesh('sphere', 'tongue')
head:add_child(tongue)
tongue:set_material(tongue_pink)
tongue:scale(1 / 0.65, 1 / 0.45, 1 / 0.8)
tongue:scale(0.8, 0.45, 0.9)
tongue:translate(0, -0.4, 0)

-- tongue2
tongue2 = gr.mesh('sphere', 'tongue2')
tongue:add_child(tongue2)
tongue2:set_material(tongue_pink)
tongue2:scale(1 / 0.8, 1 / 0.45, 1 / 0.9)
tongue2:scale(0.1, 0.1, 0.60)
tongue2:rotate('x', -60)
tongue2:rotate('y', 40)
tongue2:translate(-0.5, 0.3, -1)

-- tongue3
tongue3 = gr.mesh('sphere', 'tongue3')
tongue:add_child(tongue3)
tongue3:set_material(tongue_pink)
tongue3:scale(1 / 0.8, 1 / 0.45, 1 / 0.9)
tongue3:scale(0.25, 0.25, 0.8)
tongue3:rotate('x', 40)
tongue3:rotate('y', 100)
tongue3:translate(-1.3, 0.2, -1.05)


-- left arm pivot
left_shoulder_pivot = gr.node('left_shoulder_pivot')
torso:add_child(left_shoulder_pivot)
left_shoulder_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
left_shoulder_pivot:rotate('y', 100)
left_shoulder_pivot:translate(0.5, 0.6, 0.3)



-- left arm joint
left_shoulder_joint = gr.joint('left_shoulder_joint', {-45, 0, 45}, {0, 0, 0})
left_shoulder_pivot:add_child(left_shoulder_joint)


-- left arm
left_upper_arm = gr.mesh('sphere', 'left_upper_arm')
left_shoulder_joint:add_child(left_upper_arm)
left_upper_arm:set_material(body_blue)
left_upper_arm:scale(0.15, 0.15, 0.5)
left_upper_arm:translate(0, 0, 0.5)

-- left elbow
left_elbow = gr.mesh('sphere', 'left_elbow')
left_upper_arm:add_child(left_elbow)
left_elbow:set_material(body_blue)
left_elbow:scale(1.0 / 0.15, 1.0 / 0.15, 1.0 / 0.5)
left_elbow:scale(0.15, 0.15, 0.15)
left_elbow:translate(0, 0, 1)

-- left elbow joint
left_elbow_joint = gr.joint('left_elbow_joint', {0, 0, 45}, {0, 0, 0})
left_elbow:add_child(left_elbow_joint)

-- left elbow white
left_elbow_white = gr.mesh('sphere', 'left_elbow_white')
left_elbow:add_child(left_elbow_white)
left_elbow_white:set_material(white)
left_elbow_white:scale(0.8, 0.8, 0.8)
left_elbow_white:translate(0, 0.25, 0)

-- left lower arm
left_lower_arm = gr.mesh('sphere', 'left_lower_arm')
left_elbow_joint:add_child(left_lower_arm)
left_lower_arm:set_material(body_blue)
left_lower_arm:scale(1 / 0.15, 1 / 0.15, 1 / 0.15)
left_lower_arm:scale(0.15, 0.15, 0.5)
left_lower_arm:translate(0, 0, 3.3)


-- left hand pivot
left_hand_pivot = gr.node('left_hand_pivot')
left_lower_arm:add_child(left_hand_pivot)
left_hand_pivot:translate(0, 0, 0.4)
left_hand_pivot:scale(1 / 0.15, 1 / 0.15, 1 / 0.5)

-- left hand joint
left_hand_joint = gr.joint('left_hand_joint', {-45, 0, 45}, {0, 0, 0})
left_hand_pivot:add_child(left_hand_joint)

-- left hand
left_hand = gr.mesh('sphere', 'left_hand')
left_hand_joint:add_child(left_hand)
left_hand:set_material(body_blue)
left_hand:scale(0.15, 0.1, 0.15)
left_hand:translate(0, 0, 0.15)

-- left hand white
left_hand_white = gr.mesh('sphere', 'left_elbow_white')
left_hand:add_child(left_hand_white)
left_hand_white:set_material(white)
left_hand_white:scale(0.8, 0.8, 0.8)
left_hand_white:translate(0, 0.25, 0)

-- left finger 1
left_finger1 = gr.mesh('sphere', 'left_finger1')
left_hand:add_child(left_finger1)
left_finger1:set_material(body_blue)
left_finger1:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
left_finger1:scale(0.1, 0.1, 0.2)
left_finger1:rotate('y', -30)
left_finger1:translate(-1, 0, 1)

-- left finger 2
left_finger2 = gr.mesh('sphere', 'left_finger2')
left_hand:add_child(left_finger2)
left_finger2:set_material(body_blue)
left_finger2:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
left_finger2:scale(0.1, 0.1, 0.2)
left_finger2:translate(0, 0, 1)

-- left finger 3
left_finger3 = gr.mesh('sphere', 'left_finger3')
left_hand:add_child(left_finger3)
left_finger3:set_material(body_blue)
left_finger3:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
left_finger3:scale(0.1, 0.1, 0.2)
left_finger3:rotate('y', 30)
left_finger3:translate(1, 0, 1)

--------------------------- end left arm

-- right arm pivot
right_shoulder_pivot = gr.node('right_shoulder_pivot')
torso:add_child(right_shoulder_pivot)
right_shoulder_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
right_shoulder_pivot:rotate('y', -10)
right_shoulder_pivot:translate(-0.7, 0.6, 0.3)



-- right arm joint
right_shoulder_joint = gr.joint('right_shoulder_joint', {0, 45, 45}, {0, 0, 0})
right_shoulder_pivot:add_child(right_shoulder_joint)


-- right arm
right_upper_arm = gr.mesh('sphere', 'right_upper_arm')
right_shoulder_joint:add_child(right_upper_arm)
right_upper_arm:set_material(body_blue)
right_upper_arm:scale(0.15, 0.15, 0.5)
right_upper_arm:translate(0, 0, 0.5)

-- right elbow
right_elbow = gr.mesh('sphere', 'right_elbow')
right_upper_arm:add_child(right_elbow)
right_elbow:set_material(body_blue)
right_elbow:scale(1.0 / 0.15, 1.0 / 0.15, 1.0 / 0.5)
right_elbow:scale(0.15, 0.15, 0.15)
right_elbow:translate(0, 0, 1)

-- right elbow joint
right_elbow_joint = gr.joint('right_elbow_joint', {-45, -10, 45}, {0, 0, 0})
right_elbow:add_child(right_elbow_joint)

-- right elbow white
right_elbow_white = gr.mesh('sphere', 'right_elbow_white')
right_elbow:add_child(right_elbow_white)
right_elbow_white:set_material(white)
right_elbow_white:scale(0.8, 0.8, 0.8)
right_elbow_white:translate(0, 0.25, 0)

-- right lower arm
right_lower_arm = gr.mesh('sphere', 'right_lower_arm')
right_elbow_joint:add_child(right_lower_arm)
right_lower_arm:set_material(body_blue)
right_lower_arm:scale(1 / 0.15, 1 / 0.15, 1 / 0.15)
right_lower_arm:scale(0.15, 0.15, 0.5)
right_lower_arm:translate(0, 0, 3.3)


-- right hand pivot
right_hand_pivot = gr.node('right_hand_pivot')
right_lower_arm:add_child(right_hand_pivot)
right_hand_pivot:translate(0, 0, 0.4)
right_hand_pivot:scale(1 / 0.15, 1 / 0.15, 1 / 0.5)

-- right hand joint
right_hand_joint = gr.joint('right_hand_joint', {-45, -40, 45}, {0, 0, 0})
right_hand_pivot:add_child(right_hand_joint)

-- right hand
right_hand = gr.mesh('sphere', 'right_hand')
right_hand_joint:add_child(right_hand)
right_hand:set_material(body_blue)
right_hand:scale(0.15, 0.1, 0.15)
right_hand:translate(0, 0, 0.15)

-- right hand white
right_hand_white = gr.mesh('sphere', 'right_elbow_white')
right_hand:add_child(right_hand_white)
right_hand_white:set_material(white)
right_hand_white:scale(0.8, 0.8, 0.8)
right_hand_white:translate(0, 0.25, 0)

-- right finger 1
right_finger1 = gr.mesh('sphere', 'right_finger1')
right_hand:add_child(right_finger1)
right_finger1:set_material(body_blue)
right_finger1:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
right_finger1:scale(0.1, 0.1, 0.2)
right_finger1:rotate('y', -30)
right_finger1:translate(-1, 0, 1)

-- right finger 2
right_finger2 = gr.mesh('sphere', 'right_finger2')
right_hand:add_child(right_finger2)
right_finger2:set_material(body_blue)
right_finger2:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
right_finger2:scale(0.1, 0.1, 0.2)
right_finger2:translate(0, 0, 1)

-- right finger 3
right_finger3 = gr.mesh('sphere', 'right_finger3')
right_hand:add_child(right_finger3)
right_finger3:set_material(body_blue)
right_finger3:scale(1 / 0.15, 1 / 0.1, 1 / 0.1)
right_finger3:scale(0.1, 0.1, 0.2)
right_finger3:rotate('y', 30)
right_finger3:translate(1, 0, 1)

------------------------------------------- end right arm


-- left leg
left_leg_pivot = gr.node('left_leg_pivot')
torso:add_child(left_leg_pivot)
left_leg_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
left_leg_pivot:rotate('y', 20)
left_leg_pivot:translate(0.7, -0.8, 0.3)


-- left hip joint
left_hip_joint = gr.joint('left_hip_joint', {-40, -40, 90}, {0, 0, 0})
left_leg_pivot:add_child(left_hip_joint)


-- left thigh
left_thigh = gr.mesh('sphere', 'left_thigh')
left_hip_joint:add_child(left_thigh)
left_thigh:set_material(body_blue)
left_thigh:scale(0.4, 0.4, 0.8)
left_thigh:translate(0, 0, 0.4)


-- left knee
left_knee = gr.mesh('sphere', 'left_knee')
left_thigh:add_child(left_knee)
left_knee:set_material(body_blue)
left_knee:scale(1.0 / 0.4, 1.0 / 0.4, 1.0 / 0.8)
left_knee:scale(0.3, 0.3, 0.3)
left_knee:translate(0, 0, 0.65)

-- left knee joint
left_knee_joint = gr.joint('left_knee_joint', {0, 120, 120}, {0, 0, 0})
left_knee:add_child(left_knee_joint)

-- left knee cap
left_knee_cap = gr.mesh('sphere', 'left_knee_cap')
left_knee:add_child(left_knee_cap)
left_knee_cap:set_material(white)
left_knee_cap:scale(0.8, 0.8, 0.8)
left_knee_cap:translate(0, 0, 0.25)

-- left calf
left_calf = gr.mesh('sphere', 'left_calf')
left_knee_joint:add_child(left_calf)
left_calf:set_material(body_blue)
left_calf:scale(1 / 0.3, 1 / 0.3, 1 / 0.3)
left_calf:scale(0.2, 0.2, 0.5)
left_calf:translate(0, 0, 2.2)


-- left foot pivot
left_foot_pivot = gr.node('left_foot_pivot')
left_calf:add_child(left_foot_pivot)
left_foot_pivot:translate(0, 0, 0.4)
left_foot_pivot:scale(1 / 0.2, 1 / 0.2, 1 / 0.5)

-- left ankle joint
left_ankle_joint = gr.joint('left_ankle_joint', {-100, -100, -45}, {0, 0, 0})
left_foot_pivot:add_child(left_ankle_joint)

-- left foot
left_foot = gr.mesh('sphere', 'left_foot')
left_ankle_joint:add_child(left_foot)
left_foot:set_material(body_blue)
left_foot:scale(0.25, 0.125, 0.25)
left_foot:translate(0, 0, 0.25)

-- left foot white
left_foot_white = gr.mesh('sphere', 'left_foot_white')
left_foot:add_child(left_foot_white)
left_foot_white:set_material(white)
left_foot_white:scale(0.8, 0.8, 0.8)
left_foot_white:translate(0, 0.25, 0)

-- left toe 1
left_toe1 = gr.mesh('sphere', 'left_toe1')
left_foot:add_child(left_toe1)
left_toe1:set_material(body_blue)
left_toe1:scale(1 / 0.25, 1 / 0.125, 1 / 0.25)
left_toe1:scale(0.125, 0.125, 0.25)
left_toe1:rotate('y', -30)
left_toe1:translate(-1, 0, 1)

-- left toe 2
left_toe2 = gr.mesh('sphere', 'left_toe2')
left_foot:add_child(left_toe2)
left_toe2:set_material(body_blue)
left_toe2:scale(1 / 0.25, 1 / 0.125, 1 / 0.25)
left_toe2:scale(0.125, 0.125, 0.25)
left_toe2:translate(0, 0, 1)

-- left toe 3
left_toe3 = gr.mesh('sphere', 'left_toe3')
left_foot:add_child(left_toe3)
left_toe3:set_material(body_blue)
left_toe3:scale(1 / 0.25, 1 / 0.125, 1 / 0.25)
left_toe3:scale(0.125, 0.125, 0.25)
left_toe3:rotate('y', 30)
left_toe3:translate(1, 0, 1)


--------------------------------- end left leg


-- right leg
right_leg_pivot = gr.node('right_leg_pivot')
torso:add_child(right_leg_pivot)
right_leg_pivot:scale(1 / 0.75, 1 / 1.0, 1 / 0.6)
right_leg_pivot:rotate('y', -40)
right_leg_pivot:translate(-0.7, -0.8, 0.3)


-- right hip joint
right_hip_joint = gr.joint('right_hip_joint', {-40, -40, 90}, {0, 0, 0})
right_leg_pivot:add_child(right_hip_joint)


-- right thigh
right_thigh = gr.mesh('sphere', 'right_thigh')
right_hip_joint:add_child(right_thigh)
right_thigh:set_material(body_blue)
right_thigh:scale(0.4, 0.4, 0.8)
right_thigh:translate(0, 0, 0.4)


-- right knee
right_knee = gr.mesh('sphere', 'right_knee')
right_thigh:add_child(right_knee)
right_knee:set_material(body_blue)
right_knee:scale(1.0 / 0.4, 1.0 / 0.4, 1.0 / 0.8)
right_knee:scale(0.3, 0.3, 0.3)
right_knee:translate(0, 0, 0.65)

-- right knee joint
right_knee_joint = gr.joint('right_knee_joint', {0, 140, 140}, {0, 0, 0})
right_knee:add_child(right_knee_joint)

-- right knee cap
right_knee_cap = gr.mesh('sphere', 'right_knee_cap')
right_knee:add_child(right_knee_cap)
right_knee_cap:set_material(white)
right_knee_cap:scale(0.8, 0.8, 0.8)
right_knee_cap:translate(0, 0, 0.25)

-- right calf
right_calf = gr.mesh('sphere', 'right_calf')
right_knee_joint:add_child(right_calf)
right_calf:set_material(body_blue)
right_calf:scale(1 / 0.3, 1 / 0.3, 1 / 0.3)
right_calf:scale(0.2, 0.2, 0.5)
right_calf:translate(0, 0, 2.2)


-- right foot pivot
right_foot_pivot = gr.node('right_foot_pivot')
right_calf:add_child(right_foot_pivot)
right_foot_pivot:translate(0, 0, 0.4)
right_foot_pivot:scale(1 / 0.2, 1 / 0.2, 1 / 0.5)

-- right ankle joint
right_ankle_joint = gr.joint('right_ankle_joint', {-100, -100, -45}, {0, 0, 0})
right_foot_pivot:add_child(right_ankle_joint)

-- right foot
right_foot = gr.mesh('sphere', 'right_foot')
right_ankle_joint:add_child(right_foot)
right_foot:set_material(body_blue)
right_foot:scale(0.25, 0.125, 0.25)
right_foot:translate(0, 0, 0.25)

-- right foot white
right_foot_white = gr.mesh('sphere', 'right_foot_white')
right_foot:add_child(right_foot_white)
right_foot_white:set_material(white)
right_foot_white:scale(0.8, 0.8, 0.8)
right_foot_white:translate(0, 0.25, 0)

-- right toe 1
right_toe1 = gr.mesh('sphere', 'right_toe1')
right_foot:add_child(right_toe1)
right_toe1:set_material(body_blue)
right_toe1:scale(1 / 0.25, 1 / 0.125, 1 / 0.25)
right_toe1:scale(0.125, 0.125, 0.25)
right_toe1:rotate('y', -30)
right_toe1:translate(-1, 0, 1)

-- right toe 2
right_toe2 = gr.mesh('sphere', 'right_toe2')
right_foot:add_child(right_toe2)
right_toe2:set_material(body_blue)
right_toe2:scale(1 / 0.25, 1 / 0.125, 1 / 0.25)
right_toe2:scale(0.125, 0.125, 0.25)
right_toe2:translate(0, 0, 1)

-- right toe 3
right_toe3 = gr.mesh('sphere', 'right_toe3')
right_foot:add_child(right_toe3)
right_toe3:set_material(body_blue)
right_toe3:scale(1 / 0.25, 1 / 0.125, 1 / 0.25)
right_toe3:scale(0.125, 0.125, 0.25)
right_toe3:rotate('y', 30)
right_toe3:translate(1, 0, 1)





return rootnode
