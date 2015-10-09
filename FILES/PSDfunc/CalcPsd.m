% function PSD    =    CalcPsd(Data,toDel,PSD)
% Power Spectral Density estimate. 
% Process matlab data to be sent to external software. 
% 
% INPUTS
% Data: A m x n matrix containing the data to be processed the data must be passed ROW-WISE
% First row MUST contain the vector of time
% 
% toDel: the amount of time (in seconds) to be eliminated of the test data. Default is 1 s
% 
% PSD: (optional) An structure with setup parameters to the PSD algorithm, if none in passed 
% the algorithm takes a predefined setup.
% 
% OUTPUTS
% PSD: A k x m matrix with the estimated PSD. The length of the columns is based on the amount 
% of time eliminated. Basically k = (tTotal - tDel)/Ts. The first columns contains the vector of 
% frequency (in Hz). The remaining columns follow the same structure of the input data, one for each variable
% 
% This function also saves a file called 'FromPSD.mat' containing the aforsaid PSD matrix for future processing.
%
% WARNING!!! 
% Once the window of the PSD software is launched, you have to pass the file called 'ToPSD' placed in the
% current working directory path.


function PSD    =    CalcPsd(Data,toDel,PSD)
cd SpectrAn
if (nargin < 3)
    disp('Loaded default PSD parameters...')
    %	tipo di scala delle frequenze (1 log, 0 decimale)
    PSD.Scala           =   1;
    % nuovo 14 lug 2004   num periodogrammi
    PSD.Nperiodo        =   1;
%   eliminazione della media  (0 no, 1 sì  2  eliminazione del trend 3 el.della parabola)
	PSD.ElimMedia       =   1;
%   densità spettrale unilatera *sqrt(2)  (1 sì 0 no) non usato (unilatera)
%    tracciata tra 0 e fmax (fc/2) senza ripiegamento
    PSD.Trac            =   1;
%   finestra di media (1 gaussiana 0 rettangolo)  variabile 1  costante 0
	PSD.Window.Type     =	1;
    PSD.Window.Variable =	1;
%    semibanda finestra (dev. standard per gaussiana) in campioni di frequenza
	PSD.Window.Semiband =   0.2;
%   larghezza effettiva della gaussiana (>0) in multiplo della semibanda
    PSD.Window.width    =   3.5;
%  esponente  della lunghezza finestra
    PSD.Window.Exp      =   .5;
% else
%     PSD.Scala           =   Params.Scala;
%     PSD.Nperiodo        =   Params.Nperiodo;
% 	PSD.ElimMedia       =   Params.ElimMedia;
%     PSD.Trac            =   Params.Trac;
% 	PSD.Window.Type     =	Params.Window.Type;
%     PSD.Window.Variable =	Params.Window.Variable;
% 	PSD.Window.Semiband =   Params.Window.Semiband;
%     PSD.Window.width    =   Params.Window.width;
%     PSD.Window.Exp      =   Params.Window.Exp;
end
if (nargin < 2)
%     disp('No data will be deleted...')
    toDel   =   1;
end
    
delete ToPSD*
namePSDFile =   'ToPSD';
datatobinary(Data,namePSDFile);

StepData=   (Data(1,3)-Data(1,2));

Freq    =   1/StepData;

toProcess   =   Data(1,end)-toDel; % time of test to be considered
Nbits2Proc  =   floor(log2(toProcess*Freq)); % # of bits to be used by PSD algorthm (power of 2)
Vars2Proc   =   size(Data,1) - 1;

if exist([pwd '\Descrizione'],'dir') ~= 0
    rmdir([pwd '\Descrizione'],'s')
end
if exist([pwd '\Rapporti'],'dir') ~= 0
    rmdir([pwd '\Rapporti'],'s')
end
mkdir(pwd,'\Descrizione')
mkdir(pwd,'\Rapporti')

MyFile  =   fopen([pwd '/Descrizione/Spettro.dat'],'w');

fprintf(MyFile,'%  Data generated from Matlab at: %s \n',datestr(now));
fprintf(MyFile,'%  From: %s \n\n\n',[pwd '\CalcPsd.m']);
fprintf(MyFile,'% ************************* Spettro.dat	*************************** \n');
fprintf(MyFile,' =   %d \n',Nbits2Proc);
fprintf(MyFile,' =   %d \n',Vars2Proc);
fprintf(MyFile,' =   %d // tipo di scala delle frequenze (1 log, 0 decimale) \n',PSD.Scala);
fprintf(MyFile,' =   %f \n',StepData);
fprintf(MyFile,' =   %f \n',toDel);
fprintf(MyFile,' =   %d // num periodogrammi \n',PSD.Nperiodo);
fprintf(MyFile,' =   %d // eliminazione della media  (0 no, 1 sì  2  eliminazione del trend 3 el.della parabola) \n',PSD.ElimMedia);
fprintf(MyFile,' =   %d // tracciata tra 0 e fmax (fc/2) senza ripiegamento \n',PSD.Trac);
fprintf(MyFile,' =   %d // finestra di media (1 gaussiana 0 rettangolo)  \n',PSD.Window.Type);
fprintf(MyFile,' =   %d // Finestra variabile 1  costante 0 \n',PSD.Window.Variable);
fprintf(MyFile,' =   %f // semibanda finestra (dev. standard per gaussiana) in campioni di frequenza \n',PSD.Window.Semiband);
fprintf(MyFile,' =   %f // larghezza effettiva della gaussiana (>0) in multiplo della semibanda \n',PSD.Window.width);
fprintf(MyFile,' =   %f // esponente  della lunghezza finestra \n',PSD.Window.Exp);

for i = 1:Vars2Proc
    fprintf(MyFile,' =   %d = 1 = 1  \n',i-1);
end
    
fclose(MyFile);
clear Data

!SpectrAn.exe
PSD         =   LoadPSDData([namePSDFile '.spe']);
save FromPSD PSD

cd ..

end