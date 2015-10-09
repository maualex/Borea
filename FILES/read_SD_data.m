clear all
close all
clc
set(0,'DefaultFigureWindowStyle','docked')

%%
%path dove è presente il file binario ricevuto dalla gorund station
% OPENLOOP: open loop, fz=20N
% SPENTO  : PER MISURARE LA psd DEL GIROSCOPIO
bin_file    = 'OPENLOOP_BUTTERGYRO20HZ';
file_name   = [bin_file '.bin'];    %il file deve essere all'interno di questa cartella

NFLOAT_TO_WRITE_SD  = 105;           %numero di variabili float
NBYTE_TO_WRITE_SD   = 4*NFLOAT_TO_WRITE_SD + 1;

PACKET_OFFSET_1     = 15;                       % 15 variabili
PACKET_OFFSET_2     = (PACKET_OFFSET_1 + 14);   % 14 variabili
PACKET_OFFSET_3     = (PACKET_OFFSET_2 + 24);   % 24 variabili
PACKET_OFFSET_4     = (PACKET_OFFSET_3 + 30);   % 30 variabili
                                                % 22 variabili
%                                        totale: 105 variabili

%flags
SAVE_FIGURES            = 0;

PLOT_REF_STATES         = 0;
PLOT_OBS_STATES         = 1;
PLOT                    = 1;        

font_size                    = 14;

% 1. apro file in lettura  
f           = fopen(file_name);     


% 2. Conto quanti byte sono stati ricevuti
frewind(f);     %punto al primo byte
n_bytes     = 0;
while ~feof(f)
    byte            = fread(f,1,'uint8'); %read 1 byte
    if ~isempty(byte)
        n_bytes         = n_bytes + 1;
    end
end

resto       = mod(n_bytes,NBYTE_TO_WRITE_SD);           %bytes of the last incomplete message
n_msg       = (n_bytes-resto)/NBYTE_TO_WRITE_SD;        %number of message entirely received

fprintf('\n measurements : %d',n_msg);


misure = zeros(NFLOAT_TO_WRITE_SD,1);
frewind(f);     %punto al primo byte

