close all
clear all
clc

syms T b gamma beq bed real
syms k1 k2 k3

format long
%% linear model for control law
Ac  = [ 1 1 0;              %position
        0 1 1;              %velocity
        0 0 1-bed];       %time constant
   
Bc  = [0 0 bed]';         %NOTA: considero un nuovo comando u*=b*u

K   = [k1 k2 k3];

Acc = Ac - Bc*K;

% polinomio caratteristico
Ac  = eye(3)*(gamma+1) - Acc;

pretty(collect(det(Ac),gamma))

% gain calculation
T       = 0.02;
tau     = 83e-3;
bed   = T/tau;    
J       = 0.016;
b       = T^2/J;

alfa    = 0.5; 

go1     = bed;
go2     = go1/(2^(1*alfa));
go3     = go1/(2^(2*alfa));

go      = [go1 go2 go3];
c       = poly(-go);
k3 = (c(2)-bed)/bed;
k2 = c(3)/bed;
k1 = c(4)/bed;

K = [k1 k2 k3]

% verifico autovalori
Acc = eval(Acc);
eig(Acc)
1-go'

%% observer
syms l0 m0 m1 m2 bed beq gamma


Aobs = [1 1 0 0 0;
        -l0*bed 1-bed bed 0 bed*m0;
        0 0 1 1 m1;
        0 0 0 1 m2;
        -1 0 0 0 1-beq]
    
Acobs = eye(5)*(gamma+1) - Aobs

pretty(collect(det(Acobs),gamma))

bed   = T/tau; 
alfa    = 0.9;

go1     = bed;
go2     = go1/(2^(1*alfa));
go3     = go1/(2^(2*alfa));
go4     = go1/(2^(3*alfa));
go5     = go1/(2^(4*alfa));

% go4     = 0.3;
% go5     = 0.6;

go_obs          = [go1 go2 go3 go4 go5]
c_obs           = poly(-go_obs);


beq    = c_obs(2)-bed;
l0      = (c_obs(3) - beq*bed)/bed;
m0      = (c_obs(4) - bed*l0*beq)/bed;
m1      = c_obs(5)/bed;
m2      = c_obs(6)/bed;

obs_gains = [m0 m1 m2 l0 beq bed]

Aobs = eval(Aobs);
autovalori_matrice      = eig(Aobs)
autovalori_desiderati   = 1-go_obs'

%%
A = [1 1 0 0 0 0;
     zeros(5,1) Aobs];
Bu = [0 0 bed*b 0 0 0]';
Bl = [0 0 l0*bed 0 0 1]';
% B = [Bu Bl];
B = Bl;
C = [0 1 0 0 0 0;       % y1 = ym = omega stimata
     0 -1 0 0 0 0];     % y2 = em
% D = [0 0; 0 1];
D = [0 1]';

observer    = ss(A,B,C,D,T);
[num,den]   = ss2tf(A,B,C,D,1);


fmax        = 1/(2*T);
fine        = log10(fmax);
N           = 10000;
fx          = logspace(-2,fine,N);
w           = 2*pi*fx;
[mag,phase] = bode(observer,w);
input = 1;
m21         = squeeze(mag(1,input,:)); %  ym/y
m22         = squeeze(mag(2,input,:)); %  em/y

figure()
loglog(fx,m21,'b','Linewidth',2)
hold on
loglog(fx,m22,'r','Linewidth',2)
grid on
xlabel('Hz')
ylabel('rad/sqrt(Hz)')
legend('ym/y','em/y')
axis([1e-2 fmax 1e-4 1e1])
set(gca,'fontsize',14)
% % z = tf('z',T)
% syms z
% H = C*(inv(z*eye(5)-A))*B + D;
% pretty(H)


