% Ler da porta serial
% Ainda precisa melhorar
% BUGS:
% - file_simulated_freq ainda não funciona muito bem

clear all;
%fclose(instrfind);  %Fechar possível porta serial
close all;

% layout Macros
Vazio = '';
Acel = 'Acel';
Vel = 'Vel';
Space = 'Space';
Gvel = 'Gvel';
Gdeg = 'Gdeg';
Gtilt = 'Gtilt';
Mag = 'Mag';
FusionTilt = 'FusionTilt';
CompTilt = 'CompTilt';
KalmanTilt = 'KalmanTilt';
MadgwickTilt = 'MadgwickTilt';
Space3D = 'Space3D';
Tilt3D = 'Tilt3D';



%%%%%%%%%%%%%%%%%%%%%% PARAMETROS DE USUÁRIO %%%%%%%%%%%%%%%%%%%%%%%%%
% Fonte de leitura
read_from_serial=false;      % Set to false to use a file
serial_COM='COM3';          
serial_baudrate=115200;     
file_full_path='Dados/roll-pitch-yaw-45-seq.txt';          
file_simulated_freq=Inf;    % Simulate a pulling frequence, e.g.: to simulate an freq of 100 samples per second use 100hz

% Amostragem
max_size=4000;              % Quantidade maxima de amostras exibidas na tela
freq_sample=100;            % Frequencia da amostragem dos dados

% Media movel parametros 
window_k = 10;              % Janela da media movel (minimo = 2)

% Plotagem
freq_render=10;             % Frequencia de atualização do plot
layout= {...                % Layout dos plots, as visualizações possíveis estão variaveis no inicio do arquivo

    Gvel,       Gdeg,       Acel,       Acel,;...
    Gtilt,      Gtilt,      CompTilt,   CompTilt;...
    FusionTilt, FusionTilt, KalmanTilt, KalmanTilt;...

};                          % OBS: Repita o nome no layout p/ expandir o plot em varios grids

% Constantes do sensor
const_g=9.8;                % Constante gravitacional segundo fabricante do MPU
gx_bias=0;%0.80;               % 
gy_bias=0;%0.77;               % 
gz_bias=0;%0.45;               % 
ax_bias=0;%0.0325;             % 
az_bias=0;%0.035;              % 
ay_bias=0;%0.07;               % 
esc_ac=2;                   % Vem do Arduino, função que configura escalas de aceleração
esc_giro=250;               % e giro //+/- 2g e +/-250gr/seg


% Variável de ajuste do filtro complementar
mu=0.02;

% Variável de ajuste do filtro de kalman, os valores iniciais de X e P são por padrão 0s
% Nosso modelo contem:
% - 1 entrada (uk = delta Giro/s)
% - 2 estados (x1 = Tilt usando Giro e  x2 = Drift)
% - 1 saida (yk = Tilt do acelerometro)
% portanto nosso modelo fica:
%
% x[k] = A*x[k-1] + B*u[k] + w[k]
% X1 = (x1 + x2 * deltaT) + (deltaT * uk) + ruido
% X2 = (x2) + ruido
%
% y[k] = C*x[k] + v[k]
% Y = X1 + ruido

deltaT = 1/freq_sample;
A = [1 deltaT; 0 1];
B = [deltaT; 0];
C = [1 0];
Q = [0.002^2 0; 0 0];
R = 0.03;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% Variáveis de dados
vazios=zeros(1,max_size); 
cont=0;                                 % Conta quantas amostras foram lidas

