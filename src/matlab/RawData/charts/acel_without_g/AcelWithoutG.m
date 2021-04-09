classdef AcelWithoutG < CommonsLine
    % Calcula a Aceleração absoluta (relativo a terra) removendo a gravidade
    % Obtem os vetores de aceleração atual rotacionando NO MESMO SENTIDO realizado pelo corpo
    % (uma vez que os queremos os vetores que não rotacionam com o corpo, como a gravidade que sempre aponta p/ baixo)
    % (seria o equivalente a desrotacionar a plataforma e ver para onde os vetores de aceleração apontam agora)
    % e subtraindo a gravidade = 1g do eixo Z
    % Usando os dados do filtro de kalman desrotacionar os vetores
    % Ref do calculo: https://www.nxp.com/docs/en/application-note/AN3461.pdf

    properties
    end

    methods
        function obj = AcelWithoutG(w_size, some_tilt_chart, acel_chart)
            p_title = sprintf('Acel em g sem gravidade (%s)', class(some_tilt_chart));
            obj = obj@CommonsLine(...
                p_title, ...                                % p_title
                'Amostra', ...                              % p_xlabel
                'g', ...                                    % p_ylabel
                {'aX', 'aY', 'aZ'}, ...                     % s_legend
                {'r', 'g', 'b'}, ...                        % sources_color
                w_size);

            obj.w_size = w_size;
            obj.data = zeros(w_size, 3);

            % Chart dependences
            obj.dependencies.some_tilt = some_tilt_chart;
            obj.dependencies.acel = acel_chart;
        end
        
        function calculate(obj, mpu_new_data, baselines_new_data, n_sample)
            %% Verifica se já calculou essa amostra
            if obj.has_calculated_this_sample(n_sample)
                return
            end
            
            %% Obtem o valor de outros charts ao qual este é dependente
            obj.dependencies.some_tilt.calculate(mpu_new_data, baselines_new_data, n_sample);
            tilt = obj.dependencies.some_tilt.last();
            
            obj.dependencies.acel.calculate(mpu_new_data, baselines_new_data, n_sample);
            A = obj.dependencies.acel.last();
            
            %% Calcula o valor p/ a próxima amostra
            t = tic();
            
            new_data = calculate_aceleration_without_gravity(tilt, A);
            
            obj.time = obj.time + toc(t);
            obj.data = [obj.data(2:obj.w_size, :); new_data];
        end
    end
end