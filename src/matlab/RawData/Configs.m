classdef Configs < handle
    properties
        %% Configurações estáticas

        % Fonte de leitura
        read_from_serial=false;     % Set to false to use a file
        serial_COM='COM4';
        serial_baudrate=115200;
        file_full_path='Dados/teste1.txt';

        % Amostragem
        max_size=4000;              % Quantidade maxima de amostras exibidas na tela

        % Plotagem
        plot_in_real_time=false;     % Define se o plot será so no final, ou em tempo real
        freq_render=5;              % Frequencia de atualização do plot

        % Media movel parametros 
        window_k = 10;              % Janela da media movel (minimo = 2)

        % Variável de ajuste do filtro complementar
        mu=0.02;

        % Variável de ajuste do filtro madgwick
        beta=0.1;

        % Constante gravitacional
        const_g=9.8;                

        %% Configurações dinamicas
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

            %% Configura as variáveis do MPU
            obj.esc_ac = obj.cxp.aesc_op;        % Vem do Arduino, função que configura escalas de Aceleração
            obj.esc_giro = obj.cxp.gesc_op;      % e giro //+/- 2g e +/-250gr/seg

            %% Variável de ajuste do filtro de kalman, os valores iniciais de X e P são por padrão 0s
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

            deltaT = 1/obj.freq_sample;
            obj.A = [1 deltaT; 0 1];
            obj.B = [deltaT; 0];
            obj.C = [1 0];
            obj.Q = [0.002^2 0; 0 0];
            obj.R = 0.03;
        end

        function setLayout(obj, c)
            %% Define o layout de plotagem as visualizações possíveis estão no arquivo 'Charts.m'
            % O parâmetro 'c', contêm a instância de todos os gráficos
            %
            % OBS: Repita o objeto no layout p/ expandir o plot em varios grids
            % E.x.: layout = {aceleration, aceleration; gyroscope, magnetometer};
            obj.layout = {...                

                c.aceleration, c.gyroscope, c.magnetometer, c.gyro_relative_tilt, c.gyro_absolute_tilt;...
                c.acel_mag_tilt, c.comp_tilt, c.acel_without_g, c.velocity, c.position;...
                c.kalman_tilt, c.Madgwick_tilt, c.quaternion, c.compass_compensated, c.car_3d_gdeg;...
                c.car_3d_gtilt, c.car_3d_acelMag, c.car_3d_comp, c.car_3d_kalman, c.car_3d_madgwick;...

            };
        end
    end
end