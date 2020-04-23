% Ler da porta serial
% Ainda precisa melhorar

fclose all;
close all;

%Vem do Arduino, função que configura escalas
%mpu_escalas(0,0);     //+/- 2g e +/-250gr/seg
esc_ac=2;
esc_giro=250;

% Abre porta serial
sid=serial('COM3','Baudrate',115200);
fopen(sid);

if (sid==-1)
    fprintf(1,'Nao abriu COM3.\n');
    return
end
fprintf(1,'Pronto para receber dados!\n');

% Aguarda o MPU inicializar (enviar -1 pela serial)
while true
    temp = fscanf(sid,'%s');
    if strcmp(temp,'start') == 1
        break;
    end
end
fprintf('Iniciando leitura.\n')


mediamovel_k = 10;          % Janela da media movel
cont=0;                     % conta quandos dados foram lidos
fim=0;                      % condição de parada
max_size=2000;              % quantidade maxima de amostras exibidas na tela
ax=zeros(1,max_size);
ay=zeros(1,max_size);
az=zeros(1,max_size);
tp=zeros(1,max_size);
gx=zeros(1,max_size);
gy=zeros(1,max_size);
gz=zeros(1,max_size);


% Define a janela p/ plot
f1 = figure('units','normalized','outerposition',[0 0 1 1]);

% Desenhar gráficos
%Plota aceleração
figure(f1);
subplot(3,3,1);
p_ax = plot(ax, 'r');
grid;
title('Aceleração em g');
xlabel('Amostra');
ylabel('g');

hold on
p_ay = plot(ay, 'g');
p_az = plot(az, 'b');
legend('aX', 'aY', 'aZ');
hold off

%Plota giro/s
subplot(3,3,4);
p_gx = plot(gx, 'r');
grid;
title('Giro em graus/seg');
xlabel('Amostra');
ylabel('graus/seg');

hold on
p_gy = plot(gy, 'g');
p_gz = plot(gz, 'b');
legend('gX', 'gY', 'gZ');
hold off

p_ax.YDataSource = 'ax';
p_ay.YDataSource = 'ay';
p_az.YDataSource = 'az';
p_gx.YDataSource = 'gx';
p_gy.YDataSource = 'gy';
p_gz.YDataSource = 'gz';

% Obtem os dados e plota em tempo real (taxa de atualizaçao 10hz)
% NOTA: Se o buffer do serial encher (> 512 bytes) o programa explode, caso isso ocorra
% abaixe o tempo de renderização do grafico
time = now;
while fim==0
    
    i=1;
    data=zeros(1,7);   % array de dados lidos (7 por iteração)
    while i<=7
        z=fscanf(sid,'%s', 10);

        if strcmp(z,'fim') == 1
            fim=1;
            break;
        else
            z = str2num(z);
        end

        data(i)=z;
        cont=cont+1;
        i=i+1;
    end
    
    % Converter acelerações em "g"
    data(1)=esc_ac*(data(1)/32767);
    data(2)=esc_ac*(data(2)/32767);
    data(3)=esc_ac*(data(3)/32767);

    % Converter giros em "graus/seg"
    data(5)=esc_giro*(data(5)/32767);
    data(6)=esc_giro*(data(6)/32767);
    data(7)=esc_giro*(data(7)/32767);

    % Converter temperatura para Celsius
    data(4)=(data(4)/340)+36.53;
    
    % Salva valores lidos
    ax = [ax(2:max_size) data(1)];
    ay = [ay(2:max_size) data(2)];
    az = [az(2:max_size) data(3)];
    tp = [tp(2:max_size) data(4)];
    gx = [gx(2:max_size) data(5)];
    gy = [gy(2:max_size) data(6)];
    gz = [gz(2:max_size) data(7)];
    
    if (now-time)*100000 > 0.1
        refreshdata
        drawnow
        time = now;
    end
end

%Aqui acaba o script
fprintf(1,'Recebidos %d dados\nLeituras = %d.\n',cont,cont/7);
fclose(sid);
return;

