
$$
X = \left [ \begin{matrix}
roll\\
pitch\\
yaw\\
\dot{roll}\\
\dot{pitch}\\
\dot{yaw}\\
x\\
y\\
z\\
\dot{x}\\
\dot{y}\\
\dot{z}\\
\end{matrix}
\right ] = 
\left [ \begin{matrix}
x_1\\
x_2\\
x_3\\
x_4\\
x_5\\
x_6\\
x_7\\
x_8\\
x_9\\
x_{10}\\
x_{11}\\
x_{12}\\
\end{matrix}
\right ]
\\
\newline
\\
U = \left [ \begin{matrix}
u_1\\
u_2\\
u_3\\
u_4
\end{matrix}
\right ]
$$



$$
\\
\ddot{roll} = 10*U_1 - 0.77*\dot{pitch}*\dot{yaw} \newline
\ddot{pitch} = 10*U_2 + 0.78*\dot{roll}*\dot{yaw} \\
\ddot{yaw} = 5.8*U_3 - 0.011*\dot{pitch}*\dot{roll};\\
\ddot{x} = 0.29*U_4*(sin(roll)*sin(yaw) + cos(roll)*cos(yaw)*sin(pitch));\\
\ddot{y}= -0.29*U_4*(cos(yaw)*sin(roll) - 1.0*cos(roll)*sin(pitch)*sin(yaw));\\
\ddot{z} = 0.29*U_4*cos(pitch)*cos(roll) - 9.8;\\
\\
\dot{x_1} = x_4\\
\dot{x_2} = x_5\\
\dot{x_3} = x_6\\
\dot{x_4} = 10*U_1 - 0.77*x_5*x_6\\
\dot{x_5} = 10*U_2 + 0.78*x_4*x_6;\\
\dot{x_6} = 5.8*U_3 - 0.011*x_5*x_4;\\
\dot{x_7} = x_10\\
\dot{x_8} = x_11\\
\dot{x_9} = x_12\\
\dot{x_10} = 0.29*U_4*(sin(x_1)*sin(x_3) + cos(x_1)*cos(yaw)*sin(x_2));\\
\dot{y_11}= -0.29*U_4*(cos(x_3)*sin(x_1) - 1.0*cos(x_1)*sin(x_2)*sin(yaw));\\
\dot{z_12} = 0.29*U_4*cos(x_2)*cos(x_1) - 9.8;\\
\\
\\
$$