ax=vazios;                              % Aceleração eixo X
ay=ax;                                  % Aceleração eixo Y
az=ax;                                  % Aceleração eixo Z
tp=ax;                                  % Temperatura do MPU
gx=ax;                                  % Giro/s eixo X
gy=ax;                                  % Giro/s eixo Y
gz=ax;                                  % Giro/s eixo Z
gPitch=ax;                              % Movimento de Pitch usando giro/s (relativo a ultima rotação do corpo)
gRoll=ax;                               % Movimento de Roll usando giro/s (relativo a ultima rotação do corpo)
gYaw=ax;                                % Movimento de Yaw usando giro/s (relativo a ultima rotação do corpo)
gPitch_abs=ax;                          % Movimento de Pitch usando giro/s e matriz de rotação (em relação a posição inicial do corpo)
gRoll_abs=ax;                           % Movimento de Roll usando giro/s e matriz de rotação (em relação a posição inicial do corpo)
gYaw_abs=ax;                            % Movimento de Yaw usando giro/s e matriz de rotação (em relação a posição inicial do corpo)
Rot=[[1,0,0];[0,1,0];[0,0,1]];          % Matriz de rotação que move o corpo da posição inicial p/ a atual
aPitch=ax;                              % Movimento de Roll usando Aceleração
aRoll=ax;                               % Movimento de Yaw usando Aceleração
vx=ax;                                  % Velocidade no eixo X
vy=ax;                                  % Velocidade no eixo Y
vz=ax;                                  % Velocidade no eixo Z
px=ax;                                  % Posição no eixo X
py=ax;                                  % Posição no eixo Y
pz=ax;                                  % Posição no eixo Z
ax_without_gravity=ax;                  % Aceleração eixo X removido a gravidade
ay_without_gravity=ax;                  % Aceleração eixo Y removido a gravidade
az_without_gravity=ax;                  % Aceleração eixo Z removido a gravidade
r_data=zeros(window_k,7);               % Dados não modificados (sem filtro ou media)
compl_Roll=ax;                          % Movimento de Roll usando filtro complementar
compl_Pitch=ax;                         % Movimento de Pitch usando filtro complementar
compl_Yaw=ax;                           % Movimento de Yaw usando filtro complementar
kalman_Roll=ax;                         % Movimento de Roll usando filtro de kalman
kalman_Pitch=ax;                        % Movimento de Pitch usando filtro de kalman
kalman_Yaw=ax;                          % Movimento de Yaw usando filtro de kalman
t = 0:max_size;                         % Eixo temporal do gráfico

% Define uma janela p/ plot
plot_1 = render(freq_render, layout);