for i=1:n_msg-1
    %leggo i messaggi contenuti nel file binario
    misure              = fread(f,NFLOAT_TO_WRITE_SD,'float32'); 
    state_machine(i)    = fread(f,1,'uint8');       %leggo 1 byte 
    
    %spacchetto
    sensor_acc(i,:)     = [misure(1) misure(2) misure(3)];      % [m/s^2]
    sensor_ome(i,:)     = [misure(4) misure(5) misure(6)];      % [rad/s]
    sensor_mag(i,:)     = [misure(7) misure(8) misure(9)];      % []
    sensor_gps(i,:)     = [misure(10) misure(11) misure(12)];   % []
    sensor_bat(i)       = misure(13);                           % [V]
    sensor_son(i)       = misure(14);                           % [m]
    sensor_cur(i)       = misure(15);                           % [A]
    
    ref1_xc1(i)         = misure(PACKET_OFFSET_1+1); 
    ref1_xc2(i)         = misure(PACKET_OFFSET_1+2); 
    ref1_xc3(i)         = misure(PACKET_OFFSET_1+3); 
    ref1_xq1(i)         = misure(PACKET_OFFSET_1+4); 
    ref1_xq2(i)         = misure(PACKET_OFFSET_1+5); 
    ref1_u(i)           = misure(PACKET_OFFSET_1+6);
    ref1_in(i)          = misure(PACKET_OFFSET_1+7);
    
    ref2_xc1(i)         = misure(PACKET_OFFSET_1+8); 
    ref2_xc2(i)         = misure(PACKET_OFFSET_1+9); 
    ref2_xc3(i)         = misure(PACKET_OFFSET_1+10); 
    ref2_xq1(i)         = misure(PACKET_OFFSET_1+11); 
    ref2_xq2(i)         = misure(PACKET_OFFSET_1+12); 
    ref2_u(i)           = misure(PACKET_OFFSET_1+13);
    ref2_in(i)          = misure(PACKET_OFFSET_1+14);
    
    
    obs_XC1(i,:)        = [misure(PACKET_OFFSET_2+1) misure(PACKET_OFFSET_2+7) misure(PACKET_OFFSET_2+13)]; % angle     [rad] 
    obs_XC2(i,:)        = [misure(PACKET_OFFSET_2+2) misure(PACKET_OFFSET_2+8) misure(PACKET_OFFSET_2+14)]; % d angle   [rad]
    obs_XC3(i,:)        = [misure(PACKET_OFFSET_2+3) misure(PACKET_OFFSET_2+9) misure(PACKET_OFFSET_2+15)]; % dd angle  [rad]
    obs_XD1(i,:)        = [misure(PACKET_OFFSET_2+4) misure(PACKET_OFFSET_2+10) misure(PACKET_OFFSET_2+16)];
    obs_XD2(i,:)        = [misure(PACKET_OFFSET_2+5) misure(PACKET_OFFSET_2+11) misure(PACKET_OFFSET_2+17)];
    obs_XQ1(i,:)        = [misure(PACKET_OFFSET_2+6) misure(PACKET_OFFSET_2+12) misure(PACKET_OFFSET_2+18)];
    obs_EM(i,:)         = [misure(PACKET_OFFSET_2+19) misure(PACKET_OFFSET_2+20) misure(PACKET_OFFSET_2+21)];
    obs_y(i,:)          = [misure(PACKET_OFFSET_2+22) misure(PACKET_OFFSET_2+23) misure(PACKET_OFFSET_2+24)]; 
    
    control1_err(i,:)       = [misure(PACKET_OFFSET_3+1) misure(PACKET_OFFSET_3+2) misure(PACKET_OFFSET_3+3)];
    control1_fb(i)          = misure(PACKET_OFFSET_3+4);
    control1_dist(i)        = misure(PACKET_OFFSET_3+5);
    control1_ref(i)         = misure(PACKET_OFFSET_3+6);
    control1_uc_obs_p(i)    = misure(PACKET_OFFSET_3+7);
    control1_uc_torque(i)   = misure(PACKET_OFFSET_3+8);
    control1_uc_torque_r(i) = misure(PACKET_OFFSET_3+9);
    control1_uc_obs(i)      = misure(PACKET_OFFSET_3+10);
    
    w = 10;
    control2_err(i,:)       = [misure(PACKET_OFFSET_3+1+w) misure(PACKET_OFFSET_3+2+w) misure(PACKET_OFFSET_3+3+w)];
    control2_fb(i)          = misure(PACKET_OFFSET_3+4+w);
    control2_dist(i)        = misure(PACKET_OFFSET_3+5+w);
    control2_ref(i)         = misure(PACKET_OFFSET_3+6+w);
    control2_uc_obs_p(i)    = misure(PACKET_OFFSET_3+7+w);
    control2_uc_torque(i)   = misure(PACKET_OFFSET_3+8+w);
    control2_uc_torque_r(i) = misure(PACKET_OFFSET_3+9+w);
    control2_uc_obs(i)      = misure(PACKET_OFFSET_3+10+w);
    
    w = 20;
    control3_err(i,:)       = [misure(PACKET_OFFSET_3+1+w) misure(PACKET_OFFSET_3+2+w) misure(PACKET_OFFSET_3+3+w)];
    control3_fb(i)          = misure(PACKET_OFFSET_3+4+w);
    control3_dist(i)        = misure(PACKET_OFFSET_3+5+w);
    control3_ref(i)         = misure(PACKET_OFFSET_3+6+w);
    control3_uc_obs_p(i)    = misure(PACKET_OFFSET_3+7+w);
    control3_uc_torque(i)   = misure(PACKET_OFFSET_3+8+w);
    control3_uc_torque_r(i) = misure(PACKET_OFFSET_3+9+w);
    control3_uc_obs(i)      = misure(PACKET_OFFSET_3+10+w);
    
    %commands
    command_uc(i,:)         = [misure(PACKET_OFFSET_4+1) misure(PACKET_OFFSET_4+2) misure(PACKET_OFFSET_4+3) misure(PACKET_OFFSET_4+4)];
    command_thrust(i,:)     = [misure(PACKET_OFFSET_4+5) misure(PACKET_OFFSET_4+6) misure(PACKET_OFFSET_4+7) misure(PACKET_OFFSET_4+8)];
    command_digital(i,:)    = [misure(PACKET_OFFSET_4+9) misure(PACKET_OFFSET_4+10) misure(PACKET_OFFSET_4+11) misure(PACKET_OFFSET_4+12)];
    
    %attitude
    attitude(i,:)           = [misure(PACKET_OFFSET_4+13) misure(PACKET_OFFSET_4+14) misure(PACKET_OFFSET_4+15)];
    
    %PID
    pid_e(i,:)              = [misure(PACKET_OFFSET_4+16) misure(PACKET_OFFSET_4+17) misure(PACKET_OFFSET_4+18)];
    pid_x(i,:)              = [misure(PACKET_OFFSET_4+19) misure(PACKET_OFFSET_4+20) misure(PACKET_OFFSET_4+21)];
    
    %control step
    step(i)                 = misure(PACKET_OFFSET_4+22);
