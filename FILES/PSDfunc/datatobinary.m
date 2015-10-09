function []=datatobinary(data,nome)

aout=fopen(nome,'w');
size_data=size(data);
head=[size_data(1),[0:size_data-2]];
tofile=[[head]' [data]];
count=fwrite(aout,tofile,'float32');
fclose('all');
