
$$
F_i(u_i)= (9.81/1000)*(-0.0026u_i^3 + 0.4892u_i^2 - 4.2855*u_i+0.8182)\\
\frac{\partial F_i(u_i)}{\partial u_i} = dF_i(u_i) = (9.81/1000) * (-0.0078*u_i^2 + 0.9784*u_i - 4.2855)\\
\\
\\

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

\\
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
\ddot{roll} = 4.2*F_1 - 4.2*F_2 - 4.2*F_3 + 4.2*F_4 - 0.77*\dot{pitch}*\dot{yaw}\\
\ddot{pitch} = 4.1*F_3 - 4.1*F_2 - 4.1*F_1 + 4.1*F_4 + 0.78*\dot{roll}*\dot{yaw};\\
\ddot{yaw} = 5.8*M_2 - 5.8*M_1 - 5.8*M_3 + 5.8*M_4 - 0.011*\dot{pitch}*\dot{roll};\\


\\
\dot{x_1} = x_4\\
\dot{x_2} = x_5\\
\dot{x_3} = x_6\\

\dot{x_4} = 4.2*F_1 - 4.2*F_2 - 4.2*F_3 + 4.2*F_4 - 0.77*x_5*x_6\\
\dot{x_5} = 4.1*F_3 - 4.1*F_2 - 4.1*F_1 + 4.1*F_4 + 0.78*x_4*x_6;\\
\dot{x_6} = 5.8*M_2 - 5.8*M_1 - 5.8*M_3 + 5.8*M_4 - 0.011*x_5*x_4;\\

\dot{x_7} = x_{10}\\
\dot{x_8} = x_{11}\\
\dot{x_9} = x_{12}\\


\dot{x_{10}} = 0.29*(sin(x_1)*sin(x_3) + cos(x_1)*cos(x_3)*sin(x_2))*(F_1 + F_2 + F_3 + F_4)\\
\dot{x_{11}} = -0.29*(cos(x_3)*sin(x_1) - 1.0*cos(x_1)*sin(x_2)*sin(x_3))*(F_1 + F_2 + F_3 + F_4)\\
\dot{x_{12}} = 0.29*cos(x_2)*cos(x_1)*(F_1 + F_2 + F_3 + F_4) - 9.8;\\
\\
\\
\\
$$

$$
A_L = \left [ 
\begin{matrix}

0                                                    & 0                               & 0                                                   & 1          & 0          & 0         & 0 & 0 & 0 & 0 & 0 & 0 \\
0                                                    & 0                               & 0                                                   & 0          & 1          & 0         & 0 & 0 & 0 & 0 & 0 & 0 \\
0                                                    & 0                               & 0                                                   & 0          & 0          & 1         & 0 & 0 & 0 & 0 & 0 & 0 \\
0                                                    & 0                               & 0                                                   & 0          & -0.77*x_6  & -0.77*x_5 & 0 & 0 & 0 & 0 & 0 & 0 \\
0                                                    & 0                               & 0                                                   & 0.78*x_6   & 0          & 0.78*x_4  & 0 & 0 & 0 & 0 & 0 & 0 \\
0                                                    & 0                               & 0                                                   & -0.011*x_5 & -0.011*x_4 & 0         & 0 & 0 & 0 & 0 & 0 & 0 \\
0                                                    & 0                               & 0                                                   & 0          & 0          & 0         & 0 & 0 & 0 & 1 & 0 & 0 \\
0                                                    & 0                               & 0                                                   & 0          & 0          & 0         & 0 & 0 & 0 & 0 & 1 & 0 \\
0                                                    & 0                               & 0                                                   & 0          & 0          & 0         & 0 & 0 & 0 & 0 & 0 & 1 \\
K1*(sin(x_3)*cos(x_1) - sin(x_1)*cos(x_3)*sin(x_2))  & K1*(cos(x_2)*cos(x_1)*cos(x_3)) & K1*(cos(x_3)*sin(x_1)-cos(x_1)*sin(x_3)*sin(x_2))   & 0          & 0          & 0         & 0 & 0 & 0 & 0 & 0 & 0 \\
K1*(-cos(x_1)*cos(x_3) + sin(x_1)*sin(x_2)*sin(x_3)) & K1*(cos(x_1)*cos(x_2)*sin(x_3)) & K1*(sin(x_3)*sin(x_1) + cos(x_1)*sin(x_2)*cos(x_3)) & 0          & 0          & 0         & 0 & 0 & 0 & 0 & 0 & 0 \\
K1*(-sin(x_1)*cos(x_2))                              & K1*(-sin(x_2)*cos(x_1))         & 0                                                   & 0          & 0          & 0         & 0 & 0 & 0 & 0 & 0 & 0 \\
\end{matrix}
\right ]

\\
\\
$$
$$
B_L = \left [ 
\begin{matrix}
0            & 0           & 0            & 0           & \\
0            & 0           & 0            & 0           & \\
0            & 0           & 0            & 0           & \\
4.2*dF_1     & -4.2*dF_2   & -4.2*dF_3    & 4.2*dF_4    & \\
-4.1*dF_1    & -4.1*dF_2   & 4.1*dF_3     & 4.1*dF_4    & \\
-5.8*Kv*dF_1 & 5.8*Kv*dF_2 & -5.8*Kv*dF_3 & 5.8*Kv*dF_4 & \\
0            & 0           & 0            & 0           & \\
0            & 0           & 0            & 0           & \\
0            & 0           & 0            & 0           & \\
K2*dF_1      & K2*dF_2     & K2*dF_3      & K2*dF_4     & \\
K3*dF_1      & K3*dF_2     & K3*dF_3      & K3*dF_4     & \\
K4*dF_1      & K4*dF_2     & K4*dF_3      & K4*dF_4     & \\
\end{matrix}

\right ]
$$
$$
A_L = \left [ 
\begin{matrix}
0 & -0.77*\dot{yaw} & - 0.77*\dot{pitch} \\
0.78*\dot{yaw} & 0 &  0.78*\dot{roll}\\
- 0.011*\dot{pitch} & - 0.011*\dot{roll} & 0\\
\end{matrix}
\right ]

\\
\\

B_L = \left [ 
\begin{matrix}
4.2*(9.81/1000) * (-0.0078*u_1^2 + 0.9784*u_1 - 4.2855)  & -4.2*(9.81/1000) * (-0.0078*u_2^2 + 0.9784*u_2-4.2855)  & -4.2*(9.81/1000) * (-0.0078*u_3^2 + 0.9784*u_3 - 4.2855) & 4.2*(9.81/1000) * (-0.0078*u_4^2 + 0.9784*u_4 - 4.2855)\\
4.1*(9.81/1000) * (-0.0078*u_1^2 + 0.9784*u_1 - 4.2855)  & -4.1*(9.81/1000) * (-0.0078*u_2^2 + 0.9784*u_2-4.2855)  & -4.1*(9.81/1000) * (-0.0078*u_3^2 + 0.9784*u_3 - 4.2855) & 4.1*(9.81/1000) * (-0.0078*u_4^2 + 0.9784*u_4 - 4.2855)\\
5.8*(0.05*9.81/1000) * (-0.0078*u_1^2 + 0.9784*u_1 - 4.2855)  & -5.8*(0.05*9.81/1000) * (-0.0078*u_2^2 + 0.9784*u_2-4.2855)  & -5.8*(0.05*9.81/1000) * (-0.0078*u_3^2 + 0.9784*u_3 - 4.2855) & 5.8*(0.05*9.81/1000) * (-0.0078*u_4^2 + 0.9784*u_4 - 4.2855)\\
\end{matrix}

\right ]
$$