# coding:utf-8
import math
import numpy as np
import scipy.linalg as la
from matplotlib import pyplot as plt
from scipy.sparse.construct import eye

Kp = 1.0  # 速度比例控制增益

Q = np.eye(3)  # Q矩阵[3x3]
R = np.eye(1)  # R矩阵[1]

dt = 0.1  # 时间步长
L = 0.5  # 轴距/m
delta = 0
max_steer = np.deg2rad(30.0)  # 最大前轮转角，单位弧度


# 车辆状态类
class State:
    def __init__(self, x=0.0, y=0.0, yaw=0.0, v=0.0):
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v


# 车辆运动学模型
def update(state, a, delta):
    if delta >= max_steer:
        delta = max_steer
    if delta <= - max_steer:
        delta = - max_steer
    state.x = state.x + state.v * math.cos(state.yaw) * dt
    state.y = state.y + state.v * math.sin(state.yaw) * dt
    state.yaw = state.yaw + state.v / L * math.tan(delta) * dt
    state.v = state.v + a * dt

    return state


def get_trajectory():
    with open("trajectory.txt", "r") as f:
        for line in f.readlines():
            l = []
            l = line.split(" ")
            if l[0] == "x":
                x = [float('{:.4f}'.format(float(i))) for i in l[1:]]
            elif l[0] == "y":
                y = [float('{:.4f}'.format(float(i))) for i in l[1:]]
            elif l[0] == "yaw":
                yaw = [float('{:.4f}'.format(float(i))) for i in l[1:]]
            else:
                k = [float('{:.4f}'.format(float(i))) for i in l[1:]]

    return x, y, yaw, k


def pi_2_pi(angle):
    if angle > math.pi:
        angle -= 2 * math.pi
    elif angle < -math.pi:
        angle += 2 * math.pi
    else:
        angle = angle

    return angle


def cal_speed_list(cyaw, target_speed):
    speed = [target_speed] * len(cyaw)
    direction = 1.0
    speed[-1] = 0.0
    return speed


def PIDControl(target, current):
    a = Kp * (target - current)

    return a


def cal_nearest_index(state, cx, cy, cyaw):
    dx = [state.x - icx for icx in cx]
    dy = [state.y - icy for icy in cy]

    d = [idx ** 2 + idy ** 2 for (idx, idy) in zip(dx, dy)]

    min_d = min(d)
    index_d = d.index(min_d)

    dis = math.sqrt(min_d)

    dxl = cx[index_d] - state.x
    dyl = cy[index_d] - state.y
    angle = pi_2_pi(cyaw[index_d] - math.atan2(dyl, dxl))

    if angle < 0:
        dis *= -1
    return index_d, dis


def solve_riccati_equation(A, B, Q, R):
    P = eye(3)  # P矩阵初始化，维度与A和Q矩阵相同
    maxiter = 150  # 最大迭代次数
    eps = 0.01  # 当P的两次迭代插值小于eps时，则认为收敛
    for i in range(maxiter):
        Pn = Q + A.T @ P @ A - A.T @ P @ B @ \
             la.inv(R + B.T @ P @ B) @ B.T @ P @ A
        # Pn = P + P @ A @ A.T + Q - P @ B @ la.inv(R) @ B.T @ P
        if (abs(Pn - P)).max() < eps:
            break
        P = Pn
    return Pn


def dlqr(A, B, Q, R):
    # 求解黎卡提方程
    P = solve_riccati_equation(A, B, Q, R)
    # 计算LQR反馈增益
    K = la.inv(B.T @ P @ B + R) @ (B.T @ P @ A)

    return K


def lqr_steering_control(state, cx, cy, cyaw, ck):
    global delta
    ind, e_d = cal_nearest_index(state, cx, cy, cyaw)
    k = ck[ind]  # 最近点曲率
    v = state.v  # 获取当前车速

    A = np.zeros((3, 3))
    A[0, 0] = 1.0
    A[0, 2] = -v * math.sin(cyaw[ind]) * dt
    A[1, 1] = 1.0
    A[1, 2] = v * math.cos(cyaw[ind]) * dt
    A[2, 2] = 1.0

    B = np.zeros((3, 1))
    B[2, 0] = v / (L * pow(math.cos(delta), 2)) * dt

    K = dlqr(A, B, Q, R)

    X = np.zeros((3, 1))
    X[0, 0] = cx[ind] - state.x
    X[1, 0] = cy[ind] - state.y
    X[2, 0] = pi_2_pi(cyaw[ind] - state.yaw)

    fb = -pi_2_pi(-K @ X)
    ff = math.atan2(L * k, 1)
    delta = fb + ff
    return delta, ind


def closed_loop_process(cx, cy, cyaw, ck, sp, stop_position):
    T = 500.0  # 最大仿真时间
    goal_dis = 0.3  # 距离终点停车距离
    time = 0.0  # 累计计时
    # 车辆状态类初始化
    state = State(x=-0.0, y=-0.0, yaw=0.0, v=0.0)
    x = [state.x]
    y = [state.y]
    yaw = [state.yaw]
    v = [state.v]
    t = [0.0]

    while T >= time:
        dl, target_ind = lqr_steering_control(state, cx, cy, cyaw, ck)
        ai = PIDControl(sp[target_ind], state.v)
        state = update(state, ai, dl)

        time = time + dt

        dx = state.x - stop_position[0]
        dy = state.y - stop_position[1]
        if (math.hypot(dx, dy) <= goal_dis) | (math.hypot(dx, dy) >= goal_dis * 100.0):
            print("Goal")
            break
        x.append(state.x)
        y.append(state.y)
        yaw.append(state.yaw)
        v.append(state.v)
        t.append(time)

        if target_ind % 1 == 0:
            plt.cla()
            plt.gcf().canvas.mpl_connect('key_release_event',
                                         lambda event: [exit(0) if event.key == 'escape' else None])
            plt.plot(cx, cy, "-r", label="course")
            plt.plot(x, y, "ob", label="trajectory")
            plt.plot(cx[target_ind], cy[target_ind], "xg", label="target")
            plt.axis("equal")
            plt.grid(True)
            plt.title("speed[km/h]:" + str(round(state.v * 3.6, 2))
                      + ",target index:" + str(target_ind))
            plt.pause(0.001)

    return t, x, y, yaw, v


def main():
    print("LQR steering control tracking start!!")
    # 获取全局轨迹点
    cx, cy, cyaw, ck = get_trajectory()
    # 设定目标速度
    target_speed = 3  # 10m/s
    # 设定结束位置
    stop_position = [cx[-1], cy[-1]]
    # 计算每个轨迹点参考速度
    sp = cal_speed_list(cyaw, target_speed)
    # 反馈+前馈
    t, x, y, yaw, v = closed_loop_process(cx, cy, cyaw, ck, sp, stop_position)

    plt.plot(cx, cy, "-r", label="spline")
    plt.plot(x, y, "-g", label="tracking")
    plt.grid(True)
    plt.axis("equal")
    plt.xlabel("x[m]")
    plt.ylabel("y[m]")
    plt.savefig('lqr_steering_control03.jpg')
    plt.legend()


if __name__ == "__main__":
    main()
