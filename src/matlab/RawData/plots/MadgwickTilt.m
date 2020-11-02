classdef MadgwickTilt < TemplateLine
    % Calcula Rotação usando filtro de madgwick
    % Ref do calculo: https://nitinjsanket.github.io/tutorials/attitudeest/madgwick
    % e https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
    
    properties
        madgwickFilter
    end

    methods
        function obj = MadgwickTilt()
            obj.name = 'MadgwickTilt';
        end

        function obj = initialize(obj, fig, w_size, freq_sample, beta)
            obj.w_size = w_size;
            obj.my_plot = fig.setItemType(obj.name, 'plotline');
            obj.my_plot.configPlot('Filtro de Madgwick', 'Amostra', 'graus', {'Roll', 'Pitch', 'Yaw'}, {'r', 'g', 'b'});
            obj.data = zeros(w_size, 3);

            % Inicializa o filtro de madgwick
            obj.madgwickFilter = MadgwickAHRS('SamplePeriod', 1/freq_sample, 'Beta', beta);
        end

        function calculate(obj, G, A, H)
            % Calcula a predição p/ cada eixo individualmente
            obj.madgwickFilter.Update(G*pi/180, A, H);

            % Converte o resultado do filtro de quaternion para angulos absolutos (relativo a terra) em euler
            euler = quatern2euler(quaternConj(obj.madgwickFilter.Quaternion)) * (180/pi);
            obj.data = [obj.data(2:obj.w_size, :); euler];
        end

        function ret = last_quaternion(obj)
            ret = obj.madgwickFilter.Quaternion;
        end
    end
end