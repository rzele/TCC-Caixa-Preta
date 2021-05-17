classdef Configs < handle
    properties
        %% Configura��es est�ticas

        % Fonte de leitura
        read_from = 'mockup';     % difina como, 'serial', 'bluetooth', 'arquivo' ou 'mockup'

        % Se a fonte for 'arquivo' ou 'mockup': Coloque o path completo 
        file_full_path='Dados/simulados/rpr_seq_89'; 

        % Se a fonte for 'serial' ou 'bluetooth': Coloque a porta COM e a taxa de comunica��o
        serial_COM='COM4';
        serial_baudrate=115200;
        
        % Se a fonte for 'mockup': tamb�m defina os par�metros abaixo
        only_generate = false;           % Ir� apenas gerar os dados falso, n�o executa o programa de dashboard
        fake_sample_freq = 100;         % Frequ�ncia que simula a amostragem do MPU
        fake_esc_ac = 2;                % Escala do aceler�metro
        fake_esc_giro = 250;            % Escala do girosc�pio
        debug_on = false;                % Exibe um gr�fico com os dados gerados, antes de converter p/ as escalas

        % Amostragem
        max_size=3500;              % Quantidade maxima de amostras exibidas na tela

        % Plotagem
        plot_in_real_time=false;     % Define se o plot ser� so no final, ou em tempo real
        freq_render=20;               % Frequencia de atualiza��o do plot

        % Filtros das entradas (aplicado ao acel, gyro, mag e AcelMagTilt)
        window_k = 10;                    % Janela do filtro (minimo = 1, 1 equivale a n�o usar)
        filter_type = 'media';            % pode ser 'media' ou 'mediana'

        % Vari�vel de ajuste do filtro complementar
        mu=0.0015;

        % Vari�vel de ajuste do filtro madgwick
        beta=0.0180;

        % Constante gravitacional
        const_g=9.8;                

        %% Configura��es dinamicas
        % Qualquer coisa que precise de dados do mpu
        cxp % metadados da caixa preta

        layout
    end
    
    methods
        function obj = Configs()
        end

        function setDynamicConfigs(obj, caixaPretaMetadata)
            obj.cxp = caixaPretaMetadata;
        end

        function setLayout(obj, c)
            %% Define o layout de plotagem as visualiza��es poss�veis est�o no arquivo 'Charts.m'
            % O par�metro 'c', cont�m a inst�ncia de todos os gr�ficos
            %
            % OBS: Repita o objeto no layout p/ expandir o plot em varios grids
            % E.x.: layout = {aceleration, aceleration; gyroscope, magnetometer};
            obj.layout = {...

                % c.aceleration, c.gyroscope, c.magnetometer;...
                c.acel_mag_tilt, c.gyro_absolute_tilt, c.comp_tilt;...
                c.kalman_tilt, c.madgwick_tilt_euler, c.baseline_tilt;...
                c.compare_errors, c.compare_errors, c.compare_errors;...

                c.compare_errors, c.compare_errors, c.vazio;...
                % c.vazio, c.vazio, c.vazio;...

            };
        end
    end
end