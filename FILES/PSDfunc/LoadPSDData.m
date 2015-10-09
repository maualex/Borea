function [PSD] =   LoadPSDData(NameFile)

myFile  =   fopen(NameFile);

lriga=fread(myFile,1,'int32');
nvar=lriga/4;
frewind(myFile);
[vuoto,n]=fread(myFile,inf,'float32');
frewind(myFile);

npunti=n/(nvar+2);
[yvar]  =   fread(myFile,[nvar+2,npunti],'float32');

if(~fclose(myFile))
    disp('File closed')
end

tini = 1;
tfine = npunti;   

df=yvar(2,npunti)/npunti/2;
if tfine>npunti, tfine=npunti; end

asse=linspace(tini,tfine,tfine-tini+1);

PSD =   10.^(yvar(2,asse)-df)';

for i=1:nvar-1
    PSD   =   [PSD (10.^(yvar(2+i,asse)/10))'];
end