end

fclose(f);

%%
Tcon = 0.02;
t               = Tcon*(step-step(1)); 

%% data structure for simulation
% test_t                     = t;
% test_xc0.signals.values    = pid_xp_XC0;
% test_xc0.time              = t;
% test_xc1.signals.values    = pid_xp_XC1;
% test_xc1.time              = t;
% test_xc2.signals.values    = pid_x_XC2;
% test_xc2.time              = t;
% test_xd1.signals.values    = pid_xp_XD1;
% test_xd1.time              = t;
% test_xd2.signals.values    = pid_xp_XD2;
% test_xd2.time              = t;
% test_xq1.signals.values    = pid_xp_XQ1;
% test_xq1.time              = t;
% test_xu1.signals.values    = pid_xp_XU1;
% test_xu1.time              = t;
% test_xu2.signals.values    = pid_xp_XU2;
% test_xu2.time              = t;
% test_upid.signals.values   = u_PID;
% test_upid.time             = t;
% test_gyro.signals.values   = gy;
% test_gyro.time             = t;
% test_angle.signals.values  = theta;
% test_angle.time            = t;
% 
% save('test1.mat','test_t','test_xc0','test_xc1','test_xc2','test_xd1','test_xd2','test_xq1','test_xu1','test_xu2','test_upid','test_gyro','test_angle')

%% data plot
if(PLOT)

        % command
        h(1) = figure('Name','Commands');
        subplot(221)
%         h(1) = figure('Name','uc1');
        stairs(t,command_uc(:,1),'Linewidth',1,'Color','b');
        grid on
        legend('Fz')
        xlabel('time [s]')
        ylabel('[N]')
        title('command vertical thrust')
        set(gca,'fontsize',font_size)
        
        subplot(222)
%         h(2) = figure('Name','torques');
        stairs(t,command_uc(:,2),'Linewidth',1,'Color','r');
        hold on
        stairs(t,command_uc(:,3),'Linewidth',1,'Color','g');
        stairs(t,command_uc(:,4),'Linewidth',1,'Color','b');
        grid on
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[Nm]')
        title('command torques')
        set(gca,'fontsize',font_size)
        
        subplot(223)
