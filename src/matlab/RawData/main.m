% Ler da porta serial
% Ainda precisa melhorar
% BUGS:
% - file_simulated_freq ainda não funciona muito bem

addpath('quaternion_library');      % include quaternion library
addpath('render');                  % include plot library
addpath('reader');                  % include reader library
addpath('helpers');                 % include some useful functions

close all;                          % close all figures
clear;                              % clear all variables
clc;                                % clear the command terminal

% layout Macros
Vazio = '';                         % Deixa a celula vazia
Acel = 'A';                         % Aceleração X,Y,Z
Vel = 'B';                          % Velocidade X,Y,Z
Space = 'C';                        % Espaço percorrido X,Y,Z
Gvel = 'D';                         % Velocidade angular em X,Y,Z
Gdeg = 'E';                         % Posição angular em Z,Y,X relativo (em relação a Posição anterior)
Gtilt = 'F';                        % Posição angular em Z,Y,X absoluto (em relação aos eixos iniciais)
Mag = 'G';                          % Magnetometro
AcelMagTilt = 'H';                  % Posição angular em Z,Y,X absoluto, usando Aceleração e magnetometro
CompTilt = 'I';                     % Posição angular usando o filtro complementar
KalmanTilt = 'J';                   % Posição angular usando o filtro de kalman
MadgwickTilt = 'K';                 % Posição angular usando o filtro de madgwick
Quat = 'L';                         % Plot dos valores de quaternions obtidos pno filtro de madgwick
Acel_G = 'M';                       % Aceleração desconsiderando a gravidade (utilizando o melhor filtro p/ remove-la)
Quat = 'N';                         % Plot com os valore de quaternion extraidos do filtro de madgwick
Car3DGdeg = 'O';                    % Posição angular atual usando um objeto 3D rotacionando utilizando matriz de Rotação (Gdeg)
Car3DGtilt = 'P';                   % Posição angular atual usando um objeto 3D rotacionando utilizando matriz de Rotação (Gtilt)
Car3DAcelMag = 'Q';                 % Posição angular atual usando um objeto 3D rotacionando utilizando matriz de Rotação (AcelMagTilt)
Car3DComp = 'R';                    % Posição angular atual usando um objeto 3D rotacionando utilizando matriz de Rotação (CompTilt)
Car3DKalman = 'S';                  % Posição angular atual usando um objeto 3D rotacionando utilizando matriz de Rotação (KalmanTilt)
Car3DMadgwick = 'T';                % Posição angular atual usando um objeto 3D rotacionando utilizando quaternios advindos do filtro de Madgwick
Compass = 'V';                      % Angulo de yaw estraido do magnetometro sem compensação de tilt plotado em plano polar
CompassCompensated = 'W';           % Angulo de yaw estraido do magnetometro com compensação de tilt, usando dados do MPU, plotado em plano polar
Space3D = 'X';                      % Posição em um Espaço 3D
                                    % pois é necessário saber bem a Posição angular p/ isso)

%% PARAMETROS DE USUÁRIO %%
% Fonte de leitura
read_from_serial=false;     % Set to false to use a file
serial_COM='COM4';
serial_baudrate=115200;
file_full_path='Dados/teste1.txt';

% Amostragem
max_size=4000;              % Quantidade maxima de amostras exibidas na tela
freq_sample=100;            % Frequencia da amostragem dos dados

% Plotagem
plot_in_real_time=true;     % Define se o plot será so no final, ou em tempo real
freq_render=5;              % Frequencia de atualização do plot
layout= {...                % Layout dos plots, as visualizações possíveis estão variaveis no inicio do arquivo

    Acel, Vel, CompTilt, Acel_G;...
    Space,  Gvel, KalmanTilt, Vazio;...
    Gdeg,  Gtilt, MadgwickTilt, Vazio;...
    Mag,  AcelMagTilt, Quat, Vazio;...

};                          % OBS: Repita o nome no layout p/ expandir o plot em varios grids

% Constantes do sensor
const_g=9.8;                % Constante gravitacional segundo fabricante do MPU
gx_bias=-1.05;              % 
gy_bias=0.2;                % 
gz_bias=-0.52;              % 
ax_bias=0;                  % 
ay_bias=0;                  % 
az_bias=0.04;               % 
hx_offset=-70;              % 
hy_offset=228;              % 
hz_offset=10;               % 
hx_scale=1.020833;          % 
hy_scale=0.940048;          % 
hz_scale=1.045333;          % 


