close all
clear all
clc

syms kp kd ki T b gamma

A = [1 1 0;
    -kp*b 1-kd*b/T ki*T*b;
    -1 0 1]

Ac = eye(3)*(gamma+1) - A

pretty(collect(det(Ac),gamma))

% go          = [0.24 0.1 0.01]
go          = [0.5 0.2 0.005]
c=poly(-go);

T = 0.02;
J  = 0.032; 
b = T^2/J;

Kp = c(3)/b;
Kd = c(2)*T/b;
Ki = c(4)/(b*T);

kp = Kp;
ki = Ki;
kd = Kd;
A=eval(A);
1-eig(A)'
K_PID = [Kp Ki Kd]



%%
B = [0 0; kp*b kd*b/T; 1 0];
C = [1 0 0];
D = [0 0];

model = ss(A,B,C,D,T);

zpk(model)

figure()
bode(model)
grid on