%         h(3) = figure('Name','Thrust');
        stairs(t,command_thrust(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,command_thrust(:,2),'Linewidth',1,'Color','g');
        stairs(t,command_thrust(:,3),'Linewidth',1,'Color','b');
        stairs(t,command_thrust(:,4),'Linewidth',1,'Color','k');
        grid on
        legend('Actuator 1','Actuator 2','Actuator 3','Actuator 4')
        xlabel('time [s]')
        ylabel('[N]')
        title('Actuator thrust')
        set(gca,'fontsize',font_size)
        
        subplot(224)
%         h(4) = figure('Name','Digital command');
        stairs(t,command_digital(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,command_digital(:,2),'Linewidth',1,'Color','g');
        stairs(t,command_digital(:,3),'Linewidth',1,'Color','b');
        stairs(t,command_digital(:,4),'Linewidth',1,'Color','k');
        grid on
        legend('Actuator 1','Actuator 2','Actuator 3','Actuator 4')
        xlabel('time [s]')
        ylabel('[digital]')
        title('Actuator digital command')
        set(gca,'fontsize',font_size)
        
        % PID
        figure('Name','PID');
        subplot(211)
%         h(2) = figure('Name','PID errors');
        stairs(t,pid_e(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,pid_e(:,2),'Linewidth',1,'Color','g');
        stairs(t,pid_e(:,3),'Linewidth',1,'Color','b');
        grid on
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('PID tracking error')
        set(gca,'fontsize',font_size)
        subplot(212)
%         h(2) = figure('Name','PID state');
        stairs(t,pid_x(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,pid_x(:,2),'Linewidth',1,'Color','g');
        stairs(t,pid_x(:,3),'Linewidth',1,'Color','b');
        grid on
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('PID integrative state')
        set(gca,'fontsize',font_size)
        
        % sensor
        h(5) = figure('Name','measurement');
        stairs(t,sensor_acc(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,sensor_acc(:,2),'Linewidth',1,'Color','g');
        stairs(t,sensor_acc(:,3),'Linewidth',1,'Color','b');
        grid on
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[m/s^2]')
        title('IMU accelerometer(50Hz)')
        set(gca,'fontsize',font_size)

        h(6) = figure('Name','measurement');
        stairs(t,sensor_ome(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,sensor_ome(:,2),'Linewidth',1,'Color','g');
        stairs(t,sensor_ome(:,3),'Linewidth',1,'Color','b');
        grid on
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad/s]')
        title('IMU gyroscope(50Hz)')
        set(gca,'fontsize',font_size)

        h(7) = figure('Name','attitude');
        stairs(t,attitude(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,attitude(:,2),'Linewidth',1,'Color','g');
        stairs(t,attitude(:,3),'Linewidth',1,'Color','b');
        grid on
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('Attitude Euler angles')
        set(gca,'fontsize',font_size)

        h(8) = figure('Name','sonar');
        stairs(t,sensor_son,'Linewidth',1,'Color','r');
        grid on
        legend('ultrasound')
        xlabel('time [s]')
        ylabel('[m]')
        title('ultrasound')
        set(gca,'fontsize',font_size)
        
        h(9) = figure('Name','batteria');
        stairs(t,sensor_bat,'Linewidth',1,'Color','r');
        grid on
        legend('battery')
        xlabel('time [s]')
        ylabel('[V]')
        title('battery')
        set(gca,'fontsize',font_size)
        
        
        % observer
        if(PLOT_OBS_STATES)
        h(10) = figure('Name','model error');
        stairs(t,obs_EM(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_EM(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_EM(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('e_m_x','e_m_y','e_m_z')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('Model error')
        set(gca,'fontsize',font_size)
        
        h(11) = figure('Name','Xc1');
        stairs(t,obs_XC1(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_XC1(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_XC1(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('phi','theta','psi')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('state x_c_1')
        set(gca,'fontsize',font_size)
        
        h(12) = figure('Name','Xc2');
        stairs(t,obs_XC2(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_XC2(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_XC2(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('phi punto','theta punto','psi punto')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
%         title('angular rates estimation')
        title('state x_c_2')
        set(gca,'fontsize',font_size)
        
        h(13) = figure('Name','Xc3');
        stairs(t,obs_XC3(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_XC3(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_XC3(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('phi punto','theta punto','psi punto')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
%         title('actuator state')
        title('state x_c_3')
        set(gca,'fontsize',font_size)

        h(14) = figure('Name','XD1');
        stairs(t,obs_XD1(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_XD1(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_XD1(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('x_d_1_x','x_d_1_y','x_d_1_z')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
%         title('xd1')
        title('state x_d_1')
        set(gca,'fontsize',font_size)

        h(15) = figure('Name','XD2');
        stairs(t,obs_XD2(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_XD2(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_XD2(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('x_d_2_x','x_d_2_y','x_d_2_z')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
%         title('xd2')
        title('state x_d_2')
        set(gca,'fontsize',font_size)

        h(16) = figure('Name','XQ');
        stairs(t,obs_XQ1(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_XQ1(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_XQ1(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('x_q_x','x_q_y','x_q_z')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('Noise estimator state')
        set(gca,'fontsize',font_size)
        
        h(18) = figure('Name','obs_y');
        stairs(t,obs_y(:,1),'Linewidth',1,'Color','r');
        hold on
        stairs(t,obs_y(:,2),'Linewidth',1,'Color','g');
        stairs(t,obs_y(:,3),'Linewidth',1,'Color','b');
        grid on
%         legend('obs_y_x','obs_y_y','obs_y_z')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('Gyroscope trasformed (observer input)')
        set(gca,'fontsize',font_size)
        
        h(19) = figure('Name','obs command');
        stairs(t,control1_uc_obs,'Linewidth',1,'Color','r');
        hold on
        stairs(t,control2_uc_obs,'Linewidth',1,'Color','g');
        stairs(t,control3_uc_obs,'Linewidth',1,'Color','b');
        grid on
%         legend('obs_u_x','obs_u_y','obs_u_z')
        legend('X-axis','Y-axis','Z-axis')
        xlabel('time [s]')
        ylabel('[rad]')
        title('Command trasformed (observer input)')
        set(gca,'fontsize',font_size)
        end
        
        % control law
        
%tracking errors
        h(20) = figure('Name','(X-axis)error');
        subplot(311)
        stairs(t,control1_err(:,1),'Linewidth',1,'Color','r');
        grid on
        ylabel('[rad]')
        legend('position')
        title('Tracking error (X-axis)')
        set(gca,'fontsize',font_size)
        
        subplot(312)
        stairs(t,control1_err(:,2),'Linewidth',1,'Color','g');
        grid on
        ylabel('[rad]')
        legend('velocity')
        set(gca,'fontsize',font_size)
        
        subplot(313)
        stairs(t,control1_err(:,3),'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('velocity time constant')
        set(gca,'fontsize',font_size)
        
        h(20) = figure('Name','(Y-axis)error');
        subplot(311)
        stairs(t,control2_err(:,1),'Linewidth',1,'Color','r');
        grid on
        ylabel('[rad]')
        legend('position')
        title('Tracking error (Y-axis)')
        set(gca,'fontsize',font_size)
        
        subplot(312)
        stairs(t,control2_err(:,2),'Linewidth',1,'Color','g');
        grid on
        ylabel('[rad]')
        legend('velocity')
        set(gca,'fontsize',font_size)
        
        subplot(313)
        stairs(t,control2_err(:,3),'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('velocity time constant')
        set(gca,'fontsize',font_size)
        
        h(20) = figure('Name','(Z-axis)error');
        subplot(311)
        stairs(t,control3_err(:,1),'Linewidth',1,'Color','r');
        grid on
        ylabel('[rad]')
        legend('position')
        title('Tracking error (Z-axis)')
        set(gca,'fontsize',font_size)
        
        subplot(312)
        stairs(t,control3_err(:,2),'Linewidth',1,'Color','g');
        grid on
        ylabel('[rad]')
        legend('velocity')
        set(gca,'fontsize',font_size)
        
        subplot(313)
        stairs(t,control3_err(:,3),'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('velocity time constant')
        set(gca,'fontsize',font_size)
        
        
        
%command components        
        h(21) = figure('Name','(X-axis)Command');
        stairs(t,control1_fb,'Linewidth',1,'Color','r');
        hold on
        stairs(t,control1_dist,'Linewidth',1,'Color','g');
        stairs(t,control1_ref,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('feedback','dist compensation','reference')
        set(gca,'fontsize',font_size)
        title('Command components (X-axis)')
        
        h(21) = figure('Name','(Y-axis)Command');
        stairs(t,control2_fb,'Linewidth',1,'Color','r');
        hold on
        stairs(t,control2_dist,'Linewidth',1,'Color','g');
        stairs(t,control2_ref,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('feedback','dist compensation','reference')
        set(gca,'fontsize',font_size)
        title('Command components (Y-axis)')
        

        h(21) = figure('Name','(Z-axis)Command');
        stairs(t,control3_fb,'Linewidth',1,'Color','r');
        hold on
        stairs(t,control3_dist,'Linewidth',1,'Color','g');
        stairs(t,control3_ref,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('feedback','dist compensation','reference')
        set(gca,'fontsize',font_size)
        title('Command components (Z-axis)')
        
        
        
%comando totale
        h(22) = figure('Name','command (trasformated)');
        stairs(t,control1_uc_obs_p,'Linewidth',1,'Color','r');
        hold on
        stairs(t,control2_uc_obs_p,'Linewidth',1,'Color','g');
        stairs(t,control3_uc_obs_p,'Linewidth',1,'Color','b');
        grid on
        ylabel('[rad]')
        legend('X-axis','Y-axis','Z-axis')
        set(gca,'fontsize',font_size)
        title('command (trasformated)')
        
        
        h(23) = figure('Name','plant command');
        stairs(t,control1_uc_torque,'Linewidth',1,'Color','r');
        hold on
        stairs(t,control2_uc_torque,'Linewidth',1,'Color','g');
        stairs(t,control3_uc_torque,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[Nm]')
        legend('X-axis','Y-axis','Z-axis')
        set(gca,'fontsize',font_size)
        title('command to the plant')
        
        h(24) = figure('Name','plant command recostructed');
        stairs(t,control1_uc_torque_r,'Linewidth',1,'Color','r');
        hold on
        stairs(t,control2_uc_torque_r,'Linewidth',1,'Color','g');
        stairs(t,control3_uc_torque_r,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[Nm]')
        legend('X-axis','Y-axis','Z-axis')
        set(gca,'fontsize',font_size)
        title('command to the plant (recostructed)')
        

%         h(25) = figure('Name','machine state');
%         stairs(t,state_machine,'Linewidth',1,'Color','r');
%         grid on
%         xlabel('time [s]')
%         ylabel('state [int]')
%         title('machine state')
%         set(gca,'fontsize',font_size)
        
        
        % reference generator
        
        h(26) = figure('Name','ref_genX');
        stairs(t,ref1_xc1,'Linewidth',1,'Color','b');
        hold on
        stairs(t,ref1_in,'Linewidth',1,'Color','r');
        grid on
        ylabel('[rad]')
        legend('x_c_1','input1')
        set(gca,'fontsize',font_size)
        title('reference x_c_1 (X-axis)')

        if(PLOT_REF_STATES)      
        h(27) = figure('Name','ref_genXstates');
        subplot(311)
        stairs(t,ref1_xc2,'Linewidth',1,'Color','b');
        grid on
        ylabel('[rad]')
        legend('x_c_2')
        set(gca,'fontsize',font_size)
        title('reference states (X-axis)')
        subplot(312)
        stairs(t,ref1_xc3,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('x_c_3')
        set(gca,'fontsize',font_size)
        subplot(313)
        stairs(t,ref1_u,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('u_r_e_f')
        set(gca,'fontsize',font_size)
        end
        
        h(27) = figure('Name','ref_genY');
        stairs(t,ref2_xc1,'Linewidth',1,'Color','b');
        hold on
        stairs(t,ref2_in,'Linewidth',1,'Color','r');
        grid on
        ylabel('[rad]')
        legend('x_c_1','input2')
        set(gca,'fontsize',font_size)
        title('reference x_c_1 (Y-axis)')
        
        if(PLOT_REF_STATES) 
        h(28) = figure('Name','ref_genYstates');
        subplot(311)
        stairs(t,ref2_xc2,'Linewidth',1,'Color','b');
        grid on
        ylabel('[rad]')
        legend('x_c_2')
        set(gca,'fontsize',font_size)
        title('reference states (Y-axis)')
        subplot(312)
        stairs(t,ref2_xc3,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('x_c_3')
        set(gca,'fontsize',font_size)
        subplot(313)
        stairs(t,ref2_u,'Linewidth',1,'Color','b');
        grid on
        xlabel('time [s]')
        ylabel('[rad]')
        legend('u_r_e_f')
        set(gca,'fontsize',font_size)
        end
        
end

return

%% PSD
vector  = pid_e;
fc      = 50;
t0      = 5*fc;

PSD_3axial( vector, t, t0, '[rad/sqrt(Hz)]', font_size)

%% PSD errors
fc      = 50;
t0      = 5*fc;
te      = numel(t); %60*fc;
tt      = (t(t0:te)-t(t0))';  %time input

mis     = obs_EM(t0:te,2).*Tcon^-1;
dat     = [tt,mis];
PSD_me  = CalcPsd(dat',0);

mis     = sensor_ome(t0:te,2);
dat     = [tt,mis];
PSD_ome = CalcPsd(dat',0);

mis     = sensor_bat(t0:te)';
dat     = [tt,mis];
PSD_batt = CalcPsd(dat',0);

h(14) = figure('Name','err PSD');
loglog(PSD_me(:,1),PSD_me(:,2),'r','Linewidth',2);
grid on
hold on
loglog(PSD_ome(:,1),PSD_ome(:,2),'g','Linewidth',2);
loglog(PSD_batt(:,1),PSD_batt(:,2),'b','Linewidth',2);
legend('model error','measurement')
title('PSD errors (50Hz signal)')
xlabel('Frequenza [Hz]')
ylabel('[rad/s/sqrt(Hz)]')
set(gca,'fontsize',font_size)

%%
h(15) = figure('Name','err');
stairs(tt,obs_EM(t0:end).*Tcon^-1,'Linewidth',1,'Color','r');
grid on
hold on
stairs(tt,obs_actual_track_e(t0:end),'Linewidth',1,'Color','g');
stairs(tt,obs_true_track_e(t0:end),'Linewidth',1,'Color','b');
legend('model','actual','true')
xlabel('time [s]')
ylabel('[rad/s]')
title('Errors (50Hz signal)')
set(gca,'fontsize',font_size)



%%
mis     = ext_gx(t0:end);
dat     = [tt,mis];
PSD_X   = CalcPsd(dat',0);

mis     = ext_gy(t0:end);
dat     = [tt,mis];
PSD_Y   = CalcPsd(dat',0);

mis     = ext_gz(t0:end);
dat     = [tt,mis];
PSD_Z   = CalcPsd(dat',0);

figure();
loglog(PSD_X(:,1),PSD_X(:,2),'r','Linewidth',2);
hold on
loglog(PSD_Y(:,1),PSD_Y(:,2),'g','Linewidth',2);
loglog(PSD_Z(:,1),PSD_Z(:,2),'b','Linewidth',2);
grid on
legend('ext gx','ext gy','ext gz')
title('50Hz signal')
xlabel('Frequenza [Hz]')
ylabel('[rad/s/sqrt(Hz)]')


%% PSD xd1
mis = obs_XD1(t0:end);
tt  = t(t0:end)-t(t0);
dat=[tt,mis];
PSD=CalcPsd(dat',0);


h(16) = figure();
loglog(PSD(:,1),PSD(:,2),'r','Linewidth',2);
grid on
legend('disturbance xd1')
title('50Hz signal')
xlabel('Frequenza [Hz]')
ylabel('[rad/s^2/sqrt(Hz)]')

%% prova PSD
psd_mis_y   = obs_y(1650:end); %((60*50):end-710);
psd_mis_xc2 = obs_XC2(1650:end); %((60*50):end-710);
psd_t       = t(1650:end); %((60*50):end-710);
std(psd_mis_y)
std(psd_mis_xc2)
std(control2_err((40*50):(100*50),2))
std(obs_EM((40*50):(100*50)))

tt  = psd_t;

mis = psd_mis_xc2;
dat=[tt,mis];
PSD1=CalcPsd(dat',0);

mis = psd_mis_y;
dat=[tt,mis];
PSD2=CalcPsd(dat',0);

figure();
loglog(PSD1(:,1),PSD1(:,2),'r','Linewidth',2);
grid on
hold on
loglog(PSD2(:,1),PSD2(:,2),'b','Linewidth',2);
title('50Hz signal')
xlabel('Frequenza [Hz]')
ylabel('[rad/s^2/sqrt(Hz)]')
legend('stima','misura')

%% salvo figure
if SAVE_FIGURES == 1
    
    % pwd ritorna l'indirizzo assoluto della cartella corrente
    %[parentFolder current_folder] = fileparts(pwd);

    newSubFolder = test_name;
    % Finally, create the folder if it doesn't exist already.
    if ~exist(newSubFolder, 'dir')
      mkdir(newSubFolder);
    end
    
    str = newSubFolder; %[pwd,'\',newSubFolder];

    f_01 = [nome_prova '__01___u_plant.fig'];
    f_02 = [nome_prova '__02___thrust.fig'];
    f_03 = [nome_prova '__03___gyro_measurement.fig'];
    f_04 = [nome_prova '__04___attitude.fig'];
    f_05 = [nome_prova '__05___xc1.fig'];
    f_06 = [nome_prova '__06___em.fig'];
    f_07 = [nome_prova '__07___xc2.fig'];
    f_08 = [nome_prova '__08___xd1.fig'];
    f_09 = [nome_prova '__09___angles_diff.fig'];
    f_10 = [nome_prova '__10___xq.fig'];
    f_11 = [nome_prova '__11___u_obs.fig'];
    f_12 = [nome_prova '__12___PSD_errors.fig'];
    f_13 = [nome_prova '__13___errors.fig'];
    
    saveas(h(1),f_01);
    saveas(h(2),f_02);
    saveas(h(3),f_03);
    saveas(h(4),f_04);
    saveas(h(5),f_05);
    saveas(h(6),f_06);
    saveas(h(7),f_07);
    saveas(h(8),f_08);
    saveas(h(9),f_09);
    saveas(h(10),f_10);
    saveas(h(11),f_11);
    saveas(h(14),f_12);
    saveas(h(15),f_13);
    
    movefile(f_01,str);
    movefile(f_02,str);
    movefile(f_03,str);
    movefile(f_04,str);
    movefile(f_05,str);
    movefile(f_06,str);
    movefile(f_07,str);
    movefile(f_08,str);
    movefile(f_09,str);
    movefile(f_10,str);
    movefile(f_11,str);
    movefile(f_12,str);
    movefile(f_13,str);
end
