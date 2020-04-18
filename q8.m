clear all;

esc_ac=2;
esc_giro=250;

nome='q8data.txt';
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
total_leituras = total/6;

ax = zeros(1,total_leituras);
ay = ax;
az = ax;

gx = zeros(1,total_leituras);
gy = gx;
gz = gx;

for i=1:total_leituras
    ax(1,i) = dados( ((i-1)*6)+1,1 );
    ay(1,i) = dados( ((i-1)*6)+2,1 );
    az(1,i) = dados( ((i-1)*6)+3,1 );
    
    gx(1,i) = dados(((i-1)*6)+4 , 1 );
    gy(1,i) = dados(((i-1)*6)+5 , 1 );
    gz(1,i) = dados(((i-1)*6)+6 , 1 );
end

% Converter acelerações em "g"
ax=esc_ac*(ax/32767);
ay=esc_ac*(ay/32767);
az=esc_ac*(az/32767);

% Converter giros em "graus/seg"
gx=esc_giro*(gx/32767);
gy=esc_giro*(gy/32767);
gz=esc_giro*(gz/32767);

% Desenhar gráficos
%ax
figure(1);
plot(ax);
grid;
title('Aceleração: eixo X em g');
xlabel('Amostra');
ylabel('g');

%ay
figure(2);
plot(ay);
grid;
title('Aceleração: eixo Y em g');
xlabel('Amostra');
ylabel('g');

%az
figure(3);
plot(az);
grid;
title('Aceleração: eixo Z em g');
xlabel('Amostra');
ylabel('g');

%gx
figure(4);
plot(gx);
grid;
title('Giro: eixo X em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

%gy
figure(5);
plot(gy);
grid;
title('Giro: eixo Y em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

%gz
figure(6);
plot(gz);
grid;
title('Giro: eixo Z em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

return;