% Seta as labels de cada gráfico
plot_1.setProperties(Acel, 'Aceleração em g', 'Amostra', 'g', {'aX', 'aY', 'aZ'});
plot_1.setProperties(Vel, 'Velocidade em m/s', 'Amostra', 'm/s', {'vX', 'vY', 'vZ'});
plot_1.setProperties(Space, 'Espaço em metros', 'Amostra', 'metros', {'X', 'Y', 'Z'});
plot_1.setProperties(Gvel, 'Giro em graus/seg', 'Amostra', 'graus/seg', {'gX', 'gY', 'gZ'});
plot_1.setProperties(Gdeg, 'Giro em graus(relativo)', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'});
plot_1.setProperties(Gtilt, 'Giro em graus(absoluto)', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'});
plot_1.setProperties(FusionTilt, 'Giro em graus(absoluto) usando acel + mag', 'Amostra', 'graus', {'aRoll', 'aPitch', 'mYaw'});
plot_1.setProperties(CompTilt, 'Filtro complementar', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'});
plot_1.setProperties(KalmanTilt, 'Filtro de Kalman', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'});

% Seta a fonte de dados de cada gráfico
plot_1.setSource(Acel, 'ax', 'ay', 'az');
plot_1.setSource(Gvel, 'gx', 'gy', 'gz');
plot_1.setSource(Gdeg, 'gRoll', 'gPitch', 'gYaw');
plot_1.setSource(Gtilt, 'gRoll_abs', 'gPitch_abs', 'gYaw_abs');
plot_1.setSource(FusionTilt, 'aRoll', 'aPitch', 'vazios');
plot_1.setSource(CompTilt, 'compl_Roll', 'compl_Pitch', 'compl_Yaw');
plot_1.setSource(KalmanTilt, 'kalman_Roll', 'kalman_Pitch', 'kalman_Yaw');

% Inicializa os filtrso de kalman, um para cada eixo,
% podemos fazer tudo com um filtro só, entretanto os parâmetros ficariam
% bem grandes e de dificil visualização
kalmanFilterRoll = kalman(A,B,C,Q,R);
kalmanFilterPitch = kalman(A,B,C,Q,R);
kalmanFilterYaw = kalman(A,B,C,Q,R);

% Abre porta serial
if read_from_serial
    reader_1 = reader();
    reader_1.set_serial_reader(serial_COM, serial_baudrate);
else
    reader_1 = reader();
    reader_1.set_file_reader(file_full_path, file_simulated_freq);
end

% Obtem os dados e plota em tempo real
% NOTA: Se o buffer do serial encher (> 512 bytes) o programa explode, caso isso ocorra
% abaixe a taxa de renderização do grafico
while true
    
    % Lê uma amostra de cada da porta serial/arquivo
    data = reader_1.read_sample();

    % Se é o fim do arquivo ou deu algum erro finaliza
    if isempty(data)
        break;
    end

    cont=cont+1;
    
    data = str2double(data);
    
    % Converter acelerações em "g"
    data(1)=esc_ac*(data(1)/32767) - ax_bias;
    data(2)=esc_ac*(data(2)/32767) - ay_bias;
    data(3)=esc_ac*(data(3)/32767) - az_bias;

    % Converter giros em "graus/seg"
    data(5)=esc_giro*(data(5)/32767) - gx_bias;
    data(6)=esc_giro*(data(6)/32767) - gy_bias;
    data(7)=esc_giro*(data(7)/32767) - gz_bias;

    % Converter temperatura para Celsius
    data(4)=(data(4)/340)+36.53;

    % Salva os dados sem alteração p/ poder aplicar a media movel mais a frente
    r_data = [r_data(2:window_k,:) ; data(1:7)];

    % Faz a media movel dos dados lidos
    data = sum(r_data) / window_k;
    
    % Salva valores após aplicar a media movel
    ax = [ax(2:max_size) data(1)];
    ay = [ay(2:max_size) data(2)];
    az = [az(2:max_size) data(3)];
    tp = [tp(2:max_size) data(4)];
    gx = [gx(2:max_size) data(5)];
    gy = [gy(2:max_size) data(6)];
    gz = [gz(2:max_size) data(7)];

    % Calcula Yaw, Pitch e Roll realtivos(em relação a ultima rotação do corpo) p/ a nova amostra usando giro
    % Usando o giro, fazemos a integral discreta (área do trapézio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trapézio (entre ultimo dado e o novo)
    % É considerado nesse calculo que, as amostragens estão espaçadas de 1
    % periodo da amostragem, ent o trapézio é igual a 1/freq * ((n-1 + n)/2)
    newPitch = (gPitch(max_size) + ((gy(max_size-1) + gy(max_size)) / (2 * freq_sample) ));
    newRoll = (gRoll(max_size) + ((gx(max_size-1) + gx(max_size)) / (2 * freq_sample) ));
    newYaw = (gYaw(max_size) + ((gz(max_size-1) + gz(max_size)) / (2 * freq_sample) ));
    
    gPitch  = [gPitch(2:max_size) newPitch];
    gRoll   = [gRoll(2:max_size)  newRoll];
    gYaw    = [gYaw(2:max_size)   newYaw];
    
    % Calcula a matriz de rotação (X,Y,Z) que foi responsável por mover o corpo da posição da amostra anterior para a atual
    delta_gPitch = gPitch(max_size) - gPitch(max_size-1);
    delta_gRoll = gRoll(max_size) - gRoll(max_size-1);
    delta_gYaw = gYaw(max_size) - gYaw(max_size-1);
    temp_Rx = [...
        [1,                0,               0        ];...
        [0,     cosd(delta_gRoll),  -sind(delta_gRoll)];...
        [0,     sind(delta_gRoll),  cosd(delta_gRoll)]
    ];
    temp_Ry = [...
        [cosd(delta_gPitch),        0,       sind(delta_gPitch)];...
        [0,                         1,               0        ];...
        [-sind(delta_gPitch),        0,      cosd(delta_gPitch)]
    ];
    temp_Rz = [...
        [cosd(delta_gYaw),         -sind(delta_gYaw),       0];...
        [sind(delta_gYaw),         cosd(delta_gYaw),       0];...
        [0,                         0,                      1]
    ];
    temp_R = temp_Rx * temp_Ry * temp_Rz;

    % Calcula a matriz de rotação (X,Y,Z) que move o corpo da posição inicial para a posição atual
    Rot = Rot * temp_R;
    

    % Extrai Yaw, Pitch e Roll absolutos(em relação a posição inicial do corpo) p/ a nova amostra usando a atual matriz de rotação (X,Y,Z)
    newRoll = atan2(Rot(3,2), Rot(3,3)) * 180/pi;
    newYaw = atan2(Rot(2,1), Rot(1,1)) * 180/pi;
    if cosd(newYaw) == 0
        newPitch = atan2(-Rot(3,1), Rot(2,1)/sind(newYaw)) * 180/pi;
    else
        newPitch = atan2(-Rot(3,1), Rot(1,1)/cosd(newYaw)) * 180/pi;
    end

    gPitch_abs = [gPitch_abs(2:max_size) newPitch];
    gRoll_abs = [gRoll_abs(2:max_size) newRoll];
    gYaw_abs = [gYaw_abs(2:max_size) newYaw];
    
    % Calcula Yaw, Pitch e Roll absolutos(em relação a posição inicial do corpo) p/ a nova amostra usando aceleração
    % Usando a aceleração, usamos o vetor de gravidade que deve sempre
    % estar presente p/ determinar a posição do corpo
    % assim temos que p/ a order de rotação X,Y,Z
    newPitch = atan2(-ax(max_size), sqrt( ay(max_size)^2 + az(max_size)^2 )) * 180/pi;
    if (az(max_size)>=0)
        sign = 1;
    else
        sign = -1;
    end
    miu = 0.001;
    newRoll = atan2( ay(max_size),   (sign * sqrt( az(max_size)^2 + miu * ax(max_size)^2 ))) * 180/pi;
    
    aPitch  = [aPitch(2:max_size) newPitch];
    aRoll   = [aRoll(2:max_size)   newRoll];

    % Calcula rotação usando filtro complementar
    newRoll = (1-mu)*(compl_Roll(max_size) + delta_gRoll) + mu*aRoll(max_size);
    newPitch = (1-mu)*(compl_Pitch(max_size) + delta_gPitch) + mu*aPitch(max_size);
    % newYaw = 

    compl_Roll = [compl_Roll(2:max_size) newRoll];
    compl_Pitch = [compl_Pitch(2:max_size) newPitch];
    % compl_Yaw = [compl_Yaw(2:max_size) newYaw]

    % Calcula rotação usando filtro de Kalman
    % Calcula a predição p/ cada eixo individualmente
    kalmanFilterRoll.predict(gx(max_size));
    kalmanFilterPitch.predict(gy(max_size));
    
    % Atualiza a predição p/ cada eixo individualmente
    roll_and_bias = kalmanFilterRoll.update(aRoll(max_size));
    pitch_and_bias = kalmanFilterPitch.update(aPitch(max_size));

    % Insere o valor filtrado no eixo a ser plotado
    kalman_Roll = [kalman_Roll(2:max_size)  roll_and_bias(1)];
    kalman_Pitch = [kalman_Pitch(2:max_size)  pitch_and_bias(1)];
    
    % Calcula a aceleração removendo a gravidade
    % Obtem o vetor gravidade atual rotacionando em sentido contrário realizado pelo corpo, considerando gravidade = 1g
    gravity_vector =   [-sind(gPitch(max_size)),...
                         cosd(gPitch(max_size))*sind(gRoll(max_size)),...
                         cosd(gPitch(max_size))*cosd(gRoll(max_size))
                       ];
    ax_without_gravity = [ax_without_gravity(2:max_size) (ax(max_size) - gravity_vector(1))];
    ay_without_gravity = [ay_without_gravity(2:max_size) (ay(max_size) - gravity_vector(2))];
    az_without_gravity = [az_without_gravity(2:max_size) (az(max_size) - gravity_vector(3))];
    
    %ax = ax_without_gravity;
    %ay = ay_without_gravity;
    %az = az_without_gravity;

    % Tenta redesenhar o plot, se deu o tempo da frequencia
    plot_1.try_render()
end

%Aqui acaba o script
fprintf(1,'Recebidos %d amostras\n\n',cont);
reader_1.delete();
return;