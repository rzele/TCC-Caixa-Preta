addpath('D:\tcc\tcc github\TCC-Caixa-Preta\src\matlab\quaternion_library');      % inclui biblioteca de quaternions
close all;                          
clear;                              
clc;                                
esc_ac=2;
esc_giro=250;


nome='mpudata.txt';
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

% Converter aceleracoes em "g"
ax=esc_ac*(ax/32767);
ay=esc_ac*(ay/32767);
az=esc_ac*(az/32767);

% Converter giros em "graus/seg"
gx=esc_giro*(gx/32767);
gy=esc_giro*(gy/32767);
gz=esc_giro*(gz/32767);


%calcular as medias moveis
mm = 4;

axm = mediamovel(ax,mm);
aym = mediamovel(ay,mm);
azm = mediamovel(az,mm);

gxm = mediamovel(gx,mm);
gym = mediamovel(gy,mm);
gzm = mediamovel(gz,mm);

%calcular as integrais
intervalo = 0.01; %10ms
eixoX = 0:length(ax)-1;
eixoX = eixoX * intervalo;

axi = cumsum(ax)*intervalo;
ayi = cumsum(ay)*intervalo;
azi = cumsum(az)*intervalo;

gxi = cumsum(gx)*intervalo;
gyi = cumsum(gy)*intervalo;
gzi = cumsum(gz)*intervalo;


%integral com as medias moveis
axim = cumsum(axm)*intervalo;
ayim = cumsum(aym)*intervalo;
azim = cumsum(azm)*intervalo;

gxim = cumsum(gxm)*intervalo;
gyim = cumsum(gym)*intervalo;
gzim = cumsum(gzm)*intervalo;


%filtro de madgwick (imu)
AHRS = MadgwickAHRS('SamplePeriod', 0.01, 'Beta', 0.1);

quaternion = zeros(length(eixoX), 4);
giro = [gx' gy' gz'];
acel = [ax' ay' az'];

for t = 1:length(eixoX)
    AHRS.UpdateIMU(giro(t,:) * (pi/180), acel(t,:));	% gyroscope units must be radians
    quaternion(t, :) = AHRS.Quaternion;
end






% Desenhar graficos

%sem filtro

%ax
% figure(1);
% plot(ax);
% grid;
% title('Aceleração: eixo X em g');
% xlabel('Amostra');
% ylabel('g');
% %ay
% figure(2);
% plot(ay);
% grid;
% title('Aceleração: eixo Y em g');
% xlabel('Amostra');
% ylabel('g');
% %az
% figure(3);
% plot(az);
% grid;
% title('Aceleração: eixo Z em g');
% xlabel('Amostra');
% ylabel('g');

%gx
figure(4);
plot(gx);
grid;
title('Giro: eixo X em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');
% %gy
% figure(5);
% plot(gy);
% grid;
% title('Giro: eixo Y em graus/seg');
% xlabel('Amostra');
% ylabel('graus/seg');
% %gz
% figure(6);
% plot(gz);
% grid;
% title('Giro: eixo Z em graus/seg');
% xlabel('Amostra');
% ylabel('graus/seg');


% Com filtro de media movel

%axm
% figure(7);
% plot(axm);
% grid;
% title('Aceleração Média movel: eixo X em g');
% xlabel('Amostra');
% ylabel('g');
% %aym
% figure(8);
% plot(aym);
% grid;
% title('Aceleração Média móvel: eixo Y em g');
% xlabel('Amostra');
% ylabel('g');
% %azm
% figure(9);
% plot(azm);
% grid;
% title('Aceleração Média móvel: eixo Z em g');
% xlabel('Amostra');
% ylabel('g');

%gxm
figure(10);
plot(gxm);
grid;
title('Giro: eixo X Média móvel em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');
% %gym
% figure(11);
% plot(gym);
% grid;
% title('Giro: eixo Y Média móvel em graus/seg');
% xlabel('Amostra');
% ylabel('graus/seg');
% %gzm
% figure(12);
% plot(gzm);
% grid;
% title('Giro: eixo Z Média móvel em graus/seg');
% xlabel('Amostra');
% ylabel('graus/seg');


%integrais



%axi
% figure(13);
% plot(axi);
% grid;
% title('Velocidade: eixo X em g/s');
% xlabel('Amostra');
% ylabel('g/s');
% %ayi
% figure(14);
% plot(ayi);
% grid;
% title('Velocidade: eixo Y em g/s');
% xlabel('Amostra');
% ylabel('g');
% %az
% figure(15);
% plot(azi);
% grid;
% title('Velocidade: eixo Z em g/s');
% xlabel('Amostra');
% ylabel('g');

%gxi
figure(16);
plot(eixoX,gxi);
grid;
title('Giro: eixo X em graus');
xlabel('segundos');
ylabel('graus');
% %gy
% figure(5);
% plot(gy);
% grid;
% title('Giro: eixo Y em graus/seg');
% xlabel('Amostra');
% ylabel('graus/seg');
% %gz
% figure(6);
% plot(gz);
% grid;
% title('Giro: eixo Z em graus/seg');
% xlabel('Amostra');
% ylabel('graus/seg');


%integral com a média móvel
%gxi
figure(17);
plot(eixoX,gxim);
grid;
title('Giro: eixo X Media movel em graus');
xlabel('segundos');
ylabel('graus');

%madgwick
euler = quatern2euler(quaternConj(quaternion)) * (180/pi);	% use conjugate for sensor frame relative to Earth and convert to degrees.

figure('Name', 'Angulo de euler');
hold on;
plot(eixoX, euler(:,1), 'r');
grid;
%plot(eixoX, euler(:,2), 'g');
%plot(eixoX, euler(:,3), 'b');
title('Rotação - Filtro de Madgwick');
xlabel('Tempo (s)');
ylabel('Angulos (graus)');
%legend('\phi', '\theta', '\psi');
legend('\phi');
hold off;

return;