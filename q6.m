clear all;

esc_giro=250;

nome='q6data.txt';
fid=fopen(nome,'r');
%Verificar se abriu o arquivo
if (fid==-1)
    fprintf(1,'Nao abriu arquivo [%s]. Parar!\n',nome);
    return;
end


dados = fscanf(fid,'%d');
fclose(fid);

%fprintf('%d\n', dados);

total = size(dados,1);
total_leituras = total/3;

gx = zeros(1,total_leituras);
gy = gx;
gz = gx;

for i=1:total_leituras
    gx(1,i) = dados(((i-1)*3)+1 , 1 );
    gy(1,i) = dados(((i-1)*3)+2 , 1 );
    gz(1,i) = dados(((i-1)*3)+3 , 1 );
end

% Converter giros em "graus/seg"
gx=esc_giro*(gx/32767);
gy=esc_giro*(gy/32767);
gz=esc_giro*(gz/32767);

% Desenhar gráficos
%gx
figure(1);
plot(gx);
grid;
title('Giro: eixo X em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

%gy
figure(2);
plot(gy);
grid;
title('Giro: eixo Y em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

%gz
figure(3);
plot(gz);
grid;
title('Giro: eixo Z em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');


return;