% Media movel parametros 
window_k = 10;              % Janela da media movel (minimo = 2)

% Variável de ajuste do filtro complementar
mu=0.02;

% Variável de ajuste do filtro de kalman, os valores iniciais de X e P são por padrão 0s
% Nosso modelo countem:
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

% Variável de ajuste do filtro madgwick
beta=0.1;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% Variáveis de dados
vazios=zeros(1,max_size); 
count=0;                                 % conta quantas amostras foram lidas

ax=vazios;                              % Aceleração eixo X
ay=ax;                                  % Aceleração eixo Y
az=ax;                                  % Aceleração eixo Z
gx=ax;                                  % Giro/s eixo X
gy=ax;                                  % Giro/s eixo Y
gz=ax;                                  % Giro/s eixo Z
hx=ax;                                  % Leitura do magnetometro X
hy=ax;                                  % Leitura do magnetometro Y
hz=ax;                                  % Leitura do magnetometro Z
gPitch=ax;                              % Movimento de Pitch usando giro/s (relativo a ultima Rotação do corpo)
gRoll=ax;                               % Movimento de Roll usando giro/s (relativo a ultima Rotação do corpo)
gYaw=ax;                                % Movimento de Yaw usando giro/s (relativo a ultima Rotação do corpo)
gPitch_abs=ax;                          % Movimento de Pitch usando giro/s e matriz de Rotação (em relação a Posição inicial do corpo)
gRoll_abs=ax;                           % Movimento de Roll usando giro/s e matriz de Rotação (em relação a Posição inicial do corpo)
gYaw_abs=ax;                            % Movimento de Yaw usando giro/s e matriz de Rotação (em relação a Posição inicial do corpo)
Rot=[[1,0,0];[0,1,0];[0,0,1]];          % Matriz de Rotação que move o corpo da Posição inicial p/ a atual
aPitch=ax;                              % Movimento de Pitch usando Aceleração
aRoll=ax;                               % Movimento de Roll usando Aceleração
mYaw=ax;                                % Movimento de Yaw usando Magnetômetro
vx=ax;                                  % Velocidade no eixo X
vy=ax;                                  % Velocidade no eixo Y
vz=ax;                                  % Velocidade no eixo Z
px=ax;                                  % Posição no eixo X
py=ax;                                  % Posição no eixo Y
pz=ax;                                  % Posição no eixo Z
ax_without_gravity=ax;                  % Aceleração eixo X removido a gravidade
ay_without_gravity=ax;                  % Aceleração eixo Y removido a gravidade
az_without_gravity=ax;                  % Aceleração eixo Z removido a gravidade
compl_Roll=ax;                          % Movimento de Roll usando filtro complementar
compl_Pitch=ax;                         % Movimento de Pitch usando filtro complementar
compl_Yaw=ax;                           % Movimento de Yaw usando filtro complementar
kalman_Roll=ax;                         % Movimento de Roll usando filtro de kalman
kalman_Pitch=ax;                        % Movimento de Pitch usando filtro de kalman
kalman_Yaw=ax;                          % Movimento de Yaw usando filtro de kalman
madgwick_Roll=ax;                       % Movimento de Roll usando filtro de madgwick
madgwick_Pitch=ax;                      % Movimento de Pitch usando filtro de madgwick
madgwick_Yaw=ax;                        % Movimento de Yaw usando filtro de madgwick
q1=ax;                                  % Quaternio 1 do filtro de madgwick
q2=ax;                                  % Quaternio 2 do filtro de madgwick
q3=ax;                                  % Quaternio 3 do filtro de madgwick
q4=ax;                                  % Quaternio 4 do filtro de madgwick

%% Define uma janela p/ plot
plot_1 = Render(freq_render, layout);


% Obtem a lista de itens unicos definidos no layout
% para evitar calculo de itens descenessários
setted_objects_name = plot_1.setted_objects_name;

