close all
clear all
clc

%% reference generator
syms q1 q2 l0 l1 l2 bd gamma real

% senza beta
% Aobs = [  1  1  0   0  0; 
%           0  1  1   0  0;
%         -l0  0  1  l1  l2;
%           0  0  0 1-q1 1;     %xq1
%          -1  0  0  -q2 1];    %xq2

% con beta
Aobs = [  1  1  0     0   0; 
          0  1  1     0   0;
        -bd*l0  0  1-bd  bd*l1  bd*l2;
          0  0  0    1-q1 1;      %xq1
         -1  0  0    -q2  1 ];    %xq2

Aobs

% gamma = lambda - 1 -> lambda = gamma + 1

Ac  = eye(length(Aobs(1,:)))*(gamma+1) - Aobs;
pol = collect(det(Ac),gamma);
pretty(pol)


T       = 0.02;     % time control unit
tau     = 40e-3;    % time constant (from identification)
bd      = T/tau;    % autovalore complementare approssimante

% define complementary eigenvalues
alfa    = 2;

go1     = bd;
go2     = go1/(2^(1*alfa));
go3     = go1/(2^(2*alfa));
go4     = go1/(2^(3*alfa));
go5     = go1/(2^(4*alfa)); 
% go5     = 0.5;
go_obs          = [go1 go2 go3 go4 go5];
c_obs           = poly(-go_obs);


q1 = c_obs(2) - bd;
q2 = c_obs(3) - bd*q1;
l0 = (c_obs(4) - bd*q2)/bd;
l2 = (c_obs(5) - l0*q1*bd)/bd;
l1 = (c_obs(6) - bd*l2*q1 - bd*l0*q2)/bd;

Aobs = eval(Aobs);
Bobs = [0 0 l0*bd 0 1]';
Cobs = [1 0 0 0 0];
Dobs = 0;

sis = ss(Aobs, Bobs, Cobs, Dobs, 0.02);
zpk(sis)

figure()
bode(sis)
grid on

disp('autovalori complementari')
1-eig(Aobs)
go_obs'
fprintf('\n// - -  horizontal reference generator  - -');
fprintf('\n// complementary eigenvalues = [%f, %f, %f, %f, %f]',go_obs);
fprintf('\n// gains');
fprintf('\n#define REF1_BD %f',bd);
fprintf('\n#define REF1_L0 %f',l0);
fprintf('\n#define REF1_L1 %f',l1);
fprintf('\n#define REF1_L2 %f',l2);
fprintf('\n#define REF1_Q1 %f',q1);
fprintf('\n#define REF1_Q2 %f\n',q2);



