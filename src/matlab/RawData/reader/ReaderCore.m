classdef ReaderCore < handle
    %READERCORE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        f_pt
        stop_wait = false
        read_attempts = 5
        file_simulated_freq
        read_current_attempts = 0
        time
    end
    
    methods
        function obj = ReaderCore()
        end

        function wait_start_signal(obj)
            while ~obj.stop_wait
                temp = fgetl(obj.f_pt);
                if strcmp(temp,'start') == 1
                    obj.stop_wait = true;
                    break;
                end
                fprintf('%s\n', temp);
            end
            fprintf('Iniciando leitura.\n');
        end

        function force_stop_wait(obj)
            obj.stop_wait = true;
        end

        function data = read_sample(obj)
            % Simula um delay de leitura, para simular leitura em tempo real
            while (now-obj.time)*100000 < 1/obj.file_simulated_freq
            end
            obj.time = now;

            % L� at� achar quebra de linha
            data=fgetl(obj.f_pt);
            
            % Checa se � o final da transmiss�o
            if ~isempty(strfind(data,'fim'))
                data = [];
                return;
            end

            % Quebra a string no marcador ';'
            data=strsplit(data,';');

            % Checa se realmente leu todos os dados, se n l� novamente at� no maximo 'read_attempts' vezes
            if length(data) < 7
                fprintf('Erro ao ler dados, tentando novamente - %d\n', obj.read_current_attempts);
                if (obj.read_current_attempts == obj.read_attempts)
                    data = [];
                    return;
                end
                obj.read_current_attempts = obj.read_current_attempts + 1;
                data = obj.read_sample();
                obj.read_current_attempts = 0;
            end
        end

        % Fecha o arquivo ou conex�o com porta serial
        function delete(obj)
            fclose(obj.f_pt);
        end
    end
end