% Seta os tipos de cada gráfico
plotAcel = plot_1.setItemType(Acel, 'plotline');
plotVel = plot_1.setItemType(Vel, 'plotline');
plotSpace = plot_1.setItemType(Space, 'plotline');
plotGvel = plot_1.setItemType(Gvel, 'plotline');
plotGdeg = plot_1.setItemType(Gdeg, 'plotline');
plotGtilt = plot_1.setItemType(Gtilt, 'plotline');
plotMag = plot_1.setItemType(Mag, 'plotline');
plotAcelMagTilt = plot_1.setItemType(AcelMagTilt, 'plotline');
plotCompTilt = plot_1.setItemType(CompTilt, 'plotline');
plotKalmanTilt = plot_1.setItemType(KalmanTilt, 'plotline');
plotMadgwickTilt = plot_1.setItemType(MadgwickTilt, 'plotline');
plotQuat = plot_1.setItemType(Quat, 'plotline');
plotAcel_G = plot_1.setItemType(Acel_G, 'plotline');
plotCar3DGdeg = plot_1.setItemType(Car3DGdeg, 'plot3dcar');
plotCar3DMadgwick = plot_1.setItemType(Car3DMadgwick, 'plot3dcar');
plotCar3DGtilt = plot_1.setItemType(Car3DGtilt, 'plot3dcar');
plotCar3DAcelMag = plot_1.setItemType(Car3DAcelMag, 'plot3dcar');
plotCar3DKalman = plot_1.setItemType(Car3DKalman, 'plot3dcar');
plotCar3DComp = plot_1.setItemType(Car3DComp, 'plot3dcar');
plotSpace3D = plot_1.setItemType(Space3D, 'plot3dline');
plotCompass = plot_1.setItemType(Compass, 'plotcompass');
plotCompassCompensated = plot_1.setItemType(CompassCompensated, 'plotcompass');

% Seta a fonte de dados de cada gráfico
plotAcel.setSource({'ax', 'ay', 'az'}, {'r', 'g', 'b'});
plotVel.setSource({'vx', 'vy', 'vz'}, {'r', 'g', 'b'});
plotSpace.setSource({'px', 'py', 'pz'}, {'r', 'g', 'b'});
plotGvel.setSource({'gx', 'gy', 'gz'}, {'r', 'g', 'b'});
plotGdeg.setSource({'gRoll', 'gPitch', 'gYaw'}, {'r', 'g', 'b'});
plotGtilt.setSource({'gRoll_abs', 'gPitch_abs', 'gYaw_abs'}, {'r', 'g', 'b'});
plotMag.setSource({'hx', 'hy', 'hz'}, {'r', 'g', 'b'});
plotAcelMagTilt.setSource({'aRoll', 'aPitch', 'mYaw'}, {'r', 'g', 'b'});
plotCompTilt.setSource({'compl_Roll', 'compl_Pitch', 'compl_Yaw'}, {'r', 'g', 'b'});
plotKalmanTilt.setSource({'kalman_Roll', 'kalman_Pitch', 'kalman_Yaw'}, {'r', 'g', 'b'});
plotMadgwickTilt.setSource({'madgwick_Roll', 'madgwick_Pitch', 'madgwick_Yaw'}, {'r', 'g', 'b'});
plotQuat.setSource({'q1', 'q2', 'q3', 'q4'}, {'r', 'g', 'b', 'y'});
plotAcel_G.setSource({'ax_without_gravity', 'ay_without_gravity', 'az_without_gravity'}, {'r', 'g', 'b'});
plotCar3DGdeg.setCar();
plotCar3DMadgwick.setCar();
plotCar3DGtilt.setCar();
plotCar3DAcelMag.setCar();
plotCar3DKalman.setCar();
plotCar3DComp.setCar();
plotSpace3D.setSource({'px', 'py', 'pz'});
plotCompass.setCompass();
plotCompassCompensated.setCompass();

