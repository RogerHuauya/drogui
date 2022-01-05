
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
\ddot{roll} = 10*U_1 - 0.77*\dot{pitch}*\dot{yaw}\\
\ddot{pitch} = 10*U_2 + 0.78*\dot{roll}*\dot{yaw};\\
\ddot{yaw} = 5.8*U_3 - 0.011*\dot{pitch}*\dot{roll};\\


\\
\dot{x_1} = x_4\\
\dot{x_2} = x_5\\
\dot{x_3} = x_6\\

\dot{x_4} = 10*U_1 - 0.77*x_5*x_6\\
\dot{x_5} = 10*U_2 + 0.78*x_4*x_6;\\
\dot{x_6} = 5.8*U_3 - 0.011*x_5*x_4;\\


\\
\\

f(x) = \left [ 
\begin{matrix}

x_4\\
x_5\\
x_6\\
-0.77*x_5*x_6\\
0.78*x_4*x_6;\\
-0.011*x_5*x_4;\\

\end{matrix}
\right ]

\\
\\

g(x) = 
\left [ 
\begin{matrix}

0  & 0  & 0   & \\
0  & 0  & 0   & \\
0  & 0  & 0   & \\
10 & 0  & 0   & \\
0  & 10 & 0   & \\
0  & 0  & 5.8 & \\

\end{matrix}
\right ]

\\

V = \left [ \begin{matrix}
y_1^1 \\
y_2^1 \\
y_3^1 \\
\end{matrix}
\right ] = \left [ \begin{matrix}
L_fh_1(x) + \sum_{i=1}^{3} L_{g_i}h_1u_i  \\
L_fh_2(x) + \sum_{i=1}^{3} L_{g_i}h_2u_i  \\
L_fh_3(x) + \sum_{i=1}^{3} L_{g_i}h_3u_i  \\
\end{matrix}
\right ] 

\\
H(x) = \left [ \begin{matrix}
h_1(x) \\
h_2(x) \\
h_3(x) \\
\end{matrix}
\right ] =  \left [ \begin{matrix}
x_1 \\
x_2 \\
x_3 \\
\end{matrix}
\right ] 
\\

\alpha (x) = \left [ \begin{matrix}
L_f^2h_1(x) & \\
L_f^2h_2(x) & \\
L_f^2h_3(x) & \\
\end{matrix}
\right ] = \left [ \begin{matrix}
\sum_{i=1}^6 \frac{\partial h_1}{\partial x_i}f_i(x) \\ 
\sum_{i=1}^6 \frac{\partial h_2}{\partial x_i}f_i(x) \\ 
\sum_{i=1}^6 \frac{\partial h_3}{\partial x_i}f_i(x) \\ 
\end{matrix}
\right ] \\
\\


\alpha(x) = \left [ 
\begin{matrix}

-0.77*x_5*x_6\\
0.78*x_4*x_6;\\
-0.011*x_5*x_4;\\

\end{matrix}
\right ]
\\
V(x) = \left [ 
\begin{matrix}

-0.77*x_5*x_6 + 10*u_1\\
0.78*x_4*x_6 + 10*u_2;\\
-0.011*x_5*x_4 + 5.8*u_3;\\

\end{matrix}
\right ]

\\

\beta (x) = \left [ \begin{matrix}
L_{g_1}(L_fh_1(x)) & L_{g_2}(L_fh_1(x)) &L_{g_2}(L_fh_1(x)) &\\
L_{g_1}(L_fh_2(x)) & L_{g_2}(L_fh_2(x)) &L_{g_2}(L_fh_2(x)) &\\
L_{g_1}(L_fh_3(x)) & L_{g_2}(L_fh_3(x)) &L_{g_2}(L_fh_3(x)) &\\
\end{matrix}
\right ] = \left [ \begin{matrix}
\sum_{i=0}^6 \frac{\partial h_1}{\partial x_i}g_1(x) & \frac{\partial h_1}{\partial x_2}g_2(x) &  \frac{\partial h_1}{\partial x_3}g_3(x) \\
\frac{\partial h_2}{\partial x_1}g_1(x) & \frac{\partial h_2}{\partial x_2}g_2(x) &  \frac{\partial h_2}{\partial x_3}g_3(x) \\
\frac{\partial h_3}{\partial x_1}g_1(x) & \frac{\partial h_3}{\partial x_2}g_2(x) &  \frac{\partial h_3}{\partial x_3}g_3(x) \\
\end{matrix}
\right ] \\

\beta (x) =  
\left [ \begin{matrix}
10 & 0 & 0 &\\ 
0 & 10 & 0 &\\ 
0 & 0 & 5.8 &\\ 
\end{matrix}
\right ] 
$$
$$
\\
A_L = \left [ 
\begin{matrix}
0 & 0 & 0 & 1          & 0          & 0         & \\
0 & 0 & 0 & 0          & 1          & 0         & \\
0 & 0 & 0 & 0          & 0          & 1         & \\
0 & 0 & 0 & 0          & -0.77*x_6  & -0.77*x_5 & \\
0 & 0 & 0 & 0.78*x_6   & 0          & 0.78*x_4  & \\
0 & 0 & 0 & -0.011*x_5 & -0.011*x_4 & 0         & \\
\end{matrix}
\right ]

\\
\phi (x) = \left [ \begin{matrix}
h_1 & L_fh_1 & \\
h_2 & L_fh_2 & \\
h_3 & L_fh_3 & \\
\end{matrix}
\right ] = \left [ \begin{matrix}
x_1 & & \\
x_2 & L_fh_2 & \\
x_3 & L_fh_3 & \\
\end{matrix}
\right ] \\
$$
$$
B_L = \left [ 
\begin{matrix}
0  & 0  & 0   & \\
0  & 0  & 0   & \\
0  & 0  & 0   & \\
10 & 0  & 0   & \\
0  & 10 & 0   & \\
0  & 0  & 5.8 & \\
\end{matrix}

\right ]
$$
$$
Y = \left [ \begin{matrix}
y_1\\
y_2\\
y_3\\
\end{matrix}
\right ] =  \left [ \begin{matrix}
x_1\\
x_2\\
x_3\\
\end{matrix}
\right ] \\

$$
