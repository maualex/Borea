function [ ] = PSD_3axial( input, time, t0, yaxis_label, font_size )
% Descrption: plot PSD for 3 dimentional variable
% inputs size
%   vector : n x 3
%   time   : 1 x n

tt      = (time(t0:end)-time(t0))';  %time input
mis     = input(t0:end,1);
dat     = [tt,mis];
PSD_X   = CalcPsd(dat',0);

mis     = input(t0:end,2);
dat     = [tt,mis];
PSD_Y   = CalcPsd(dat',0);

mis     = input(t0:end,3);
dat     = [tt,mis];
PSD_Z   = CalcPsd(dat',0);

figure();
loglog(PSD_X(:,1),PSD_X(:,2),'r','Linewidth',2);
hold on
loglog(PSD_Y(:,1),PSD_Y(:,2),'g','Linewidth',2);
loglog(PSD_Z(:,1),PSD_Z(:,2),'b','Linewidth',2);
grid on
legend('X-axis','Y-axis','Z-axis')
title('PSD')
xlabel('Frequency [Hz]')
ylabel(yaxis_label)
set(gca,'fontsize',font_size)
end

