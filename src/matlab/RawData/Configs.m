classdef Configs < handle
    properties
        %% Configura��es est�ticas

        % Fonte de leitura
        read_from_serial=false;     % Set to false to use a file
        serial_COM='COM4';
        serial_baudrate=115200;
        file_full_path='Dados/teste1.txt';

        % Amostragem
        max_size=4000;              % Quantidade maxima de amostras exibidas na tela

        % Plotagem
        plot_in_real_time=false;     % Define se o plot ser� so no final, ou em tempo real
        freq_render=5;               % Frequencia de atualiza��o do plot

        % Filtros das entradas (aplicado ao acel, gyro e mag)
        window_k = 10;                    % Janela do filtro (minimo = 2)
        filter_type = 'media';            % pode ser 'media' ou 'mediana'

        % Vari�vel de ajuste do filtro complementar
        mu=0.02;

        % Vari�vel de ajuste do filtro madgwick
        beta=0.1;

        % Constante gravitacional
        const_g=9.8;                

        %% Configura��es dinamicas
        % Qualquer coisa que precise de dados do mpu
        freq_sample
        gx_bias, gy_bias, gz_bias
        ax_bias, ay_bias, az_bias
        hx_offset, hy_offset, hz_offset
        hx_scale, hy_scale, hz_scale
        esc_ac, esc_giro
        A, B, C, Q, R

        cxp % metadados da caixa preta

        layout
    end
    
    methods
        function obj = Configs()
        end

        function setDynamicConfigs(obj, caixaPretaMetadata)
            obj.cxp = caixaPretaMetadata;

            %% Constantes do sensor
            obj.freq_sample=obj.cxp.fammost;            % Frequencia da amostragem dos dados
            obj.gx_bias=-1.05;                          % 
            obj.gy_bias=0.2;                            % 
            obj.gz_bias=-0.52;                          % 
            obj.ax_bias=0;                              % 
            obj.ay_bias=0;                              % 
            obj.az_bias=0.04;                           % 
            obj.hx_offset=-70;                          % 
            obj.hy_offset=228;                          % 
            obj.hz_offset=10;                           % 
            obj.hx_scale=1.020833;                      % 
            obj.hy_scale=0.940048;                      % 
            obj.hz_scale=1.045333;                      % 

            %% Configura as vari�veis do MPU
            obj.esc_ac = obj.cxp.aesc_op;        % Vem do Arduino, fun��o que configura escalas de Acelera��o
            obj.esc_giro = obj.cxp.gesc_op;      % e giro //+/- 2g e +/-250gr/seg

            %% Vari�vel de ajuste do filtro de kalman, os valores iniciais de X e P s�o por padr�o 0s
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
            % 
            % OBS: O filtro de kalman utilizado foi modificado para manter os intervalos
            % entre -180,180. Para entender melhor veja o arquivo 'ModifiedKalmanFilter.m'
            % Portanto, n�o recomendamos a modifica��o das
            % matrizes A,B e C, que representam a constru��o do modelo

            deltaT = 1/obj.freq_sample;
            obj.A = [1 deltaT; 0 1];
            obj.B = [deltaT; 0];
            obj.C = [1 0];
            obj.Q = [0.002^2 0; 0 0];
            obj.R = 0.03;
        end

        function setLayout(obj, c)
            %% Define o layout de plotagem as visualiza��es poss�veis est�o no arquivo 'Charts.m'
            % O par�metro 'c', cont�m a inst�ncia de todos os gr�ficos
            %
            % OBS: Repita o objeto no layout p/ expandir o plot em varios grids
            % E.x.: layout = {aceleration, aceleration; gyroscope, magnetometer};
            obj.layout = {...               

                c.compare_rolls;...

            };
        end
    end
end