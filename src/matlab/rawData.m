% Ler da porta serial
% Ainda precisa melhorar

clear all;
fclose(instrfind);  %Fechar possível porta serial
close all;

% Abre porta serial
sid=serial('COM3','Baudrate',115200);
fopen(sid);

if (sid==-1)
    fprintf(1,'Nao abriu COM3.\n');
    return
end
fprintf('Pronto para receber dados!\n');
fprintf('Por favor, selecione Teste 12 na Caixa Preta.\n');

% Aguarda o MPU inicializar (enviar -1 pela serial)
while true
    temp = fscanf(sid,'%s');
    if strcmp(temp,'start') == 1
        break;
    end
    fprintf('%s\n', temp);
end
fprintf('Iniciando leitura.\n');


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Usuário selecionar sua janela
mediamovel_k = 10;          % Janela da media movel
max_size=2000;              % Quantidade maxima de amostras exibidas na tela
freq_render=15;             % Frequencia de atualização do plot
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Parâmetros
esc_ac=2;                   % Vem do Arduino, função que configura escalas de aceleração
esc_giro=250;               % e giro //+/- 2g e +/-250gr/seg
cont=0;                     % Conta quantas amostras foram lidas

ax=zeros(1,max_size);
ay=zeros(1,max_size);
az=zeros(1,max_size);
tp=zeros(1,max_size);
gx=zeros(1,max_size);
gy=zeros(1,max_size);
gz=zeros(1,max_size);
t = 0:max_size;

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
while true
    
    % Lê até achar quebra de linha
    data=fscanf(sid,'%s');
    
    % Checa se não é o final da transmissão
    if ~isempty(strfind(data,'fim'))
        break;
    end

    % Quebra a string no marcador ';'
    data=strsplit(data,';');

    % Checa se realmente leu todos os dados
    if length(data) < 7
        continue
    end

    cont=cont+1;
    
    data = str2double(data);
    
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
    
    % Re plota
    if (now-time)*100000 > 1/freq_render
        refreshdata
        drawnow
        time = now;
    end
end

%Aqui acaba o script
fprintf(1,'Recebidos %d amostras\n\n',cont);
fclose(sid);
return;