% Seta as labels de cada gráfico
plotAcel.setProperties('Aceleração em "g"', 'Amostra', 'g', {'vX', 'vY', 'vZ'});
plotVel.setProperties('Velocidade em m/s', 'Amostra', 'm/s', {'vX', 'vY', 'vZ'});
plotSpace.setProperties('Espaço em metros', 'Amostra', 'metros', {'X', 'Y', 'Z'});
plotGvel.setProperties('Giro em graus/seg', 'Amostra', 'graus/seg', {'gX', 'gY', 'gZ'});
plotGdeg.setProperties('Giro em graus(relativo)', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'});
plotGtilt.setProperties('Giro em graus(absoluto)', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'});
plotMag.setProperties('Magnetrometro em mili Gaus', 'Amostra', 'mG', {'hx', 'hy', 'hz'});
plotAcelMagTilt.setProperties('Giro em graus(absoluto) usando acel + mag', 'Amostra', 'graus', {'aRoll', 'aPitch', 'mYaw'});
plotCompTilt.setProperties('Filtro complementar', 'Amostra', 'graus', {'Roll', 'Pitch'});
plotKalmanTilt.setProperties('Filtro de Kalman', 'Amostra', 'graus', {'Roll', 'Pitch'});
plotMadgwickTilt.setProperties('Filtro de Madgwick', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'});
plotQuat.setProperties('Quaterions do filtro de Madgwick', 'Amostra', 'val', {'q1', 'q2', 'q3', 'q4'});
plotAcel_G.setProperties('Aceleração em g sem gravidade', 'Amostra', 'g', {'aX', 'aY', 'aZ'});
plotCar3DGdeg.setProperties('Rotação 3D usando Posição angular relativa');
plotCar3DMadgwick.setProperties('Rotação 3D usando quaternions (filtro Madgwick)');
plotCar3DGtilt.setProperties('Rotação 3D usando Posição angular absoluta');
plotCar3DAcelMag.setProperties('Rotação 3D usando acel e mag');
plotCar3DComp.setProperties('Rotação 3D usando filtro complementar');
plotCar3DKalman.setProperties('Rotação 3D usando filtro de kalman');
plotSpace3D.setProperties('Posição 3D (usando filtro de kalman)');
plotCompass.setProperties('Magnetic Heading SEM compensação de tilt');
plotCompassCompensated.setProperties('Magnetic Heading COM compensação de tilt');

%% Inicializa os filtros de kalman, um para cada eixo,
% podemos fazer tudo com um filtro só, entretanto os parâmetros ficariam
% bem grandes e de dificil visualização
kalmanFilterRoll = Kalman(A,B,C,Q,R);
kalmanFilterPitch = Kalman(A,B,C,Q,R);
kalmanFilterYaw = Kalman(A,B,C,Q,R);

%% Inicializa a classe que irá fazer média movel, com tamanho 'window_k' p/ 9 elementos
moving_average = MovingAverage(window_k, 9);

%% Iniciaaliza o filtro de madgwick
madgwickFilter = MadgwickAHRS('SamplePeriod', 1/freq_sample, 'Beta', beta);

%% Abre porta serial / arquivo
if read_from_serial
    reader = ReaderSerial(serial_COM, serial_baudrate);
else
    reader = ReaderFile(file_full_path);
end

%% Configura as variáveis do MPU
esc_ac = str2int16(reader.metadatas.aesc_op);                   % Vem do Arduino, função que configura escalas de Aceleração
esc_giro = str2int16(reader.metadatas.gesc_op);                 % e giro //+/- 2g e +/-250gr/seg

%% Obtem os dados e plota em tempo real
% NOTA: Se o buffer do serial encher (> 512 bytes) o programa pode explodir ou apresentar erros, caso isso ocorra
% abaixe a taxa de renderização do gráfico. Para verificar se erros ocorreram, compare a quantidade de amostras enviadas com a quantidade lida
while true
    
    %% Lê uma amostra de cada da porta serial/arquivo
    data = reader.read_sample();

    % Se é o fim do arquivo ou deu algum erro finaliza
    if isempty(data)
        break;
    end

    count=count+1;
    
    data = str2int16(data);
    
    acel = calculate_aceleration(data(1:3), [ax_bias, ay_bias, az_bias], esc_ac);
    gyro = calculate_gyro(data(4:6), [gx_bias, gy_bias, gz_bias], esc_giro);
    mag = calculate_mag(data(7:9), [hx_offset, hy_offset, hz_offset], [hx_scale, hy_scale, hz_scale]);

    %% Aplica media movel
    filtered = moving_average.update([acel gyro mag]);

    %% Salva valores após aplicar a media movel
    ax = [ax(2:max_size) filtered(1)];
    ay = [ay(2:max_size) filtered(2)];
    az = [az(2:max_size) filtered(3)];
    gx = [gx(2:max_size) filtered(4)];
    gy = [gy(2:max_size) filtered(5)];
    gz = [gz(2:max_size) filtered(6)];
    hx = [hx(2:max_size) filtered(7)];
    hy = [hy(2:max_size) filtered(8)];
    hz = [hz(2:max_size) filtered(9)];

    plotAcel.update({ax, ay, az});
    plotGvel.update({gx, gy, gz});
    plotMag.update({hx, hy, hz});

    %% Calcula Yaw, Pitch e Roll realtivos(em relação a ultima Rotação do corpo) p/ a nova amostra usando giro
    % Usando o giro, fazemos a integral discreta (área do trapézio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trapézio (entre ultimo dado e o novo)
    % É considerado nesse calculo que, as amostragens estão espaçadas de 1
    % periodo da amostragem, ent o trapézio é igual a 1/freq * ((n-1 + n)/2)
    if isOneIn(setted_objects_name, {Gdeg, Gtilt, AcelMagTilt, CompTilt, Car3DGdeg, Car3DGtilt, Car3DAcelMag, Car3DComp, Acel_G, Vel, Space, Space3D})
        calculated = calculate_gyro_relative_tilt(...
            [gRoll(max_size), gPitch(max_size), gYaw(max_size)],...
            [gx(max_size), gy(max_size), gz(max_size)],...
            [gx(max_size-1), gy(max_size-1), gz(max_size-1)],...
            freq_sample...
        );

        gRoll   = [gRoll(2:max_size)  calculated(1)];
        gPitch  = [gPitch(2:max_size) calculated(2)];
        gYaw    = [gYaw(2:max_size)   calculated(3)];

        plotGdeg.update({gRoll, gPitch, gYaw});
    end
    
    %% Calcula a matriz de Rotação (Z,Y,X) que foi responsável por mover o corpo da Posição da amostra anterior para a atual
    % Ref do calculo: https://www.youtube.com/watch?v=wg9bI8-Qx2Q
    if isOneIn(setted_objects_name, {Gtilt, AcelMagTilt, CompTilt, Car3DGtilt, Car3DAcelMag, Car3DComp})
        calculated = calculate_gyro_absolute_tilt([gRoll(max_size), gPitch(max_size), gYaw(max_size)]);

        gRoll_abs  = [gRoll_abs(2:max_size)  calculated(1)];
        gPitch_abs = [gPitch_abs(2:max_size) calculated(2)];
        gYaw_abs   = [gYaw_abs(2:max_size)   calculated(3)];

        plotGtilt.update({gRoll_abs, gPitch_abs, gYaw_abs});
    end
    
    %% Calcula Yaw, Pitch e Roll absolutos(em relação a Posição inicial do corpo) p/ a nova amostra usando Aceleração e Magnetômetro
    % Usando a Aceleração, usamos o vetor de gravidade que deve sempre
    % estar presente p/ determinar a Posição do corpo
    % assim temos que p/ a orderm de Rotação Z,Y,X (note que na referencia ele faz o calculo com XYZ, isso se deve por conta do lado da equação ao qual a matriz aparece)
    % Ref do calculo: https://www.nxp.com/docs/en/application-note/AN3461.pdf
    if isOneIn(setted_objects_name, {AcelMagTilt, CompTilt, KalmanTilt, Acel_G, Vel, Space, Car3DAcelMag, Car3DKalman, Car3DComp, Space3D, CompassCompensated})
        calculated = calculate_acel_mag_tilt([ax(max_size), ay(max_size), az(max_size)], [hx(max_size), hy(max_size), hz(max_size)]);
        
        aRoll   = [aRoll(2:max_size)  calculated(1)];
        aPitch  = [aPitch(2:max_size) calculated(2)];
        mYaw    = [mYaw(2:max_size)   calculated(3)];

        plotAcelMagTilt.update({aRoll, aPitch, mYaw});
    end

    %% Calcula Compass sem compensação 
    % Ref do calculo: https://blog.digilentinc.com/how-to-convert-magnetometer-data-into-compass-heading/
    % e plota em plano polar o Compass sem compensação 
    if isOneIn(setted_objects_name, {Compass})
        calculated = compass_without_compensation([hx(max_size), hy(max_size), hz(max_size)]);
        plotCompass.rotateCompass(calculated);
    end

    %% Usa o valor calculado do compass com compensação já calculado acima
    % Ref do calculo: https://www.mikrocontroller.net/attachment/292888/AN4248.pdf
    if isOneIn(setted_objects_name, {CompassCompensated})
        plotCompassCompensated.rotateCompass(mYaw(max_size));
    end

    %% Calcula Rotação usando filtro complementar
    % Ref do calculo: https://www.youtube.com/watch?v=whSw42XddsU
    if isOneIn(setted_objects_name, {CompTilt, Car3DComp})
        calculated = calculate_comp_filter(...
            [compl_Roll(max_size), compl_Pitch(max_size), compl_Yaw(max_size)],...
            [gRoll(max_size), gPitch(max_size), gYaw(max_size)],...
            [gRoll(max_size-1), gPitch(max_size-1), gYaw(max_size-1)],...
            [aRoll(max_size), aPitch(max_size), mYaw(max_size)],...
            mu...
        );

        compl_Roll  = [compl_Roll(2:max_size)  calculated(1)];
        compl_Pitch = [compl_Pitch(2:max_size) calculated(2)];
        compl_Yaw   = [compl_Yaw(2:max_size)   calculated(3)];
    end

    %% Calcula Rotação usando filtro de Kalman
    % Ref do calculo: https://www.youtube.com/watch?v=urhaoECmCQk
    % e https://www.researchgate.net/publication/261038357_Embedded_Kalman_Filter_for_Inertial_Measurement_Unit_IMU_on_the_ATMega8535
    if isOneIn(setted_objects_name, {KalmanTilt, Car3DKalman})
        % Calcula a predição p/ cada eixo individualmente
        kalmanFilterRoll.predict(gx(max_size));
        kalmanFilterPitch.predict(gy(max_size));
        kalmanFilterYaw.predict(gz(max_size));

        % Atualiza a predição p/ cada eixo individualmente
        roll_and_bias = kalmanFilterRoll.update(aRoll(max_size));
        pitch_and_bias = kalmanFilterPitch.update(aPitch(max_size));
        yaw_and_bias = kalmanFilterYaw.update(mYaw(max_size));

        % Insere o valor filtrado no eixo a ser plotado
        kalman_Roll = [kalman_Roll(2:max_size)  roll_and_bias(1)];
        kalman_Pitch = [kalman_Pitch(2:max_size)  pitch_and_bias(1)];
        kalman_Yaw = [kalman_Yaw(2:max_size)  yaw_and_bias(1)];
    end
    
    %% Calcula Rotação usando filtro de madgwick
    % Ref do calculo: https://nitinjsanket.github.io/tutorials/attitudeest/madgwick
    % e https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
    if isOneIn(setted_objects_name, {MadgwickTilt, Car3DMadgwick, Quat})
        gyroscope = [gx(max_size), gy(max_size), gz(max_size)] * (pi/180) ;
        accelerometer = [ax(max_size), ay(max_size), az(max_size)];
        magnetometer = [hx(max_size), hy(max_size), hz(max_size)];
        madgwickFilter.Update(gyroscope, accelerometer, magnetometer);

        % Converte o resultado do filtro de quaternion para angulos absolutos (relativo a terra) em euler

        euler = quatern2euler(quaternConj(madgwickFilter.Quaternion)) * (180/pi);
        madgwick_Roll = [madgwick_Roll(2:max_size)      euler(1)];
        madgwick_Pitch = [madgwick_Pitch(2:max_size)    euler(2)];
        madgwick_Yaw = [madgwick_Yaw(2:max_size)        euler(3)];
    end

    %% Plota quaterions do filtro de madgwick
    if isOneIn(setted_objects_name, {Quat})
        q1 = [q1(2:max_size)    madgwickFilter.Quaternion(1)];
        q2 = [q2(2:max_size)    madgwickFilter.Quaternion(2)];
        q3 = [q3(2:max_size)    madgwickFilter.Quaternion(3)];
        q4 = [q4(2:max_size)    madgwickFilter.Quaternion(4)];
    end
    
    %% Calcula a Aceleração absoluta (relativo a terra) removendo a gravidade
    % Obtem os vetores de aceleração atual rotacionando NO MESMO SENTIDO realizado pelo corpo
    % (uma vez que os queremos os vetores que não rotacionam com o corpo, como a gravidade que sempre aponta p/ baixo)
    % (seria o equivalente a desrotacionar a plataforma e ver para onde os vetores de aceleração apontam agora)
    % e subtraindo a gravidade = 1g do eixo Z
    % Usando os dados do filtro de kalman desrotacionar os vetores
    % Ref do calculo: https://www.nxp.com/docs/en/application-note/AN3461.pdf
    if isOneIn(setted_objects_name, {Acel_G, Vel, Space, Space3D})
        calculated = calculate_aceleration_without_gravity([gRoll(max_size), gPitch(max_size), gYaw(max_size)], [ax(max_size); ay(max_size); az(max_size)]);

        ax_without_gravity = [ax_without_gravity(2:max_size) calculated(1)];
        ay_without_gravity = [ay_without_gravity(2:max_size) calculated(2)];
        az_without_gravity = [az_without_gravity(2:max_size) calculated(3)];
    end

    %% Calcula Velocidade integrando a Aceleração
    % Usando a Aceleração, fazemos a integral discreta (área do trapézio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trapézio (entre ultimo dado e o novo)
    % É considerado nesse calculo que, as amostragens estão espaçadas de 1
    % periodo da amostragem, ent o trapézio é igual a 1/freq * ((n-1 + n)/2)
    if isOneIn(setted_objects_name, {Vel, Space, Space3D})
        calculated = calculate_velocity(...
            [vx(max_size), vy(max_size), vz(max_size)],...
            [ax_without_gravity(max_size), ay_without_gravity(max_size), az_without_gravity(max_size)],...
            [ax_without_gravity(max_size-1), ay_without_gravity(max_size-1), az_without_gravity(max_size-1)],...
            freq_sample...
        );

        vx = [vx(2:max_size)  calculated(1)];
        vy = [vy(2:max_size)  calculated(2)];
        vz = [vz(2:max_size)  calculated(3)];
    end

    %% Calcula Posição integrando a Velocidade
    % Usando a Velocidade, fazemos a integral discreta (área do trapézio aculmulado)
    % P/ o novo dado isso significa, ultimo valor + novo trapézio (entre ultimo dado e o novo)
    % É considerado nesse calculo que, as amostragens estão espaçadas de 1
    % periodo da amostragem, ent o trapézio é igual a 1/freq * ((n-1 + n)/2)
    if isOneIn(setted_objects_name, {Space, Space3D})
        calculated = calculate_position(...
            [px(max_size), py(max_size), pz(max_size)],...
            [vx(max_size), vy(max_size), vz(max_size)],...
            [vx(max_size-1), vy(max_size-1), vz(max_size-1)],...
            freq_sample...
        );

        px = [px(2:max_size)  calculated(1)];
        py = [py(2:max_size)  calculated(2)];
        pz = [pz(2:max_size)  calculated(3)];
    end

    %% Plota o carro em 3d, podendo ser usado qualquer um dos dados para rotacionar o objeto (Rotação absoluta, relativa, filtro de kalman ...)
    if isOneIn(setted_objects_name, Car3DGdeg)
        plotCar3DGdeg.rotateWithEuler(gRoll(max_size), gPitch(max_size), gYaw(max_size));
    end

    if isOneIn(setted_objects_name, Car3DGtilt)
        plotCar3DGtilt.rotateWithEuler(gRoll_abs(max_size), gPitch_abs(max_size), gYaw_abs(max_size));
    end

    if isOneIn(setted_objects_name, Car3DAcelMag)
        plotCar3DAcelMag.rotateWithEuler(aRoll(max_size), aPitch(max_size), mYaw(max_size));
    end

    if isOneIn(setted_objects_name, Car3DComp)
        plotCar3DComp.rotateWithEuler(compl_Roll(max_size), compl_Pitch(max_size), compl_Yaw(max_size));
    end

    if isOneIn(setted_objects_name, Car3DKalman)
        plotCar3DKalman.rotateWithEuler(kalman_Roll(max_size), kalman_Pitch(max_size), kalman_Yaw(max_size));
    end

    if isOneIn(setted_objects_name, Car3DMadgwick)
        plotCar3DMadgwick.rotateWithQuaternion(madgwickFilter.Quaternion);
    end

    %% Tenta redesenhar o plot, se deu o tempo da frequencia
    if plot_in_real_time
        plot_1.try_render();
    end
end

%% Renderiza pela ultima vez, independente de ter dado o tempo da frequencia
plot_1.force_render();

%% Linka todos os eixos p/ poder dar zoom em todos de uma vez, so pode ser executado quando não tiver mais atualizações
% plot_1.linkAllAxes();

%% Aqui acaba o script
fprintf(1,'Recebidos %d amostras\n\n',count);
reader.delete();
return;